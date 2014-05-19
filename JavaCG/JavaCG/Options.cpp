#include "Options.h"


Options::Options(int argc, char *argv[], vector<int> &limits)
{
	limits.clear();
	for (int i = 0; i < argc; ){
		char **strend = 0; //temp foe strtoll and other sexy staff
		if (0 == strncmp(argv[i], "-g", strlen("-g"))){ //-g N - how many numbers generate, 20 - default
			if ( i + 1 < argc && argv[i + 1] != 0 && argv[i+1][0] != '-'){
				this->opt_g = atoi(argv[i + 1]);
				i++;
			}
			else
				this->opt_g = this->opt_g_default;
			//cout << "[DEBUG] opt_g = " << this->opt_g << endl;//DEBUG
		}
		if (0 == strncmp(argv[i], "-p", strlen("-p"))){ //-p <pw leng>
			if (i + 1 < argc && argv[i + 1] != 0 && argv[i + 1][0] != '-'){
				this->opt_p = atoi(argv[i + 1]);
				i++;
			}
			else
				this->opt_p = this->opt_p_default;
			//cout << "[DEBUG] opt_p = " << this->opt_p << endl;//DEBUG
		}
		if (0 == strncmp(argv[i], "-t", strlen("-t")) && strlen(argv[i]) == strlen("-t") ){ //-t N - number of threads, 256 - default
			if (i + 1 < argc && argv[i + 1] != 0 && argv[i + 1][0] != '-'){
				this->opt_t = atoi(argv[i + 1]);
				i++;
			}
			//cout << "[DEBUG] opt_t = " << this->opt_t << endl;//DEBUG
		}
		if (0 == strncmp(argv[i], "-a", strlen("-a"))){ //-a <file name>
			if (i + 1 < argc && argv[i + 1] != 0 && argv[i + 1][0] != '-'){
				this->opt_a = argv[i + 1];
				i++;
			}
			//cout << "[DEBUG] opt_a = " << this->opt_a << endl;//DEBUG
		}
		if (0 == strncmp(argv[i], "-fn", strlen("-fn")) && strlen(argv[i]) == strlen("-fn") ){ //-fn <file name>
			if (i + 1 < argc && argv[i + 1] != 0 && argv[i + 1][0] != '-'){
				this->opt_fn = argv[i + 1];
				i++;
			}
			//cout << "[DEBUG] opt_fn = " << this->opt_fn << endl;//DEBUG
		}
		if (0 == strncmp(argv[i], "-l", strlen("-l"))){ //-l N - sequense modulo (limit), 88 - default
			if (i + 1 < argc && argv[i + 1] != 0 && argv[i + 1][0] != '-'){
				this->opt_l = atoi(argv[i + 1]);
				limits.push_back(this->opt_l);
				i++;
			}
			//cout << "[DEBUG] opt_l = " << this->opt_l << endl;//DEBUG
		}
		if (0 == strncmp(argv[i], "-n", strlen("-n")) && strlen(argv[i]) == strlen("-n") ){ //-n N - how many numbers in input sequense, 20 -default
			if (i + 1 < argc && argv[i + 1] != 0 && argv[i + 1][0] != '-'){
				this->opt_n = atoi(argv[i + 1]);
				i++;
			}
			//cout << "[DEBUG] opt_n = " << this->opt_n << endl;//DEBUG
		}
		if (0 == strncmp(argv[i], "-f", strlen("-f")) && strlen(argv[i]) == strlen("-f") ){
			if (i + 1 < argc && argv[i + 1] != 0 && argv[i + 1][0] != '-'){
				this->opt_f = atoi(argv[i + 1]);
				i++;
			}
			else 
				this->opt_f = this->opt_f_default;
			//cout << "[DEBUG] opt_f = " << this->opt_f << endl;//DEBUG
		}
		if (0 == strncmp(argv[i], "-b", strlen("-b")) && strlen(argv[i]) == strlen("-b") ){
			if (i + 1 < argc && argv[i + 1] != 0 && argv[i + 1][0] != '-'){
				this->opt_b = atoi(argv[i + 1]);
				i++;
			}
			else
				this->opt_b = this->opt_b_default;
			//cout << "[DEBUG] opt_b = " << this->opt_b << endl;//DEBUG
		}
		if (0 == strncmp(argv[i], "-s", strlen("-s")) && strlen(argv[i]) == strlen("-s") ){ //-s N - specify seed, -1 - default, means time(0)
			if (i + 1 < argc && argv[i + 1] != 0 && argv[i + 1][0] != '-'){
				this->opt_s = strtoll(argv[i + 1], strend, 10);
				i++;
			}
			//cout << "[DEBUG] opt_s = " << this->opt_s << endl;//DEBUG
		}
		if (0 == strncmp(argv[i], "-ms", strlen("-ms")) && strlen(argv[i]) == strlen("-ms")){ //-ms N - matrix size
			if (i + 1 < argc && argv[i + 1] != 0 && argv[i + 1][0] != '-'){
				this->opt_ms = atoi(argv[i + 1]);
				i++;
			}
			//cout << "[DEBUG] opt_ms = " << this->opt_ms << endl;//DEBUG
		}
		if (0 == strncmp(argv[i], "-st", strlen("-st")) && strlen(argv[i]) == strlen("-st")){ //-st - initialized by time in milliseconds
			this->opt_st = true;
			//cout << "[DEBUG] opt_st = " << this->opt_st << endl;//DEBUG
		}
		if (0 == strncmp(argv[i], "-upt", strlen("-upt")) && strlen(argv[i]) == strlen("-upt")){ //-upt <long>
			if (i + 1 < argc && argv[i + 1] != 0 && argv[i + 1][0] != '-'){
				this->opt_upt = strtoll(argv[i + 1], strend, 10);
				i++;
			}
			//cout << "[DEBUG] opt_upt = " << this->opt_upt << endl;//DEBUG
		}
		if (0 == strncmp(argv[i], "-sin", strlen("-sin")) && strlen(argv[i]) == strlen("-sin")){ //-sin <long>
			if (i + 1 < argc && argv[i + 1] != 0 && argv[i + 1][0] != '-'){
				this->opt_sin = strtoll(argv[i + 1], strend, 10);
				i++;
			}
			//cout << "[DEBUG] opt_sin = " << this->opt_sin << endl;//DEBUG
		}
		if (0 == strncmp(argv[i], "-sax", strlen("-sax")) && strlen(argv[i]) == strlen("-sax")){ //-sax <long>
			if (i + 1 < argc && argv[i + 1] != 0 && argv[i + 1][0] != '-'){
				this->opt_sax = strtoll(argv[i + 1], strend, 10);
				i++;
			}
			//cout << "[DEBUG] opt_sax = " << this->opt_sax << endl;//DEBUG
		}
		if (0 == strncmp(argv[i], "-tst2", strlen("-tst2")) && strlen(argv[i]) == strlen("-tst2")){ //-tst2 <long>
			if (i + 1 < argc && argv[i + 1] != 0 && argv[i + 1][0] != '-'){
				this->opt_tst2 = strtod(argv[i + 1], strend);
				i++;
			}
			//cout << "[DEBUG] opt_tst2 = " << this->opt_tst2 << endl;//DEBUG
		}
		if (0 == strncmp(argv[i], "-su", strlen("-su")) && strlen(argv[i]) == strlen("-su")){ //-su N - max java.util.Random's seedUniquifier, 100 - default
			if (i + 1 < argc && argv[i + 1] != 0 && argv[i + 1][0] != '-'){
				this->opt_su = atoi(argv[i + 1]);
				i++;
			}
			//cout << "[DEBUG] opt_su = " << this->opt_su << endl;//DEBUG
		}
		if (0 == strncmp(argv[i], "-c", strlen("-c"))){ //-c N - cycles per thread, 9999 - default
			if (i + 1 < argc && argv[i + 1] != 0 && argv[i + 1][0] != '-'){
				this->opt_c = atoi(argv[i + 1]);
				i++;
			}
			//cout << "[DEBUG] opt_c = " << this->opt_c << endl;//DEBUG
		}
		if (0 == strncmp(argv[i], "-d", strlen("-d")) && strlen(argv[i]) == strlen("-d") ){ //DEBUG
			this->opt_d = true;
			//cout << "[DEBUG] opt_d = " << this->opt_d << endl;//DEBUG
		}
		if (0 == strncmp(argv[i], "-bs", strlen("-bs")) && strlen(argv[i]) == strlen("-bs") ){ //breadth search
			this->opt_bs = true;
			//cout << "[DEBUG] opt_bs = " << this->opt_bs << endl;//DEBUG
		}
		if (0 == strncmp(argv[i], "-ds", strlen("-ds")) && strlen(argv[i]) == strlen("-ds")){ //depth search
			this->opt_ds = true;
			//cout << "[DEBUG] opt_ds = " << this->opt_ds << endl;//DEBUG
		}
		if (0 == strncmp(argv[i], "-tst", strlen("-tst"))){ 
			if (i + 1 < argc && argv[i + 1] != 0 && argv[i + 1][0] != '-'){
				this->opt_tst = atoi(argv[i + 1]);
				i++;
			}
			else
				this->opt_tst = this->opt_tst_default;
			//cout << "[DEBUG] opt_tst = " << this->opt_tst << endl;//DEBUG
		}
		if (0 == strncmp(argv[i], "-norm", strlen("-norm"))){ 
			this->opt_norm = true;
			//cout << "[DEBUG] opt_norm = " << this->opt_norm << endl;//DEBUG
		}
		if (0 == strncmp(argv[i], "-h", strlen("-h"))){ //print help
			this->printHelp();
		}
		i++;
	}
	if (limits.size() == 0){
		//this->opt_l = this->opt_l_default;
		limits.push_back(this->opt_l);
	}
	//check seed min and seed max
	if (this->opt_sin != -1 && this->opt_sax != -1){
		long long high_sax = this->opt_sax >> 17;
		long long high_sin = this->opt_sin >> 17;
		if (high_sax <= high_sin){
			cout << "[+] max seed(" << this->opt_sax << ") not big enough comparing to min seed (" << this->opt_sin << ") - use min seed alone. min|sax-sin| must be greater than 2^17" << endl;
			this->opt_sax = -1;
		}
	}
}

