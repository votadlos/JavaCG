//
// Number of technical classes and structures that optimise serch algorithms in multi-thread environment
//

#pragma once
#include <mutex>
#include <iostream>
#include <iomanip>
#include <thread>

using namespace std;

typedef struct _periods {
	int p, d;
	double period;
} periods;

typedef struct _found {
	long long seed = -1;
	bool found = false;
	int index; //who found
} found;

typedef struct _minmaxval {
	long long min, max;
} minmaxval;

typedef struct _index_elem {
	int index;
	bool deleted = false;
} index_elem;

class SafeLong
{
public:
	SafeLong(long long v, long long vm, int c);
	~SafeLong();

	long long getValue(int mult=1);
	minmaxval getValue2(int mult=1); //for -norm options
	void setValue(long long v);
	long long getValue_min();
	void setValue_min(long long v);

private: 
	mutex _mutex_value;
	long long hole_begin, hole_end;
	bool direction;
	long long value;
	long long value_min;

	int cycles_per_thread;

	friend ostream &operator<<(ostream &out, SafeLong &sl);
};

ostream &operator<<(ostream &out, found &sl);
