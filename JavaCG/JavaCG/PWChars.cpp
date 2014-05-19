#include "PWChars.h"

vector<int> PWChars::getNumbers(string &password)
{
	vector<int> ret;
	ret.clear();
	for (unsigned int i = 0; i < password.length(); i++)
		ret.push_back(this->alfabet.find(password[i]));

	return ret;
}

string PWChars::getChars(vector<int> &numbers)
{
	string ret = "";
	for (unsigned int i = 0; i < numbers.size(); i++)
		ret += this->alfabet[numbers[i]];

	return ret;
}

int PWChars::getLimit(){ return this->alfabet.length(); }

void PWChars::setAlfabet(string &a){
	this->alfabet = a;
}

string PWChars::getAlfabet(){
	return this->alfabet;
}


PWChars::~PWChars()
{
}
