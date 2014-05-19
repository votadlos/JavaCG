#include "JavaLCGMimic.h"


JavaLCGMimic::~JavaLCGMimic()
{
}

int JavaLCGMimic::nextI(int i){
	
	this->seed = (this->seed*this->multiplier + this->addend) & this->mask;

	return (this->seed >> (48 - i));
}

int JavaLCGMimic::prevI(int i){

	this->seed = (this->invmultiplier*(this->seed - this->addend)) & this->mask;

	return (this->seed >> (48 - i));
}

void JavaLCGMimic::forceSeed(long long fseed){
	this->seed = fseed;
}

int JavaLCGMimic::nextInt(){
	return this->nextI(32);
}

long long JavaLCGMimic::nextLong(){
	return ( (static_cast<long long>(this->nextI(32))<<32) + static_cast<long long>(this->nextI(32)) );
}

int JavaLCGMimic::nextInt(int limit){
	if (limit <= 0)
		return limit;

	if ((limit & -limit) == limit) // limit is power of 2
		return static_cast<int>((static_cast<long long>(limit)*static_cast<long long>(this->nextI(31)) >> 31)); // output limit*(seed >> 17) >> 31

	int j, k;
	do {// limit is not power of 2
		j = this->nextI(31);// (seed >> 17)
		k = j % limit; // output is(seed >> 17) modulo limit
	} 
	while ((j - k) + (limit - 1) < 0); // remove statistical bias

	return k;

}

int JavaLCGMimic::prevInt(int limit){
	if (limit <= 0)
		return limit;

	if ((limit & -limit) == limit) // limit is power of 2
		return static_cast<int>((static_cast<long long>(limit)*static_cast<long long>(this->prevI(31)) >> 31)); // output limit*(seed >> 17) >> 31

	int j, k;
	do {// limit is not power of 2
		j = this->prevI(31);// (seed >> 17)
		k = j % limit; // output is(seed >> 17) modulo limit
	} while ((j - k) + (limit - 1) < 0); // remove statistical bias

	return k;

}

