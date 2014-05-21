// JavaLCGMimic class mimics Java class java.util.Random.
//
// Programmers mostly use nextInt(int n) method of java.util.Random.
// Which produces pseudo random value between 0 and n - 1.
//
// If instances of JavaLCGMimic and java.util.Random classes are seeded with the
// same value, they produce identical outputs.

#ifndef _JavaLCGMimic_
#define _JavaLCGMimic_

//#include "Utils.h"

using namespace std;

class JavaLCGMimic
{
public:
	const long long multiplier = 0x5deece66dL, //2^34,5535577
		invmultiplier = 0xdfe05bcb1365L, //multiplier^-1 mod 2^48
		addend = 11L,
		mask = 0xffffffffffffL; // 2^48 - 1
	long long seed;

	JavaLCGMimic(long long sseed=0) : seed( (sseed^multiplier)&mask ) {};
	//JavaLCGMimic(long long sseed = 0) : seed(sseed) {};
	~JavaLCGMimic();

	void init(long long sseed);
	void forceSeed(long long fseed);
	int nextInt(int limit);
	int prevInt(int limit);
	long long nextLong();

private:
	int nextI(int i);
	int prevI(int i);
};

#endif