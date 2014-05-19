//
// Actual brute logic
// See .h for comments
//
#include "Brute.h"



void windGenerator(JavaLCGMimic &g, int index, long long &last_seed, int limit, int N, bool debug){
	if (debug) cout << endl << "[DEBUG] wind generator forward: ";
	for (int i = index; i < N; i++){
		if (limit == -1){
			long r = g.nextLong();
			if (debug) cout << r << " ";
		}
		else {
			int r;
			if (limit == 0)
				r = g.nextInt();
			else
				r = g.nextInt(limit);
			if (debug) cout << r << " ";
		}
	}
	last_seed = g.seed;
	if (debug) cout << endl << "[DEBUG] seed after: " << last_seed << endl;
}

//A B C D E
//B C D E F
//C D E F G
void malloc_check_matrix(int **&check_seq, Options &opt, int &N, vector<int> &seq){
	if (opt.opt_ms <= 0){
		N = seq.size() / 2;
		opt.opt_ms = seq.size() / 2;
	}
	else {
		N = seq.size() - opt.opt_ms;
	}
	check_seq = new int*[opt.opt_ms]; //rows
	for (int i = 0; i < opt.opt_ms; i++){
		check_seq[i] = new int[N + 1];
		check_seq[i][0] = seq[i];
		for (int j = 1; j <= N; j++){
			check_seq[i][j] = seq[i + j];
		}
	}
}
void free_check_matrix(int **check_seq, int opt_ms){
	for (int i = 0; i < opt_ms; i++)
		delete[] check_seq[i];
	delete[] check_seq;
}
void show_check_matrix(int **check_seq, int r, int N){ //DEBUG
	cout << "[DEBUG] Check matrix:" << endl;
	for (int i = 0; i < r; i++){
		cout << "[DEBUG] s0=" << check_seq[i][0] << ":";
		for (int j = 1; j <= N; j++){
			cout << " " << check_seq[i][j];
		}
		cout << endl;
	}
}
//
// ODD
//
//
//
void thr_thr_bruteOddLimitLCG(int index, found *sl, SafeLong *sl2, int **check_seq, int N, Options opt){
	//sl contains found flag for all threads
	//sl2 contains found flag for group of threads that brute seed for one of check_seq[index][0]
	//also sl2 contains high value for group of threads for one check_seq[index][0]
	
	bool debug = opt.opt_d;
	bool norm = opt.opt_norm;
	int limit = opt.opt_l;
	int cycles_per_thr = opt.opt_c;
	//int s1 = check_seq[index][1];

	JavaLCGMimic g1; // LCG generator
	
	long long high , high_min;
	if (norm){ //start chek from the middle
		minmaxval mmv = sl2->getValue2(limit);
		high = mmv.max;
		high_min = mmv.min;
	}
	else {
		high = sl2->getValue(limit);
		high_min = sl2->getValue_min();
	}

	if (debug) cout << "[DEBUG] ** Thread #" << setw(6) << this_thread::get_id() << " started, s0=" << check_seq[index][0] << ". high=" << high << ", high_min=" << high_min << endl;//DEBUG
	
	while (high >= high_min && !sl->found && high_min >= 0){
		for (int i = 0; i < cycles_per_thr && !sl->found && high >= high_min; i++){
			//cout << "[DEBUG] Thread #" << setw(6) << this_thread::get_id() << " started, s0=" << check_seq[index][0] 
			//	<< ", i=" << i << ", high="<<high<<endl; //DEBUG
			long long low = 0; // 17 low bits of seed
			while ((low < 131072) && !sl->found){ //2**17=131072  //cycle through low bits
				long long x = (high << 17) + low; // try next seed
				g1.forceSeed(x);

				bool flag = true;
				for (int j = 1; j <= N; j++){ //check if x produces sequence
					if (check_seq[index][j] != g1.nextInt(limit)){
						flag = false;
						break;
					}
				}
				if (flag){ //found seed 
					sl->found = true;
					sl->seed = x;
					sl->index = index;
					if (debug) cout << "[DEBUG] Thread #" << setw(6) << this_thread::get_id()
						<< " found seed=" << sl->seed << "!" << endl;//DEBUG
					break;
				}
				low++;
			}
			high -= limit;
		}
		//if (debug) cout << "[DEBUG]\tThread #" << setw(6) << this_thread::get_id() << " done " << cycles_per_thr
		//	<< " cycles for s0=" << check_seq[index][0] << ". last seed=" << ((high << 17) + 131072 - 1) << ", found=" << sl.found << endl;//DEBUG

		if (sl->found) break;
		if (norm){ //start chek from the middle
			minmaxval mmv = sl2->getValue2(limit);
			high = mmv.max;
			high_min = mmv.min;
		}
		else {
			high = sl2->getValue(limit);
			high_min = sl2->getValue_min();
		}
	}
}
//thread for matrix of sequence
void thr_bruteOddLimitLCG(int index, found *sl, int **check_seq, int N, Options opt){
	bool debug = opt.opt_d;
	int threads_num = opt.opt_t; //number of threads 

	int s0 = check_seq[index][0];

	//we search for high value which is look like[s0 + limit*j], where j is some integer
	//we count from 2 ^ 31 down to 0, it gives us some boost
	//long long maxj = (pow(2,31) - s0) / limit;
	//long long high = s0 + limit*maxj; // 31 high bits of seed
	long long maxj = (2147483648 - static_cast<long long>(s0)) / static_cast<long long>(opt.opt_l);//2^31 = 2 147 483 648

	long long high_min = 0, 
		high = static_cast<long long>(s0) + maxj*static_cast<long long>(opt.opt_l);
	
	if (debug)
		cout << "[DEBUG] Thread #" << setw(6) << this_thread::get_id() << ". s0=" << s0 << ", high=" << high << ", high_min=" << high_min << endl;

	SafeLong sl2(high, high_min, opt.opt_c);
	thread *threads = new thread[threads_num];
	for (int i = 0; i < threads_num; i++){
		threads[i] = thread(thr_thr_bruteOddLimitLCG, index, sl, &sl2, check_seq, N, opt);
		this_thread::sleep_for(chrono::milliseconds(1)); //sleep 1 ms
	}
	for (int i = 0; i < threads_num; i++){ //wait
		threads[i].join();
	}
	delete[] threads;
}

