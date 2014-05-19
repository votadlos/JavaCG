#include "SafeLong.h"

SafeLong::SafeLong(long long v, long long vm, int c){
	this->value = v;
	this->value_min = vm;
	
	this->cycles_per_thread = c;
	
	this->hole_begin = vm + (v - vm) / 2;
	this->hole_end = this->hole_begin;
	
	this->direction = false;
}


long long SafeLong::getValue(int mult){
	this->_mutex_value.lock();
	long long ret = this->value;
	this->value -= this->cycles_per_thread*mult;
	//cout << "Thread #" << setw(6) << this_thread::get_id() << " got:" << ret << "(" << (ret-this->value_min) <<")"<<endl;//DEBUG
	this->_mutex_value.unlock();

	return ret;
}
minmaxval SafeLong::getValue2(int mult){
	minmaxval ret;
	long long shift = this->cycles_per_thread*mult;
	this->_mutex_value.lock();
	if (this->direction){
		ret.min = this->hole_end;
		ret.max = ret.min + shift;
		this->hole_end = ret.max;
	}
	else{
		ret.max = this->hole_begin;
		ret.min = ret.max - shift;
		this->hole_begin = ret.min;
	}
	this->direction = !this->direction;
	//cout << setw(16) << right<<this->value_min << " (" << this->hole_begin << "," << this->hole_end << ") " << setw(16) <<left<<this->value<< endl;//DEBUG
	this->_mutex_value.unlock();

	return ret;
}

long long SafeLong::getValue_min(){ return this->value_min; }
void SafeLong::setValue(long long v){ this->value = v; }
void SafeLong::setValue_min(long long v){ this->value_min = v; }

SafeLong::~SafeLong(){}

ostream &operator<<(ostream &out, found &sl){
	out << "(" << sl.seed << "," << sl.found << ")";
	
	return out;
}

ostream &operator<<(ostream &out, SafeLong &sl){
	out <<"{ " << setw(16) << right << sl.value_min 
		<< " (" << sl.hole_begin << "," << sl.hole_end << ") " 
		<< setw(16) << left << sl.value 
		<<"["<<sl.direction<<"]"<< " }";
	
	return out;
}
