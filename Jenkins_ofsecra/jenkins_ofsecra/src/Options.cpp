#include "Options.h"
#include "utils.h"


Options::Options(int argc, char *argv[])
{
	for (int i = 0; i < argc; ){
		char **strend = 0; //temp foe strtoll and other sexy staff
		if (0 == strncmp(argv[i], "-i", strlen("-i"))){ 
			if ( i + 1 < argc && argv[i + 1] != 0 && argv[i+1][0] != '-'){
				this->opt_i = argv[i + 1];
				i++;
				//cout << "[DEBUG] opt_i = " << this->opt_i << endl;//DEBUG
			}
		}
		if (0 == strncmp(argv[i], "-c", strlen("-c"))){
			if (i + 1 < argc && argv[i + 1] != 0 && argv[i + 1][0] != '-'){
				this->opt_c = argv[i + 1];
				char2byte(this->opt_c, this->opt_c_bin, 16);
				i++;
				//cout << "[DEBUG] opt_c = " << this->opt_c << endl;//DEBUG
				//cout << "[DEBUG] opt_c_bin = " << this->opt_c_bin << endl;//DEBUG
			}
		}
		if (0 == strncmp(argv[i], "-p", strlen("-p"))){ 
			if (i + 1 < argc && argv[i + 1] != 0 && argv[i + 1][0] != '-'){
				this->opt_p = argv[i + 1];
				i++;
				//cout << "[DEBUG] opt_p = " << this->opt_p << endl;//DEBUG
			}
		}
		if (0 == strncmp(argv[i], "-e", strlen("-e"))){
			if (i + 1 < argc && argv[i + 1] != 0 && argv[i + 1][0] != '-'){
				this->opt_e = atoi(argv[i + 1]);
				i++;
				//cout << "[DEBUG] opt_e = " << this->opt_e << endl;//DEBUG
			}
		}
		if (0 == strncmp(argv[i], "-n", strlen("-n"))){
			if (i + 1 < argc && argv[i + 1] != 0 && argv[i + 1][0] != '-'){
				this->opt_n = atoi(argv[i + 1]);
				i++;
				//cout << "[DEBUG] opt_n = " << this->opt_n << endl;//DEBUG
			}
		}
		if (0 == strncmp(argv[i], "-m", strlen("-m"))){
			if (i + 1 < argc && argv[i + 1] != 0 && argv[i + 1][0] != '-'){
				this->opt_m = atoi(argv[i + 1]);
				i++;
				//cout << "[DEBUG] opt_m = " << this->opt_m << endl;//DEBUG
			}
		}
		if (0 == strncmp(argv[i], "-t", strlen("-t")) ){ 
			if (i + 1 < argc && argv[i + 1] != 0 && argv[i + 1][0] != '-'){
				this->opt_t = strtoll(argv[i + 1], strend, 10);
				i++;
				//cout << "[DEBUG] opt_t = " << this->opt_t << endl;//DEBUG
			}
		}
		if (0 == strncmp(argv[i], "-s", strlen("-s"))){
			if (i + 1 < argc && argv[i + 1] != 0 && argv[i + 1][0] != '-'){
				this->opt_s = strtoll(argv[i + 1], strend, 10);
				i++;
				//cout << "[DEBUG] opt_s = " << this->opt_s << endl;//DEBUG
			}
		}
		if (0 == strncmp(argv[i], "-r", strlen("-r")) ){ 
			if (i + 1 < argc && argv[i + 1] != 0 && argv[i + 1][0] != '-'){
				this->opt_r = atoi(argv[i + 1]);
				i++;
				//cout << "[DEBUG] opt_r = " << this->opt_r << endl;//DEBUG
			}
		}
		if (0 == strncmp(argv[i], "-h", strlen("-h"))){ //print help
			this->printHelp();
		}
		i++;
	}
}

void Options::printHelp(){
	const int indent = 20;
	cout << "Options: " << endl;
	cout << setw(indent) << "-i <str>" << " - IP address in dotted format" << endl;
	cout << setw(indent) << "-c <str>" << " - cookie (MD5 digest in lowcase hex)" << endl;
	cout << setw(indent) << "-p <num>" << " - port number" << endl;
	cout << setw(indent) << "-t <num>" << " - time estimate in millliseconds (PRNGMillisEstimate)" << endl;
	cout << setw(indent) << "-s <num>" << " - time estimate in millliseconds (timeMillisEstimate)" << endl;
	cout << setw(indent) << "-r <num>" << " - number of threades (128 - default)" << endl;
	cout << setw(indent) << "-e <num>" << " - seconds to start service after server has started up (300 - default)" << endl;
	cout << setw(indent) << "-n <num>" << " - clients got cookie before attack (1000 - default) (PRNGseekMax)" << endl;
	cout << setw(indent) << "-m <num>" << " - expected time packet reach client from server (1000 msec - default)" << endl;
	cout << setw(indent) << "-h" << " - print this help" << endl;
	this->opt_h = true;
}

Options::~Options()
{
}