long long bruteOddLimitLCG(Options &opt, vector<int> &seq) {
	clock_t startt = clock(); //start time
	
	JavaLCGMimic g1;
	found sl;
	
	if (opt.opt_sin > 0 || opt.opt_sax > 0){
		long long high_min, high;
		high_min = opt.opt_sin > 0 ? (opt.opt_sin >> 17) : 0;
		high = opt.opt_sax > 0 ? (opt.opt_sax >> 17) : 2147483648;//2^31

		SafeLong sl2(high, high_min, opt.opt_c);
		thread *threads = new thread[opt.opt_t];
		for (int i = 0; i < opt.opt_t; i++){
			threads[i] = thread(thr_thr_brute_st, &sl, &sl2, seq, opt);
			this_thread::sleep_for(chrono::milliseconds(1)); //sleep 1 ms
		}
		for (int i = 0; i < opt.opt_t; i++){ //wait
			threads[i].join();
		}
		delete[] threads;
	}
	else {
		int **check_seq = 0; //matrix made of sequence to test elements simultaneously
		int N = 0; //matrix size
		malloc_check_matrix(check_seq, opt, N, seq);
		if (opt.opt_d) show_check_matrix(check_seq, opt.opt_ms, N);

		thread *threads = new thread[opt.opt_ms];

		opt.opt_t = opt.opt_t / opt.opt_ms > 0 ? opt.opt_t / opt.opt_ms : 1;
		for (int i = 0; i < opt.opt_ms; i++){ //threads for elements in sequence
			threads[i] = thread(thr_bruteOddLimitLCG, i, &sl, check_seq, N, opt);
			this_thread::sleep_for(chrono::milliseconds(1)); //sleep 1 ms
		}
		for (int i = 0; i < opt.opt_ms; i++){ //wait
			threads[i].join();
		}
		delete[] threads;

		free_check_matrix(check_seq, opt.opt_ms);
	}
	if (opt.opt_d)
		cout << endl << "[DEBUG] bruteOddLimitLCG finished in " << (clock() - startt) << " msec" << endl;
	if (!sl.found){
		if (opt.opt_d) cout << "[DEBUG] Not found seed" << endl;
		return -1; //no seed found
	}

	if (opt.opt_d) cout << "[DEBUG] seed = " << sl.seed << endl;

	// wind generator to the end of sequence
	g1.forceSeed(sl.seed);
	long long last_seed;
	windGenerator(g1, sl.index, last_seed, opt.opt_l, seq.size(), opt.opt_d);

	return last_seed;
	
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// ODD ADVANCED
//
//
void thr_thr_bruteOddLimitLCG_adv(int index, found *sl, SafeLong *sl2, periods prd, int **check_seq, int N, Options opt){
	bool debug = opt.opt_d;
	//int threads_num = opt.opt_t; //number of threads 
	bool norm = opt.opt_norm;
	int limit = opt.opt_l;
	int s1 = check_seq[index][1];
	//int s0 = check_seq[index][0];

	JavaLCGMimic g1; // LCG generator

	long long high, high_min;
	if (norm){ //start chek from the middle
		minmaxval mmv = sl2->getValue2(limit);
		high = mmv.max;
		high_min = mmv.min;
	}
	else {
		high = sl2->getValue(limit);
		high_min = sl2->getValue_min();
	}
	

	if (debug) cout << "[DEBUG] ** Thread #" << setw(6) << this_thread::get_id() << " started, s0=" << check_seq[index][0] << ". high=" << high << ", high_min=" << high_min << endl;//DEBUG

	//high = 1701680758; //DEBUG

	while (high_min >= 0){

		if (opt.opt_ds){ //Depth search strategy
			if (debug)
				cout << "[DEBUG] Thread #" << setw(6) << this_thread::get_id() << ". Start depth strategy. p=" << prd.p << ", d=" << prd.d << endl;

			while (high > high_min && !sl->found){
				
				for (int l = 0; l < prd.d && !sl->found; l++){
					int low = l;

					g1.forceSeed( (high << 17) + low );
					//if (index == 0)  cout << "force = " << (static_cast<long long>(high << 17) + static_cast<long long>(low)) << endl; //DEBUG

					g1.nextInt(limit);
					long long Adh = ((g1.multiplier*prd.d) >> 17) % 2147483648;//2 147 483 648 = 2^31
					long long ii = 0; //index for JJ array
					
					vector<long long> JJ;
					JJ.push_back((2147483648 - (g1.seed >> 17)) / Adh);
					//if (index == 0) cout << "Adh = " << Adh << ", (seed)g1.seed >> 17 = ("<<g1.seed<<")" << (g1.seed >> 17)<<endl; //DEBUG
					long long Xh = ((g1.seed >> 17) + Adh*(JJ[0] + 1)) % 2147483648;
					
					long long S = JJ[0];
					// Fill JJ array
					while ( S <= (131072/prd.d) ){ //131 072 = 2^17
						long long Ji = ((2147483648 - Xh) / Adh);
						S += Ji;
						JJ.push_back(JJ.back() + Ji + 1);
						Xh = ( Xh + Adh*(Ji + 1) ) % 2147483648;
					}
					//if (index == 0) cout << "high = "<<high<< ", JJ = " << JJ << endl; //DEBUG


					while (low <= 131072 && !sl->found){ //2^17
						long long x = (high << 17) + static_cast<long long>(low);
						g1.forceSeed(x);
						int r = g1.nextInt(limit);
						int delta = (r - s1 + limit) % limit;

						if (delta != 0){
							int skip;
							if ( (low / prd.d + delta) >= JJ[ii] ){
								skip = JJ[ii] + 1 - low / prd.d;
								ii++;
							}
							else
								skip = delta;

							low += prd.d * skip;
						}
						else {//check if we on right element
							//show_check_matrix(check_seq, opt.opt_ms, N); //DEBUG
							bool flag = true;
							for (int j = 2; j <= N; j++){ //check if x produces sequence
								if (check_seq[index][j] != g1.nextInt(limit)){
									flag = false;
									break;
								}
							}
							if (flag){ //found seed 
								sl->found = true;
								sl->seed = x;
								sl->index = index;
								if (debug) cout << "[DEBUG] Thread #" << setw(6) << this_thread::get_id()
									<< " found seed=" << sl->seed << "!" << endl;//DEBUG
								break;
							}

							low += prd.d;
						}
					}
				}
				high -= limit;
			}
		}
		else { //Breadth search strategy
			if (debug)
				cout << "[DEBUG] Thread #" << setw(6) << this_thread::get_id() << ". Start breadth strategy. p=" << prd.p << ", d=" << prd.d << endl;

			vector<int> lows_;
			vector<index_elem> indexes_;
			for (int i = 0; i < prd.d; i++){
				index_elem e;
				lows_.push_back(i);
				e.index = i;
				indexes_.push_back(e);
			}

			while (high > high_min && !sl->found){
				vector<int> lows(lows_);
				vector<index_elem> indexes(indexes_);

				//if (high % 10000 == 1)
				//	cout << "[DEBUG] Thread #" << setw(6) << this_thread::get_id() << ". min seed = " << (high << 17) << endl;


				long long J0 = (static_cast<long long>(pow(2, 31)) - high) / (((g1.multiplier*prd.d) >> 17) % static_cast<long long>(pow(2, 31)));
				long long ii = 0;

				unsigned int deleted_cnt;
				do {
					deleted_cnt = 0;
					for (auto li = indexes.begin(); li != indexes.end() && !sl->found; li++){
						if (li->deleted){
							deleted_cnt++;
						}
						else {
							int low = lows[li->index];

							if (low > 131072){ //2^17
								li->deleted = true;
							}
							else {
								long long x = (high << 17) + low;
								g1.forceSeed(x);
								int r = g1.nextInt(limit);
								int delta = (r - s1 + limit) % limit;

								if (delta != 0){
									int skip;
									if ((low / prd.d + delta) > (J0 + ii*prd.period)){
										skip = max(1, delta - prd.p - 1);
										ii++;
									}
									else
										skip = delta;

									lows[li->index] += prd.d * skip;
								}
								else{ //check if we on right element
									bool flag = true;
									for (int j = 2; j <= N; j++){ //check if x produces sequence
										if (check_seq[index][j] != g1.nextInt(limit)){
											flag = false;
											break;
										}
									}
									if (flag){ //found seed 
										sl->found = true;
										sl->seed = x;
										sl->index = index;
										if (debug) cout << "[DEBUG] Thread #" << setw(6) << this_thread::get_id()
											<< " found seed=" << sl->seed << "!" << endl;//DEBUG
										break;
									}

									lows[li->index] += prd.d;
								}
							}
						}
					}
				} while (indexes.size() > deleted_cnt && !sl->found);

				high -= limit;
			}
		}// breadth search strategy
	
		if (sl->found) break;
		if (norm){ //start chek from the middle
			minmaxval mmv = sl2->getValue2(limit);
			high = mmv.max;
			high_min = mmv.min;
		}
		else {
			high = sl2->getValue(limit);
			high_min = sl2->getValue_min();
		}
	}
}

void thr_bruteOddLimitLCG_adv(int index, found *sl, periods prd, int **check_seq, int N, Options opt){
	//bool debug = opt.opt_d;
	int threads_num = opt.opt_t; //number of threads 

	int s0 = check_seq[index][0];

	//we search for high value which is look like[s0 + limit*j], where j is some integer
	//we count from 2 ^ 31 down to 0, it gives us some boost
	//long long maxj = (pow(2,31) - s0) / limit;
	//long long high = s0 + limit*maxj; // 31 high bits of seed
	long long maxj = (2147483648 - static_cast<long long>(s0)) / static_cast<long long>(opt.opt_l);//2^31 = 2 147 483 648

	long long high_min = 0, 
		high = static_cast<long long>(s0)+maxj*static_cast<long long>(opt.opt_l);


	SafeLong sl2(high, high_min, opt.opt_c);
	thread *threads = new thread[threads_num];
	for (int i = 0; i < threads_num; i++){
		threads[i] = thread(thr_thr_bruteOddLimitLCG_adv, index, sl, &sl2, prd, check_seq, N, opt);
		this_thread::sleep_for(chrono::milliseconds(1)); //sleep 1 ms
	}
	for (int i = 0; i < threads_num; i++){ //wait
		threads[i].join();
	}
	delete[] threads;
}

long long bruteOddLimitLCG_adv(Options &opt, vector<int> &seq) {
	clock_t startt = clock(); //start time

	JavaLCGMimic g1;
	found sl;

	periods prd;
	prd.d = computePeriod(opt.opt_l, g1.multiplier);
	prd.p = static_cast<int>(static_cast<long long>(pow(2, 31)) % static_cast<long long>(opt.opt_l));
	prd.period = pow(2, 31) / (((g1.multiplier * prd.d) >> 17) % static_cast<long>(pow(2, 31)));

	if (opt.opt_l > prd.period){
		cout << "[+] Fall to simple brute :(" << endl; //DEBUG
		return bruteOddLimitLCG(opt, seq);
	}

	cout << "[+] d=" << prd.d << " period=" << floor(prd.period) << " Starting advansed brute" << endl; //DEBUG
	
	if (opt.opt_sin > 0 || opt.opt_sax > 0){
		long long high_min, high;
		high_min = opt.opt_sin > 0 ? (opt.opt_sin >> 17) : 0;
		high = opt.opt_sax > 0 ? (opt.opt_sax >> 17) : 2147483648;//2^31

		SafeLong sl2(high, high_min, opt.opt_c);
		thread *threads = new thread[opt.opt_t];
		for (int i = 0; i < opt.opt_t; i++){
			threads[i] = thread(thr_thr_brute_st_adv, &sl, &sl2, prd, seq, opt);
			this_thread::sleep_for(chrono::milliseconds(1)); //sleep 1 ms
		}
		for (int i = 0; i < opt.opt_t; i++){ //wait
			threads[i].join();
		}
		delete[] threads;
	}
	else {
		int **check_seq = 0; //matrix made of sequence to test elements simultaneously
		int N = 0; //matrix size
		malloc_check_matrix(check_seq, opt, N, seq);
		if (opt.opt_d) show_check_matrix(check_seq, opt.opt_ms, N);

		thread *threads = new thread[opt.opt_ms];

		opt.opt_t = opt.opt_t / opt.opt_ms > 0 ? opt.opt_t / opt.opt_ms : 1;
		for (int i = 0; i < opt.opt_ms; i++){ //threads for elements in sequence
			threads[i] = thread(thr_bruteOddLimitLCG_adv, i, &sl, prd, check_seq, N, opt);
			this_thread::sleep_for(chrono::milliseconds(1)); //sleep 1 ms
		}
		for (int i = 0; i < opt.opt_ms; i++){ //wait
			threads[i].join();
		}
		delete[] threads;

		free_check_matrix(check_seq, opt.opt_ms);
	}

	if (opt.opt_d)
		cout << endl << "[DEBUG] bruteOddLimitLCG_adv finished in " << (clock() - startt) << " msec" << endl;
	if (!sl.found){
		if (opt.opt_d) cout << "[DEBUG] Not found seed" << endl;
		return -1; //no seed found
	}

	if (opt.opt_d) cout << "[DEBUG] seed = " << sl.seed << endl;

	// wind generator to the end of sequence
	g1.forceSeed(sl.seed);
	long long last_seed;
	windGenerator(g1, sl.index, last_seed, opt.opt_l, seq.size(), opt.opt_d);

	return last_seed;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
// POWER OF 2
//
//recover LCG seed when limit is power of 2
//complexity is O(2 ^ (37 - i)) where i is log2(limit)
//i.e.when limit = 128, complexity is O(2 ^ 30)
void thr_thr_brutePowerOf2LimitLCG2(found *sl, SafeLong *sl2, vector<int> seq, Options opt, int p, long long skip){
	bool debug = opt.opt_d;
	int limit = opt.opt_l;
	bool norm = opt.opt_norm;

	//int s1 = seq[1];
	int s0 = seq[0];

	JavaLCGMimic g1; // LCG generator

	if (debug) cout << "[DEBUG] Thread #" << setw(6) << this_thread::get_id() << " Entering thr_thr_brutePowerOf2LimitLCG2..." << endl;

	//long long high = 1842953096, high_min = 1842953095; //54 51 63 58 44 22 43 54 13 28 9 47 13 1 42 48 49 24 62 32 55 43 59 4 45 31 52 57 9 27 24, seed=241559548313664, low=114752
	long long high, high_min;
	if (norm){ //start chek from the middle
		minmaxval mmv = sl2->getValue2();
		high = mmv.max;
		high_min = mmv.min;
	}
	else {
		high = sl2->getValue();
		high_min = sl2->getValue_min();
	}

	while (high >= high_min && !sl->found && high_min >= 0) {
		//if (s0 == 126) cout << sl2 << endl; //DEBUG

		int cycles_per_thr = opt.opt_c;
		for (int i = 0; i < cycles_per_thr && !sl->found && high >= high_min; i++, high--){

			long long low = 0, // 17 low bits of seed
				jumped_low; //limit we meet after jump
			int direction; //direction of search low (actually - low curretn increment)
			bool allow_skip = true; //can we jump now?
			int tmp_prev = -1; //s1 from previous step
			while (low < 131072 && !sl->found){ //2**17=131072  //cycle through low bits
				long long x = (high << 17) + low; // try next seed
				g1.forceSeed(x);

				//If we seed generator with values[high << 17 + z]
				//where high is fixed and 0 <= z < 2 ^ (13 - p)
				//13 because 13 = 48 - 35, where 35 = log2(multipler of LCG)
				//we get same first output from LCG for all z
				//we can skip 2 ^ (13 - p) iterations if LCG[x].next != seq[0]

				int tmp = g1.nextInt(limit);
				if (allow_skip){
					if (tmp_prev != s0){
						if (tmp != s0) { //jumped somewhere (general case) - we can jump 
							low += skip*(s0>tmp ? s0 - tmp : s0 - tmp + limit);
						}
						else { //jumped to s0 area - we are in te middle of s1 area
							jumped_low = low; //remember low
							direction = -1; //go back
							allow_skip = false;
						}
					}
					tmp_prev = tmp;
					continue;
				}
				if (tmp != s0 && tmp_prev == s0){ //leaved s0 area
					if (direction > 0){ //leaved forward - allow jump again
						allow_skip = true;
						low += skip*(s0 > tmp ? s0 - tmp : s0 - tmp + limit);
					}
					else { //leaved back - return to the previous jump position
						low = jumped_low + 1;
						direction = 1; //go forward
					}
					tmp_prev = tmp;
					continue;
				}
				tmp_prev = tmp;

				g1.forceSeed(x);
				bool flag = true;
				for (int j = 0; j <seq.size(); j++){ //check if x produces sequence
					if (seq[j] != g1.nextInt(limit)){
						flag = false;
						break;
					}
				}
				if (flag){ //found seed 
					sl->found = true;
					sl->seed = x;
					sl->index = 0;
					if (debug) cout << "\t!!!!![DEBUG] Thread #" << setw(6) << this_thread::get_id()
						<< " found seed=" << sl->seed << "(" << high << "," << low << ")!!!!!" << endl;//DEBUG
					break;
				}
				low += direction;
			}
		}

		//get next
		if (norm){
			minmaxval mmv = sl2->getValue2();
			high = mmv.max;
			high_min = mmv.min;
		}
		else {
			high = sl2->getValue();
			high_min = sl2->getValue_min();
		}
	}
}
void thr_thr_brutePowerOf2LimitLCG(int index, found *sl, SafeLong *sl2, int **check_seq, int N, Options opt, int p, long long skip){
	bool debug = opt.opt_d;
	int limit = opt.opt_l;
	bool norm = opt.opt_norm;
	
	//int s0 = check_seq[index][0];
	int s1 = check_seq[index][1];
	
	//if (debug) cout << "[DEBUG] Thread #" << setw(6) << this_thread::get_id() << " started for s0=" << check_seq[index][0] << endl;//DEBUG

	JavaLCGMimic g1; // LCG generator
	//mutex found_mtx; //in this case it's possible race condition (between threads for different s0)
	
	//long long high = 1842953096, high_min = 1842953095; //54 51 63 58 44 22 43 54 13 28 9 47 13 1 42 48 49 24 62 32 55 43 59 4 45 31 52 57 9 27 24, seed=241559548313664, low=114752
	long long high, high_min;
	if (norm){ //start chek from the middle
		minmaxval mmv = sl2->getValue2();
		high = mmv.max;
		high_min = mmv.min;
	}
	else {
		high = sl2->getValue();
		high_min = sl2->getValue_min();
	}

	while ( high >= high_min && !sl->found && high_min >= 0 ) {
		//if (s0 == 126) cout << sl2 << endl; //DEBUG

		int cycles_per_thr = opt.opt_c;
		for (int i = 0; i < cycles_per_thr && !sl->found && high >= high_min; i++, high--){
			
			long long low = 0, // 17 low bits of seed
				jumped_low; //limit we meet after jump
			int direction; //direction of search low (actually - low curretn increment)
			bool allow_skip  = true; //can we jump now?
			int tmp_prev = -1; //s1 from previous step
			while (low < 131072 && !sl->found){ //2**17=131072  //cycle through low bits
				long long x = (high << 17) + low; // try next seed
				g1.forceSeed(x);

				//If we seed generator with values[high << 17 + z]
				//where high is fixed and 0 <= z < 2 ^ (13 - p)
				//13 because 13 = 48 - 35, where 35 = log2(multipler of LCG)
				//we get same first output from LCG for all z
				//we can skip 2 ^ (13 - p) iterations if LCG[x].next != seq[0]

				int tmp = g1.nextInt(limit);
				if (allow_skip){
					if (tmp_prev != s1){
						if (tmp != s1 ) { //jumped somewhere (general case) - we an jump 
							//if (s1 == 51) cout << "SKIP: low=" << low << " prev=" << tmp_prev << " tmp=" << tmp << " s1=" << s1 << " new_low=";
							low += skip*(s1>tmp? s1-tmp : s1-tmp+limit);
							//if (s1 == 51) cout << low << endl;
						}
						else { //jumped to s0 area - we are in te middle of s0 area
							//if (s1 == 51) cout << "SKIP BACK: low=" << low << " prev=" << tmp_prev << " tmp=" << tmp << " s1=" << s1 << endl;
							jumped_low = low; //remember low
							direction = -1; //go back
							allow_skip = false;
						}
					}
					tmp_prev = tmp;
					continue;
				}
				if (tmp != s1 && tmp_prev == s1){ //leaved s0 area
					if (direction > 0){ //leaved forward - allow jump again
						//if (s1 == 51) cout << "LEAVED s0 AREA: low=" << low << " prev=" << tmp_prev << " tmp=" << tmp << " s1=" << s1 << endl;
						allow_skip = true;
						low += skip*(s1 > tmp ? s1 - tmp : s1 - tmp + limit);
					}
					else { //leaved back - return to the previous jump position
						low = jumped_low + 1;
						direction = 1; //go forward
					}
					tmp_prev = tmp;
					continue;
				}
				//if (s1 == 51) cout << "IN s0 AREA one by one: low=" << low << " prev=" << tmp_prev << " tmp=" << tmp << " s1=" << s1 << endl;
				tmp_prev = tmp;

				/*if (low > 114752){ //seed=241559548313664
					if (s1 == 51) cout << "low= " << low << " s1=" << s1 << " WARNING!" << endl;
					return;
				}*/
				
				g1.forceSeed(x);
				bool flag = true;
				for (int j = 1; j <= N; j++){ //check if x produces sequence
					if (check_seq[index][j] != g1.nextInt(limit)){
						flag = false;
						break;
					}
				}
				if (flag){ //found seed 
					sl->found = true;
					sl->seed = x;
					sl->index = index;
					if (debug) cout << "\t!!!!![DEBUG] Thread #" << setw(6) << this_thread::get_id() 
						<< " found seed=" << sl->seed << "(" << high << "," << low << "), s0=" << check_seq[index][0] << "!!!!!" << endl;//DEBUG
					break;
				}
				low += direction;
			}
		}
		//if (debug) cout << "[DEBUG] Thread #" << setw(6) << this_thread::get_id() << " done " << cycles_per_thr
		//	<< " cycles for s0=" << check_seq[index][0] << ". last high=" << high << ", found=" << sl.found << endl;//DEBUG

		//get next
		if (norm){ 
			minmaxval mmv = sl2->getValue2();
			high = mmv.max;
			high_min = mmv.min;
		}
		else {
			high = sl2->getValue();
			high_min = sl2->getValue_min();
		}
	}
}
void thr_brutePowerOf2LimitLCG(int index, found *sl, int **check_seq, int N, Options opt, int p, long long skip){
	int threads_num = opt.opt_t; //number of threads (updated)
	
	int s0 = check_seq[index][0];

	//high 31 bits of seed
	//MSB of high contains s0
	//we loop from high values down to low, this gives us some boost
	long long high_min = static_cast<long long>(s0) << (31 - p), 
		high = (static_cast<long long>(s0) << (31 - p) ) + static_cast<long long>(pow(2, (31 - p) ) ) - 1L;// 31 high bits of seed
	
	//if (debug) cout << "[DEBUG] Thread #" << setw(6) << this_thread::get_id() 
	//	<< ". s0=" << s0 << ", high=" << high <<"high_min="<<high_min<< endl;

	SafeLong sl2(high, high_min, opt.opt_c); //shared between threads for one s0
	thread *threads = new thread[threads_num];
	for (int i = 0; i < threads_num; i++){
		threads[i] = thread(thr_thr_brutePowerOf2LimitLCG, index, sl, &sl2, check_seq, N, opt, p, skip);
		this_thread::sleep_for(chrono::milliseconds(1)); //sleep 1 ms
	}
	for (int i = 0; i < threads_num; i++){ //wait
		threads[i].join();
	}
	delete[] threads;
}
long long brutePowerOf2LimitLCG(Options &opt, vector<int> &seq) {
	clock_t startt = clock(); //start time
	found sl;

	int p = maxPowerOf2(opt.opt_l);//log2(limit)
	long long skip = static_cast<long long>(pow(2, 13.44644 - p)); //jump step
	
	if (opt.opt_sin > 0 || opt.opt_sax > 0){
		long long high_min, high;
		high_min = opt.opt_sin > 0 ? (opt.opt_sin >> 17) : 0;
		high = opt.opt_sax > 0 ? (opt.opt_sax >> 17) : 2147483648;//2^31

		SafeLong sl2(high, high_min, opt.opt_c);
		thread *threads = new thread[opt.opt_t];
		for (int i = 0; i < opt.opt_t; i++){
			threads[i] = thread(thr_thr_brutePowerOf2LimitLCG2, &sl, &sl2, seq, opt, p, skip);
			this_thread::sleep_for(chrono::milliseconds(1)); //sleep 1 ms
		}
		for (int i = 0; i < opt.opt_t; i++){ //wait
			threads[i].join();
		}
		delete[] threads;
	}
	else {
		int **check_seq = 0; //matrix made of sequence to test elements simultaneously
		int N = 0; //matrix size == number of elements to check
		malloc_check_matrix(check_seq, opt, N, seq);
		if (opt.opt_d) show_check_matrix(check_seq, opt.opt_ms, N);

		opt.opt_t = opt.opt_t / opt.opt_ms > 0 ? opt.opt_t / opt.opt_ms : 1; //number of threads 
		thread *threads = new thread[opt.opt_ms];
		for (int i = 0; i < opt.opt_ms; i++){
			threads[i] = thread(thr_brutePowerOf2LimitLCG, i, &sl, check_seq, N, opt, p, skip);
			this_thread::sleep_for(chrono::milliseconds(1)); //sleep 1 ms
		}
		for (int i = 0; i < opt.opt_ms; i++){ //wait
			threads[i].join();
		}
		delete[] threads;

		free_check_matrix(check_seq, opt.opt_ms);
	}


	if (opt.opt_d)
		cout << endl << "[DEBUG] brutePowerOf2LimitLCG finished in " << (clock() - startt) << " msec" << endl;

	if (!sl.found){
		if (opt.opt_d) cout << "[DEBUG] Not found seed" << endl;
		return -1; //no seed found
	}

	if (opt.opt_d) cout << "[DEBUG] seed = " << sl.seed << endl;

	// wind generator to the end of sequence
	JavaLCGMimic g1; // LCG generator
	g1.forceSeed(sl.seed);
	long long last_seed;
	windGenerator(g1, sl.index, last_seed, opt.opt_l, seq.size(), opt.opt_d);

	return last_seed;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//recover LCG seed when limit is even, but not the power of 2
//complexity is O(2 ^ (31 - i)) where i is log2(limit)
//i.e.when limit = 26, complexity is O(2 ^ 26)
long long bruteEvenLimitLCG(int limit, vector<int> &seq, bool debug) {
	if (debug) cout << "[DEBUG] bruteEvenLimitLCG started" << endl;
	
	long long low = 0; // 17 low bits of seed
	long long high = 0; // 31 high bits of seed
	int maxPowOf2_l = maxPowerOf2(limit); //p

	clock_t startt = clock(); //start time

	JavaLCGSub g1(0, limit); // generator with reduced seed

	int s0 = seq[0]; //remember first
	seq.erase(seq.begin()); //delete first
	int seqsize = seq.size();

	// we initialize g1 with different seed values x in a loop
	// x_high - high bits of x
	// x_high consists of maxPowerOf2(limit) bits
	// x_high bits contains first element of the sequence s0		

	long long x_high = static_cast<long long>(s0 % static_cast<int>(pow(2, maxPowOf2_l))) << 17;

	// if sequence is small, multiple low values could produce it
	// in that case we can not recover valid seed
	// minimal sequence length depends on limit value

	vector<long long> lows;
	

	// loop to recover low bits of seed
	// complexity is O(2 ^ 17)

	while (low < 131072) { //2**17
		g1.forceSeed(x_high + low); // new seed to try

		bool flag = true;
		for (int i = 0; i < seqsize; i++){ //check if x produces LSB of sequence
			if ( g1.nextInt() != seq[i] % static_cast<int>(pow(2, maxPowOf2_l)) ){
				flag = false;
				break; // x not produces sequence
			}
		}
		if (flag) lows.push_back(low); // we found x that produces sequence

		low += 1; //take next low
	}

	if (lows.size() == 0){ //no seed found
		if(debug) cout << "[DEBUG] No seed found" << endl;
		return -1;  // exception
	}
	else if (lows.size() > 1){ // sequence is to short
		if (debug) cout << "[DEBUG] Sequence is too short" << endl;
		return -2; // exception
	}
	else if (lows.size() == 1) // we found least 17 bits of seed!!
		low = lows[0];

	//if (debug)
	//	cout << "low = " << low << endl;

	JavaLCGMimic g2(0); // generator to bruteforce high bits of seed

	// we search for high value which is look like[s0 + limit*j], where j is some integer (because 17 lower bits are shifted right to 17)
	// we count from 2 ^ 31 down to 0, it gives us some boost
	long long maxj = (static_cast<long long>(pow(2, 31)) - static_cast<long long>(s0)) / static_cast<long long>(limit); //2**31 = 2 147 483 648
	high = static_cast<long long>(s0) + limit*maxj;

	// loop to recover high bits of seed
	// complexity is O(2 ^ (31 - i)) where i is log2(limit)

	while (high > 0){
		g2.forceSeed((high << 17) + low); //next seed to try

		bool flag = true;
		for (auto e : seq){ //check if x produces sequence
			if (g2.nextInt(limit) != e){
				flag = false;
				break; // no
			}
		}
		if (flag)
			break; // yes, we found high, break while

		high -= static_cast<long long>(limit);
	}
	if (high <= 0){ // not found valid high value
		cout << "[DEBUG] Not found valid HIGH part" << endl;
		return -3;
	}
	long long seed = (high << 17) + low; // seed value of generator after it outputs s0

	if (debug) cout
		//<< "high = " << high << endl 
		<< "[DEBUG] seed = " << seed << endl
		<< "[DEBUG] bruteEvenLimitLCG finished in " << (clock() - startt) << " msec" << endl;

	// wind generator to the end of sequence
	g2.forceSeed(seed);
	long long last_seed;
	windGenerator(g2, 0, last_seed, limit, seq.size(), debug);

	return last_seed;

}
//
// Brute GNU Class Path, initialized by milliseconds since epoch
//
void thr_thr_brute_st_adv(found *sl, SafeLong *sl2, periods prd, vector<int> seq, Options opt){
	//sl contains found flag for all threads
	//sl2 contains found flag for group of threads that brute seed for one of check_seq[index][0]
	//also sl2 contains high value for group of threads for one check_seq[index][0]

	bool debug = opt.opt_d;
	bool norm = opt.opt_norm;
	int limit = opt.opt_l;
	int cycles_per_thr = opt.opt_c;
	int s0 = seq[0];
	
	JavaLCGMimic g1; // LCG generator

	long long high, high_min;
	if (norm){ //start chek from the middle
		minmaxval mmv = sl2->getValue2(1);
		high = mmv.max;
		high_min = mmv.min;
	}
	else {
		high = sl2->getValue(1);
		high_min = sl2->getValue_min();
	}

	if (debug) cout << "[DEBUG] ** Thread #" << setw(6) << this_thread::get_id() << " started, high=" << high << ", high_min=" << high_min << endl;//DEBUG

	while (high_min >= 0){

		if (opt.opt_ds){ //Depth search strategy
			if (debug)
				cout << "[DEBUG] Thread #" << setw(6) << this_thread::get_id() << ". Start depth strategy. p=" << prd.p << ", d=" << prd.d << endl;

			while (high >= high_min && !sl->found){

				for (int l = 0; l < prd.d && !sl->found; l++){
					int low = l;

					long long x;
					if (opt.opt_upt) x =((static_cast<long long>(high << 17) + static_cast<long long>(low)) ^ g1.multiplier) & g1.mask;
					else x = static_cast<long long>(high << 17) + static_cast<long long>(low);
					
					g1.forceSeed(x);


					g1.nextInt(limit);
					long long Adh = ((g1.multiplier*prd.d) >> 17) % 2147483648;//2 147 483 648 = 2^31
					long long ii = 0; //index for JJ array

					vector<long long> JJ;
					JJ.push_back((2147483648 - (g1.seed >> 17)) / Adh);
					//if (index == 0) cout << "Adh = " << Adh << ", (seed)g1.seed >> 17 = ("<<g1.seed<<")" << (g1.seed >> 17)<<endl; //DEBUG
					long long Xh = ((g1.seed >> 17) + Adh*(JJ[0] + 1)) % 2147483648;

					long long S = JJ[0];
					// Fill JJ array
					while (S <= (131072 / prd.d)){ //131 072 = 2^17
						long long Ji = ((2147483648 - Xh) / Adh);
						S += Ji;
						JJ.push_back(JJ.back() + Ji + 1);
						Xh = (Xh + Adh*(Ji + 1)) % 2147483648;
					}
					//if (index == 0)
					//	cout << "high = "<<high<< ", JJ = " << JJ << endl; //DEBUG


					while (low <= 131072 && !sl->found){ //2^17
						long long x = (high << 17) + static_cast<long long>(low);
						g1.forceSeed(x);
						int r = g1.nextInt(limit);
						int delta = (r - s0 + limit) % limit;

						if (delta != 0){
							int skip;
							if ((low / prd.d + delta) >= JJ[ii]){
								skip = JJ[ii] + 1 - low / prd.d;
								ii++;
							}
							else
								skip = delta;

							low += prd.d * skip;
						}
						else {//check if we on right element
							bool flag = true;
							for (int j = 1; j < seq.size(); j++){ //check if x produces sequence
								if (seq[j] != g1.nextInt(limit)){
									flag = false;
									break;
								}
							}
							if (flag){ //found seed 
								sl->found = true;
								sl->seed = x;
								sl->index = 0;
								if (debug) cout << "[DEBUG] Thread #" << setw(6) << this_thread::get_id()
									<< " found seed=" << sl->seed << "!" << endl;//DEBUG
								break;
							}

							low += prd.d;
						}
					}
				}
				high -= 1;
			}
		}
		else { //Breadth search strategy
			if (debug)
				cout << "[DEBUG] Thread #" << setw(6) << this_thread::get_id() << ". Start breadth strategy. p=" << prd.p << ", d=" << prd.d << endl;

			vector<int> lows_;
			vector<index_elem> indexes_;
			for (int i = 0; i < prd.d; i++){
				index_elem e;
				lows_.push_back(i);
				e.index = i;
				indexes_.push_back(e);
			}

			while (high >= high_min && !sl->found){
				vector<int> lows(lows_);
				vector<index_elem> indexes(indexes_);

				//if (high % 10000 == 1)
				//	cout << "[DEBUG] Thread #" << setw(6) << this_thread::get_id() << ". min seed = " << (high << 17) << endl;


				double J0 = (pow(2, 31) - high) / (((g1.multiplier*prd.d) >> 17) % static_cast<long long>(pow(2, 31)));
				long long ii = 0;

				unsigned int deleted_cnt;
				do {
					deleted_cnt = 0;
					for (auto li = indexes.begin(); li != indexes.end() && !sl->found; li++){
						if (li->deleted){
							deleted_cnt++;
						}
						else {
							int low = lows[li->index];

							if (low > 131072){ //2^17
								li->deleted = true;
							}
							else {
								long long x = (high << 17) + low;
								// Only DS is used for -upt, so we don't need to check opt_upt here
								//if (opt.opt_upt > 0) g1.forceSeed((x ^ A) & M);
								//else g1.forceSeed(x);
								g1.forceSeed(x);

								int r = g1.nextInt(limit);
								int delta = (r - s0 + limit) % limit;

								if (delta != 0){
									int skip;
									if ((low / prd.d + delta) > (J0 + ii*prd.period)){
										skip = max(1, delta - prd.p - 1);
										ii++;
									}
									else
										skip = delta;

									lows[li->index] += prd.d * skip;
								}
								else{ //check if we on right element
									bool flag = true;
									for (int j = 1; j < seq.size(); j++){ //check if x produces sequence
										if (seq[j] != g1.nextInt(limit)){
											flag = false;
											break;
										}
									}
									if (flag){ //found seed 
										sl->found = true;
										sl->seed = x;
										sl->index = 0;
										if (debug) cout << "[DEBUG] Thread #" << setw(6) << this_thread::get_id()
											<< " found seed=" << sl->seed << "!" << endl;//DEBUG
										break;
									}

									lows[li->index] += prd.d;
								}
							}
						}
					}
				} while (indexes.size() > deleted_cnt && !sl->found);

				high -= 1;
			}
		}// breadth search strategy

		if (sl->found) break;
		if (norm){ //start chek from the middle
			minmaxval mmv = sl2->getValue2(1);
			high = mmv.max;
			high_min = mmv.min;
		}
		else {
			high = sl2->getValue(1);
			high_min = sl2->getValue_min();
		}
	}
}

long long brute_st_adv(Options &opt, vector<int> &seq){
	clock_t startt = clock(); //start time
	
	JavaLCGMimic g1; // LCG generator
	
	bool debug = opt.opt_d;
	int threads_num = opt.opt_t; //number of threads 

	long long high_min, high, sax;
	
	//It's safe to use current time for initialization, because we know that LCG was initialized somewhere in the past between now and 10 years ago
	if (opt.opt_sax < 0){
		sax = 1000 * time(0) + 999;
		high = sax >> 17; //now
	}
	else
		high = opt.opt_sax >> 17;

	if (opt.opt_sin < 0){
		//high_min = (sax - 315360000000L/*10 years*/) >> 17; //10 years ago
		//high_min = (sax - 157680000000L/*5 years*/) >> 17; //5 years ago
		high_min = (sax - 94608000000L/*3 years*/) >> 17; //3 years ago
		//high_min = (sax - 63072000000L/*2 years*/) >> 17; //2 years ago
	}
	else
		high_min = opt.opt_sin >> 17;
	
	if (debug)
		cout << "[DEBUG] MAIN Thread #" << setw(6) << this_thread::get_id() << " high=" << high << ", high_min=" << high_min << endl;


	periods prd;

	prd.d = computePeriod(opt.opt_l, g1.multiplier);
	prd.p = static_cast<int>(static_cast<long long>(pow(2, 31)) % static_cast<long long>(opt.opt_l));
	prd.period = pow(2, 31) / (((g1.multiplier * prd.d) >> 17) % static_cast<long>(pow(2, 31)));

	if (opt.opt_l > prd.period){
		cout << "[+] Fall to simple brute :(" << endl; //DEBUG
		return brute_st(opt, seq);
	}

	cout << "[+] d=" << prd.d << " period=" << prd.period << " Starting advansed brute" << endl; //DEBUG


	found sl;
	SafeLong sl2(high, high_min, opt.opt_c);
	
	thread *threads = new thread[threads_num];
	for (int i = 0; i < threads_num; i++){
		threads[i] = thread(thr_thr_brute_st_adv, &sl, &sl2, prd, seq, opt);
		this_thread::sleep_for(chrono::milliseconds(1)); //sleep 1 ms
	}
	for (int i = 0; i < threads_num; i++){ //wait
		threads[i].join();
	}
	delete[] threads;

	if (debug)
		cout << endl << "[DEBUG] brute_st finished in " << (clock() - startt) << " msec" << endl;

	if (!sl.found){
		if (debug) cout << "[DEBUG] Not found seed" << endl;
		return -1; //no seed found
	}

	if (debug) cout << "[DEBUG] seed = " << sl.seed << endl;

	// wind generator to the end of sequence
	g1.forceSeed(sl.seed);
	long long last_seed;
	windGenerator(g1, sl.index, last_seed, opt.opt_l, seq.size(), opt.opt_d);

	return last_seed;
}

void thr_thr_brute_st(found *sl, SafeLong *sl2, vector<int> seq, Options opt){
	//sl contains found flag for all threads
	//sl2 contains found flag for group of threads that brute seed for one of check_seq[index][0]
	//also sl2 contains high value for group of threads for one check_seq[index][0]

	bool debug = opt.opt_d;
	bool norm = opt.opt_norm;
	int limit = opt.opt_l;
	int cycles_per_thr = opt.opt_c;

	JavaLCGMimic g1; // LCG generator

	long long high, high_min;
	if (norm){ //start chek from the middle
		minmaxval mmv = sl2->getValue2(1);
		high = mmv.max;
		high_min = mmv.min;
	}
	else {
		high = sl2->getValue(1);
		high_min = sl2->getValue_min();
	}

	if (debug) cout << "[DEBUG] ** Thread #" << setw(6) << this_thread::get_id() << " started, high=" << high << ", high_min=" << high_min << endl;//DEBUG

	while (high >= high_min && !sl->found && high_min >= 0){
		for (int i = 0; i < cycles_per_thr && !sl->found && high >= high_min; i++){
			//cout << "[DEBUG] Thread #" << setw(6) << this_thread::get_id() << " started, s0=" << check_seq[index][0] 
			//	<< ", i=" << i << ", high="<<high<<endl; //DEBUG
			long long low = 0; // 17 low bits of seed
			while ((low < 131072) && !sl->found){ //2^17=131072  //cycle through low bits
				
				// try next seed
				long long x;
				if (opt.opt_upt > 0) x = (((high << 17) + low) ^ g1.multiplier) & g1.mask;
				else x = (high << 17) + low;
				
				g1.forceSeed(x);

				bool flag = true;
				for (auto j = seq.begin(); j != seq.end(); j++){ //check if x produces sequence
					if (*j != g1.nextInt(limit)){
						flag = false;
						break;
					}
				}
				if (flag){ //found seed 
					sl->found = true;
					sl->seed = x;
					sl->index = 0;
					if (debug) cout << "[DEBUG] Thread #" << setw(6) << this_thread::get_id()
						<< " found seed=" << sl->seed << "!" << endl;//DEBUG
					break;
				}
				low++;
			}
			high -= 1;
		}
		//if (debug) cout << "[DEBUG]\tThread #" << setw(6) << this_thread::get_id() << " done " << cycles_per_thr
		//	<< " cycles for s0=" << check_seq[index][0] << ". last seed=" << ((high << 17) + 131072 - 1) << ", found=" << sl.found << endl;//DEBUG

		if (sl->found) break;
		if (norm){ //start chek from the middle
			minmaxval mmv = sl2->getValue2(1);
			high = mmv.max;
			high_min = mmv.min;
		}
		else {
			high = sl2->getValue(1);
			high_min = sl2->getValue_min();
		}
	}
}

long long brute_st(Options &opt, vector<int> &seq){
	clock_t startt = clock(); //start time

	JavaLCGMimic g1; // LCG generator

	bool debug = opt.opt_d;
	int threads_num = opt.opt_t; //number of threads 

	long long high_min, high, sax;

	//It's safe to use current time for initialization, because we know that LCG was initialized somewhere in the past between now and 10 years ago
	if (opt.opt_sax < 0){
		sax = 1000 * time(0) + 999;
		high = sax >> 17; //now
	}
	else
		high = opt.opt_sax >> 17;

	if (opt.opt_sin < 0){
		//high_min = (sax - 315360000000L/*10 years*/) >> 17; //10 years ago
		//high_min = (sax - 157680000000L/*5 years*/) >> 17; //5 years ago
		high_min = (sax - 94608000000L/*3 years*/) >> 17; //3 years ago
		//high_min = (sax - 63072000000L/*2 years*/) >> 17; //2 years ago
	}
	else
		high_min = opt.opt_sin >> 17;

	if (debug)
		cout << "[DEBUG] Thread #" << setw(6) << this_thread::get_id() << " high=" << high << ", high_min=" << high_min << endl;


	found sl;
	SafeLong sl2(high, high_min, opt.opt_c);

	thread *threads = new thread[threads_num];
	for (int i = 0; i < threads_num; i++){
		threads[i] = thread(thr_thr_brute_st, &sl, &sl2, seq, opt);
		this_thread::sleep_for(chrono::milliseconds(1)); //sleep 1 ms
	}
	for (int i = 0; i < threads_num; i++){ //wait
		threads[i].join();
	}
	delete[] threads;

	if (debug)
		cout << endl << "[DEBUG] brute_st finished in " << (clock() - startt) << " msec" << endl;

	if (!sl.found){
		if (debug) cout << "[DEBUG] Not found seed" << endl;
		return -1; //no seed found
	}

	if (debug) cout << "[DEBUG] seed = " << sl.seed << endl;

	// wind generator to the end of sequence
	g1.forceSeed(sl.seed);
	long long last_seed;
	windGenerator(g1, sl.index, last_seed, opt.opt_l, seq.size(), opt.opt_d);

	return last_seed;
}


void thr_thr_brute_dumb(found *sl, SafeLong *sl2, vector<int> seq, Options opt, mutex *m, long long *s){
	//sl contains found flag for all threads
	//sl2 contains found flag for group of threads that brute seed for one of check_seq[index][0]
	//also sl2 contains high value for group of threads for one check_seq[index][0]

	bool debug = opt.opt_d;
	bool norm = opt.opt_norm;
	int limit = opt.opt_l;
	int cycles_per_thr = opt.opt_c;
	long long cycles_thr_done = 0;

	JavaLCGMimic g1; // LCG generator

	long long state, state_min;
	if (norm){ //start chek from the middle
		minmaxval mmv = sl2->getValue2(1);
		state = mmv.max;
		state_min = mmv.min;
	}
	else {
		state = sl2->getValue(1);
		state_min = sl2->getValue_min();
	}

	if (debug) cout << "[DEBUG] ** Thread #" << setw(6) << this_thread::get_id() << " started, seed_max=" << state << ", seed_min=" << state_min << endl;//DEBUG

	while (state >= state_min && !sl->found && state_min >= 0){
		for (int i = 0; i < cycles_per_thr && !sl->found && state >= state_min; i++){
			cycles_thr_done++;

			g1.forceSeed(state);

			bool flag = true;
			for (auto j = seq.begin(); j != seq.end(); j++){ //check if high produces sequence
				if (*j != g1.nextInt(limit)){
					flag = false;
					break;
				}
			}
			if (flag){ //found seed 
				sl->found = true;
				sl->seed = state;
				sl->index = 0;
				if (debug) cout << "[DEBUG] Thread #" << setw(6) << this_thread::get_id()
					<< " found seed=" << sl->seed << "!" << endl;//DEBUG
				break;
			}
			state -= 1;
		}
		
		if (sl->found) break;
		if (norm){ //start chek from the middle
			minmaxval mmv = sl2->getValue2(1);
			state = mmv.max;
			state_min = mmv.min;
		}
		else {
			state = sl2->getValue(1);
			state_min = sl2->getValue_min();
		}
	}
	if (debug) cout << "[DEBUG] ** Thread #" << setw(6) << this_thread::get_id() << " finished, " << cycles_thr_done << " seeds checked" << endl;//DEBUG
	m->lock();
	*s += cycles_thr_done;
	m->unlock();
}

void brute_dumb(Options &opt, vector<int> &seq){
	chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now(); //start time
	chrono::high_resolution_clock::time_point t2 = t1;

	long long seeds; //seeds checked for all threads
	mutex mutex_seeds; //mutex for seeds var

	JavaLCGMimic g1; // LCG generator

	bool debug = opt.opt_d;
	int threads_num = opt.opt_t; //number of threads 

	long long state_min, state;

	if (opt.opt_sin < 0) state_min = 0;
	else state_min = opt.opt_sin;
	if (opt.opt_sax < 0) state = static_cast<long long>(pow(2,48)) - 1L;
	else state = opt.opt_sax;

	found sl;
	SafeLong sl2(state, state_min, opt.opt_c);

	thread *threads = new thread[threads_num];
	for (int i = 0; i < threads_num; i++){
		threads[i] = thread(thr_thr_brute_dumb, &sl, &sl2, seq, opt, &mutex_seeds, &seeds);
		this_thread::sleep_for(chrono::milliseconds(1)); //sleep 1 ms
	}
	
	if (opt.opt_tst2 > 0.0) while (true && !sl.found){ //calculate time intrval
		this_thread::sleep_for(chrono::seconds(1));

		t2 = chrono::high_resolution_clock::now();
		if (chrono::duration_cast<chrono::duration<double>>(t2 - t1).count() > opt.opt_tst2) {
			sl.found = true;
			sl.seed = -2;
			break;
		}
	}

	for (int i = 0; i < threads_num; i++){ //wait
		threads[i].join();
	}
	delete[] threads;

	if (sl.seed == -2) cout << "[-] Not found seed" << endl;
	else cout << "[+] Found seed: " << sl.seed << endl;
	cout << "[+] Done " << seeds << " seeds in " << fixed << chrono::duration_cast<chrono::duration<double>>(t2 - t1).count() << " seconds" << endl;
}



long long brutenextIntwoLimit(Options &opt, vector<int> &av){
	//
	// If nextInt() is called the highest 32 bits goes directly to output - i.e. S0 == high 32 bit of seed
	//
	
	clock_t startt = clock(); //start time
	JavaLCGMimic g1; // LCG generator
	bool debug = opt.opt_d;
	found sl;

	if (debug) cout << "[DEBUG] brutenextIntwoLimit started" << endl;//DEBUG

	long long high = av[0];

	for (long long i = 65535; i >= 0; i--){
		long long x = (high << 16) + i;
		g1.forceSeed(x);

		bool flag = true;
		for (unsigned int j = 1; j < av.size(); j++){ //check if x produces sequence
			if (av[j] != g1.nextInt()){
				flag = false;
				break;
			}
		}
		if (flag){ //found seed 
			sl.found = true;
			sl.seed = x;
			sl.index = 1;
			if (debug) cout << "[DEBUG] found seed=" << sl.seed << "!" << endl;//DEBUG
			break;
		}
	}
	
	if (opt.opt_d)
		cout << endl << "[DEBUG] brutenextIntwoLimit finished in " << (clock() - startt) << " msec" << endl;

	if (!sl.found){
		if (opt.opt_d) cout << "[DEBUG] Not found seed" << endl;
		return -1; //no seed found
	}

	if (debug) cout << "[DEBUG] seed = " << sl.seed << endl;

	// wind generator to the end of sequence
	g1.forceSeed(sl.seed);
	long long last_seed;
	windGenerator(g1, sl.index, last_seed, opt.opt_l, av.size(), opt.opt_d);

	return last_seed;

}

long long brutenextLong(Options &opt, vector<long long> &av){
	clock_t startt = clock(); //start time
	JavaLCGMimic g1; // LCG generator
	bool debug = opt.opt_d;
	found sl;

	if (debug) cout << "[DEBUG] brutenextLong started" << endl;//DEBUG

	//long long high = av[0] >> 32;
	long long high = av[0] & 0xFFFFFFFFL;

	for (long long i = 65535; i >= 0; i--){
		long long x = (high << 16) + i;
		g1.forceSeed(x);
		bool flag = true;
		for (unsigned int j = 1; j < av.size(); j++){ //check if x produces sequence
			if (av[j] != g1.nextLong()){
				flag = false;
				break;
			}
		}
		if (flag){ //found seed 
			sl.found = true;
			sl.seed = x;
			sl.index = 1;
			if (debug) cout << "[DEBUG] found seed=" << sl.seed << "!" << endl;//DEBUG
			break;
		}
	}

	if (opt.opt_d)
		cout << endl << "[DEBUG] brutenextIntwoLimit finished in " << (clock() - startt) << " msec" << endl;

	if (!sl.found){
		if (opt.opt_d) cout << "[DEBUG] Not found seed" << endl;
		return -1; //no seed found
	}

	if (debug) cout << "[DEBUG] seed = " << sl.seed << endl;

	// wind generator to the end of sequence
	g1.forceSeed(sl.seed);
	long long last_seed;
	windGenerator(g1, sl.index, last_seed, opt.opt_l, av.size(), opt.opt_d);

	return last_seed;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
long long brute(Options &opt, vector<int> &av){
	long long seed;
	JavaLCGMimic g;

	if (opt.opt_l == 0){ //nextInt()
		if (opt.opt_d) cout << "[DEBUG] Go to brutenextIntwoLimit (" << opt.opt_l<<")" << endl;
		seed = brutenextIntwoLimit(opt, av);
	}
	else if ((opt.opt_l & -opt.opt_l) == opt.opt_l){ //limit is power of 2
		if (opt.opt_d) cout << "[DEBUG] Go to brutePowerOf2LimitLCG (" << opt.opt_l << ")" << endl;
		seed = brutePowerOf2LimitLCG(opt, av); 
	}
	else if (opt.opt_l % 2 == 0){ //limit is even
		if (opt.opt_d) cout << "[DEBUG] Go to bruteEvenLimitLCG (" << opt.opt_l << ")" << endl;
		seed = bruteEvenLimitLCG(opt.opt_l, av, opt.opt_d);
	}
	else if (opt.opt_bs || opt.opt_ds){ //advanced brute
		if (opt.opt_st){ //initialized by time
			if (opt.opt_d) cout << "[DEBUG] Go to brute_st_adv (" << opt.opt_l << ")" << endl;
			seed = brute_st_adv(opt, av);
		}
		else{
			if (opt.opt_d) cout << "[DEBUG] Go to bruteOddLimitLCG_adv (" << opt.opt_l << ")" << endl;
			seed = bruteOddLimitLCG_adv(opt, av);
		}
	}
	else if (opt.opt_st){ //initialized by time, but not advanced
		if (opt.opt_d) cout << "[DEBUG] Go to brute_st (" << opt.opt_l << ")" << endl;
		seed = brute_st(opt, av);
	}
	else if (opt.opt_upt > 0){ //server uptime is known
		opt.opt_sin = g.seedUniquifier + 1L + (opt.opt_upt - 10L) * 1000000;
		opt.opt_sax = g.seedUniquifier + static_cast<long long>(opt.opt_su) + (opt.opt_upt + 10L) * 1000000;
		opt.opt_ds = true; //use only depth search strategy

		if (opt.opt_d) cout << "[DEBUG] Go to brute_st_adv (limit=" << opt.opt_l << ",uptime="<<opt.opt_upt<<",max seedUniquifier="<<opt.opt_su
			<<",-sin "<<opt.opt_sin<<",-sax "<<opt.opt_sax<<")" << endl;
		
		seed = brute_st_adv(opt, av);
	}
	else {//limit is odd
		if (opt.opt_d) cout << "[DEBUG] Go to bruteOddLimitLCG (" << opt.opt_l << ")" << endl;
		seed = bruteOddLimitLCG(opt, av);
	}
	return seed;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*void testLCG(long limit, long long  seed, int count){
	JavaLCGMimic g1(seed);

	cout << "1. Now test JavaLCGMimic work. Seed values is '" << seed << "'. Compare it with java.util.Random output." << endl;
	for (int i = 0; i < count; ++i){
		cout << g1.nextInt(limit) << " ";
	}
	cout << endl;

	if (limit % 2 != 0){
		cout << "Limit is not divisble by 2. Bye." << endl;
		return;
	}

	if ((limit & -limit) == limit){
		cout << "Limit is power of 2. Bye." << endl;
		return;
	}

	JavaLCGSub g2(seed, limit);
	JavaLCGMimic g3(seed);

	cout << "2. Now test JavaLCGSub work. Limit values is '" << limit << "'. Compare output of JavaLCGSub with " << maxPowerOf2(limit) << " LSB of JavaLCGMimic output." << endl;
	cout << setw(13) << "JavaLCGMimic" << " versus " << setw(10) << "JavaLCGSub" << endl;

	for (int i = 0; i < count; i++){
		cout << setw(13) << (bitset<8>) g3.nextInt(limit) << " versus " << setw(10) << left << (bitset<4>) g2.nextInt() << right << endl;
		//cout << g3.nextInt(limit) << " versus " << g2.nextInt() << endl;
	}
}*/

