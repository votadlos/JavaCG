//
// Command line options
//
#pragma once
#include <cstdlib>
#include <vector>
#include <cstring>
#include <iostream>
#include <iomanip>
//#include <tchar.h>

using namespace std;

class Options
{
public:
	Options(int argc, char *argv[], vector<int> &limits);
	~Options();
	void printHelp();
	void showOpt();


	const int opt_tst_default = 40,
		opt_f_default = 20,
		opt_b_default = 20,
		opt_g_default = 20,
		opt_n_default = 20,
		opt_t_default = 128,
		opt_c_default = 9999,
		opt_su_default = 1000,
		opt_p_default = 15;



	int opt_g = -1;				//just generate sequence
	int opt_l = -1;				//limit
	int opt_su = opt_su_default;//max java.util.Random's seedUniquifier
	long long opt_upt = -1;		//server uptime in milliseconds (generate sin and sax mode)
	long long opt_s = -1,		//seed
		opt_sin =-1,			//seed min
		opt_sax =-1;			//seed max
	int opt_n = opt_n_default;	//number of elements in input
	bool opt_h = false;			//help
	bool opt_d = false;			//debug
	int opt_t = opt_t_default;	//number of threads
	int opt_c = opt_c_default;	//cycles_per_thread
	bool opt_norm = false;		//start brute from the middle of diapason (suppse nurmal spread)
	int opt_f = opt_f_default;	//wind generator forward and return next opt_f elements (20 - default)
	int opt_b = -1;				//wind generator backward and return previous opt_b elements (20 - default)
	int opt_tst = -1;			//self test mode
	double opt_tst2 = -1.0;		//test for plain brute (0.0 - till the end, >0.0 till the specified time period ends)
	int opt_p = 0;				//password len
	char *opt_a = 0;			//file with alfabet
	bool opt_bs = false;		//Breadth search strategy
	bool opt_ds = false;		//Depth search strategy
	char *opt_fn = 0;			//file with sequences to brute
	bool opt_st = false;		//it's known that seed was initialised by current time in milliseconds
	int opt_ms = 0;				//brute matrix of sequense size

};

