//
// Class to convert password chars to numbers and back
//

#ifndef _PWChars_
#define _PWChars_

#include <string>
#include <vector>
#include <iostream>
//#include "JavaLCGMimic.h"

using namespace std;

class PWChars
{
public:
	PWChars(){};
	PWChars(string a) : alfabet(a) {};
	string getChars(vector<int> &v);
	vector<int> getNumbers(string &s);
	int getLimit();
	string getAlfabet();
	void setAlfabet(string &a);
	~PWChars();

private:
	string alfabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890`~!#$%^&*()-_=+[{]};:,<.>?"; //88
};

#endif
