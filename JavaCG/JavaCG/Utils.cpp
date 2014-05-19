
//
// Contains common functions
//

#include "Utils.h"

using namespace std;

int maxPowerOf2(int limit) {
	int i = 0, k = 1;
	while (k <= limit){
		if (limit % k == 0){
			i++;
			k *= 2;
		}
		else break;
	}
	return i - 1;
}

//compute period for odd limit
int computePeriod(int limit, long long A){
	int i = 0;
	while (true){
		if ((((A*i) >> 17) % limit) == (limit - 1)){
			//cout << "[+] d=" << i << endl;//DEBUG
			return i;
		}
		i++;
	}
}

int check_limit(int limit, vector<int> &v){
	int flag = 0;
	if (limit > 0){
		for (unsigned int i = 0; i < v.size(); i++){
			if (v[i] >= limit){
				cout << "ERROR: limit=" << limit << " < v[" << i << "]=" << v[i] << ", limit must be grater than v[" << i << "]" << endl;
				flag += 1;
			}
		}
	}
	return flag;
}

/*vector<string> string_split(string str, string delim){
	vector<string> ret;
	ret.clear();

	string token = "";
	int i = 0;
	while(i < str.length() ){
		if (str.substr(i, delim.length()) != delim)
			token += str[i++];
		else{
			ret.push_back(token);
			token = "";
			i += delim.length();
		}
	}
	ret.push_back(token);

	return ret;
}

vector<long long> vstring2vlong(vector<string> vs){
	vector<long long> ret;
	ret.clear();

	for (auto i = vs.begin(); i != vs.end(); i++){	
		stringstream strstream;
		long long a;
		strstream << *i;
		strstream >> a;
		strstream.flush();
		ret.push_back(a);
	}
	return ret;
}
*/

ostream &operator<<(ostream &out, vector<int> v){
	if (v.size()>0){
		out << "(";
		for (unsigned int i = 0; i<v.size() - 1; i++)
			out << v[i] << ", ";
		out << v[v.size() - 1] << ")";
	}
	else
		out << "Empty";

	return out;
}

ostream &operator<<(ostream &out, vector<long long> v){
	if (v.size()>0){
		out << "(";
		for (unsigned int i = 0; i<v.size() - 1; i++)
			out << v[i] << ", ";
		out << v[v.size() - 1] << ")";
	}
	else
		out << "Empty";

	return out;
}
