#pragma once
#include <cstdlib>
#include <vector>
#include <cstring>
#include <iostream>
#include <iomanip>

using namespace std;

class Options
{
public:
	Options(int argc, char *argv[]);
	~Options();
	void printHelp();


	char *opt_i = 0;				//IP
	char *opt_p = 0;				//server port
	char *opt_c = 0;				//cookie
	unsigned char opt_c_bin[16];	//cookie in bytes
	long long opt_t = 0;			//time in milliseconds (PRNGMillisEstimate)
	long long opt_s = 0;			//time in milliseconds (timeSecs)
	int opt_r = 128;				//number of threads
	int opt_e = 300;				//seconds to start service since server starts up
	int opt_n = 1000;				//clients before attack
	int opt_m = 1000;				//expected time packets fly from server to client
	bool opt_h = false;				//help
};

