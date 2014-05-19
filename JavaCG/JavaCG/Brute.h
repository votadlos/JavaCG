//
// Actual brute logic
//

#ifndef _Brute_H
#define _Brute_H

#include <exception>
#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <bitset>
#include <iomanip>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <chrono>
#include "JavaLCGSub.h"
#include "JavaLCGMimic.h"
#include "SafeLong.h"
#include "Options.h"
#include "Utils.h"

using namespace std;

//wind generator forward and backward
void windGenerator(JavaLCGMimic &g, int index, long long &last_seed, int limit, int N, bool debug);
//make matrix from sequence. Each matrix row is used by brute thread as input
void malloc_check_matrix(int **&check_seq, Options &opt, int &N, vector<int> &seq);
//free matrix
void free_check_matrix(int **check_seq, int opt_ms);
//used in debug output
void show_check_matrix(int **check_seq, int r, int N);


//
// Functions to brure odd limit.
// thr_bruteOddLimitLCG - threads for matrix rows
// thr_thr_bruteOddLimitLCG - threads for high part of seed
//
void thr_thr_bruteOddLimitLCG(int index, found *sl, SafeLong *sl2, int **check_seq, int N, Options opt);
void thr_bruteOddLimitLCG(int index, found *sl, int **check_seq, int N, Options opt);
long long bruteOddLimitLCG(Options &opt, vector<int> &seq);

//
// Functions for advanced brute of odd limits
//
void thr_thr_bruteOddLimitLCG_adv(int index, found *sl, SafeLong *sl2, periods prd, int **check_seq, int N, Options opt);
void thr_bruteOddLimitLCG_adv(int index, found *sl, periods prd, int **check_seq, int N, Options opt);
long long bruteOddLimitLCG_adv(Options &opt, vector<int> &seq);

//
// Functions for brute when limit is power of 2
//
void thr_thr_brutePowerOf2LimitLCG2(found *sl, SafeLong *sl2, vector<int> seq, Options opt, int p, long long skip);
void thr_thr_brutePowerOf2LimitLCG(int index, found *sl, SafeLong *sl2, int **check_seq, int N, Options opt, int p, long long skip);
void thr_brutePowerOf2LimitLCG(int index, found *sl, int **check_seq, int N, Options opt, int p, long long skip);
long long brutePowerOf2LimitLCG(Options &opt, vector<int> &seq);

// Bute even limit
long long bruteEvenLimitLCG(int limit, vector<int> &seq, bool debug);

//
// When initial seed is time in milliseconds
//
void thr_thr_brute_st(found *sl, SafeLong *sl2, vector<int> seq, Options opt);
long long brute_st(Options &opt, vector<int> &seq);

//
// The same, but advanced algorith is used (as it is in case of odd limit)
//
void thr_thr_brute_st_adv(found *sl, SafeLong *sl2, periods prd, vector<int> seq, Options opt);
long long brute_st_adv(Options &opt, vector<int> &seq);

//
// nextLong and nextInt cases
//
long long brutenextIntwoLimit(Options &opt, vector<int> &av);
long long brutenextLong(Options &opt, vector<long long> &av);

//
// Multi-thread time calculator
//
void brute_dumb(Options &opt, vector<int> &seq);
void thr_thr_brute_dumb(found *sl, SafeLong *sl2, vector<int> seq, Options opt, mutex *m, long long *s);

//
// Brute combiane: call all finctions above depending on limit and advanced options
//
long long brute(Options &opt, vector<int> &av);

#endif