void Options::printHelp(){
	const int indent = 20;
	cout << "Options: " << endl;
	cout << setw(indent)<<"-g <num>"<<" - generate <num> random values, default num="<<this->opt_g_default << endl;
	cout << setw(indent) << "-l <num>" << " - set a limit for random values. If multiple -l options specified, the last one works. If num=0 then nextInt() case supposed, if no -l options specified nextLong() case supposed." << endl;
	cout << setw(indent) << "-a <file_path>" << " - specify alfabet for passorrd generation" << endl;
	cout << setw(indent) << "-n <num>" << " - specify number of values in sequence to read from stdin, default num="<<this->opt_n_default<<". When used with -p option (passwords are being cracked) this option is updated with length of password specified." << endl;
	cout << setw(indent) << "-fn <file_path>" << " - path to file with sequences to brute" << endl;
	cout << setw(indent) << "-s <long>" << " - specify seed, default - (std::chrono::steady_clock::now().time_since_epoch().count()^0x5deece66dL) & 0xffffffffffffL)" << endl;
	cout << setw(indent) << "-sin <long>" << " - specify mINimum Seed for search time minimization (not used in case of even limit), " << endl;
	cout << setw(indent) << "-sax <long>" << " - specify mAXimum Seed for search time minimization (not used in case of even limit), " << endl;
	cout << setw(indent) << "-upt <long>" << " - specify server uptime in MILLIseconds used for max and min seed calculation in case of Random's default constructor (takes uptime in NANOseconds). Also can be used in sequence generation mode (-g): specified milliseconds converted to nanoseconds with random addition (C's rand() is used) and Random's seedUniquifier is chosen randomly from diapason [1, -su option]" << endl;
	cout << setw(indent) << "-su <num>" << " - specify max seedUniquifier in case of Random's default constructor, default - "<<this->opt_su_default << endl;
	cout << setw(indent) << "-p <num>" << " - specify the length of generated passwords, default num=15. Numder of generated passords is specified by -g or -b parameters" << endl;
	cout << setw(indent) << "-f <num>" << " - wind generator forward and return <num> next elements, default num="<<this->opt_f_default << endl;
	cout << setw(indent) << "-b <num>" << " - wind generator backward and return <num> previous elements, default num="<<this->opt_b_default << endl;
	cout << setw(indent) << "-bs" << " - Breadth search strategy" << endl;
	cout << setw(indent) << "-ds" << " - Depth search strategy" << endl;
	cout << setw(indent) << "-st" << " - Known that initial seed is time in milliseconds (Used in GNU Class Path)" << endl;
	cout << setw(indent) << "-tst <num>" << " - run self test <num> times, if omitted num=40. Multiple -l <num> options allowed in this case, test will be performed for all of them" << endl;
	cout << setw(indent) << "-tst2 <num>" << " - start plain brute for <num> seconds and return how many seeds it has done. If num=0.0 will continue till the state will be found" << endl;
	cout << setw(indent) << "-t <num>" << " - number of threads, default - " << this->opt_t_default << endl;
	cout << setw(indent) << "-c <num>" << " - cycles per thread, default - " << this->opt_c_default << endl;
	cout << setw(indent) << "-ms <num>" << " - specify the size of the matrix made of sequense for parallel brute of number of sequences stating from different s0" << endl;
	cout << setw(indent) << "-norm" << " - start brute from the middle of diapason (suppose normal spread of HIGH part)" << endl;
	cout << setw(indent) << "-d" << " - debug output" << endl;
	cout << setw(indent) << "-h" << " - print this help" << endl;
	this->opt_h = true;
}

