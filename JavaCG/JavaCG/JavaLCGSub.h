// Class JavaLCGSub represents LCG with reduced seed of lentgth 17 + MaxPowerOf2(limit).
//
// If we have two generators JavaLCGMimic and JavaLCGSub that are seeded with the same value.
// And limit is even but not the power of 2. And 2 ^ p divides limit.

// x y z - outputs of JavaLCGMimic.
// q w e - outputs of JavaLCGSub.

// Then LSB(x, p) = q, LSB(y, p) = w, and LSB(z, p) = e.
// Where LSB(x, p) - p least significant bits of x.I.e.LSB(00110, 3) = 110.
//

#ifndef _JavaLCGSub_
#define _JavaLCGSub_

#include <cmath>
#include "Utils.h"

using namespace std;

class JavaLCGSub
{
public:
	const long long multiplier = 0x5deece66dL,
		addend = 11L;
	long long mask, seed;
	int limit;

	JavaLCGSub(long long sseed, int llimit);
	~JavaLCGSub();

	void forceSeed(long long fseed);
	int nextInt();
};

#endif
