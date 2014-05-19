#include "JavaLCGSub.h"

JavaLCGSub::JavaLCGSub(long long sseed, int llimit)
{
	this->mask = static_cast<long long>(pow(2, 17 + maxPowerOf2(llimit))) - 1L;
	this->limit = llimit; 
	this->seed = (sseed^this->multiplier) & this->mask;
}

JavaLCGSub::~JavaLCGSub()
{
}

void JavaLCGSub::forceSeed(long long fseed){
	this->seed = fseed;
}

int JavaLCGSub::nextInt(){
	
	this->seed = (this->seed*this->multiplier + this->addend) & this->mask;

	return (this->seed >> 17) % static_cast<long long>( pow(2, maxPowerOf2(this->limit)) );
}