void Options::showOpt(){
	const int indent = 20;
	cout << "Options: " << endl;
	cout << setw(indent) << "-g = " << this->opt_g << endl;
	cout << setw(indent) << "-l = " << this->opt_l << endl;
	cout << setw(indent) << "-a = " << (this->opt_a == 0 ? "NULL" : this->opt_a)  << endl;
	cout << setw(indent) << "-n = " << this->opt_n << endl;
	cout << setw(indent) << "-fn = " << (this->opt_fn == 0 ? "NULL" : this->opt_fn) << endl;
	cout << setw(indent) << "-s = " << this->opt_s << endl;
	cout << setw(indent) << "-sin = " << this->opt_sin << endl;
	cout << setw(indent) << "-sax = " << this->opt_sax << endl;
	cout << setw(indent) << "-upt = " << this->opt_upt << endl;
	cout << setw(indent) << "-su = " << this->opt_su << endl;
	cout << setw(indent) << "-p = " << this->opt_p << endl;
	cout << setw(indent) << "-f = " << this->opt_f << endl;
	cout << setw(indent) << "-b = " << this->opt_b << endl;
	cout << setw(indent) << "-bs = " << this->opt_bs << endl;
	cout << setw(indent) << "-ds = " << this->opt_ds << endl;
	cout << setw(indent) << "-st = " << this->opt_st << endl;
	cout << setw(indent) << "-tst = " << this->opt_tst << endl;
	cout << setw(indent) << "-tst2 = " << this->opt_tst2 << endl;
	cout << setw(indent) << "-t = " << this->opt_t << endl;
	cout << setw(indent) << "-c = " << this->opt_c << endl;
	cout << setw(indent) << "-ms = " << this->opt_ms << endl;
	cout << setw(indent) << "-norm = " << this->opt_norm << endl;
	cout << setw(indent) << "-d = " << this-> opt_d << endl;
}

Options::~Options()
{
}
