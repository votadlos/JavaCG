//
// Contains common functions
//

#ifndef _Utils_
#define _Utils_

#include <sstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int maxPowerOf2(int limit);
int check_limit(int limit, vector<int> &v);
int computePeriod(int limit, long long A);

//vector<string> string_split(string str, string delim);
//vector<long long> vstring2vlong(vector<string> vs);

ostream &operator<<(ostream &out, vector<int> v);
ostream &operator<<(ostream &out, vector<long long> v);

#endif
