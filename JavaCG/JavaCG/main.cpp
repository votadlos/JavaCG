//
// This is main entry point javacg java.util.Random bure combine
// Files description:
// Brute.h - all brute functions
// JavaLCGMimic - java.util.Random realization
// JavaLCGSub - subgenerator used in case of even limit
// Options - command-line options for this program
// PWChars - class for translation string passwords to vector of int and back
// SafeLong - class that can return seed portions for asking threads and number of auxilary structures
// Utils - number of auxiliary functions for multithread brute
//
//
// All this code and any part of it can be used absolutely free
//
// sergey v. soldatov, 2013-2014
//
//
//


#include <iostream>
#include <fstream>
#include <cstdlib>
#include "Options.h"
#include "Utils.h"
#include "PWChars.h"
#include "Brute.h"

using namespace std;

int main(int argc, char *argv[]){

	//cout << time(0) << endl; //DEBUG

	vector<int> limits;
	Options opt(argc, argv, limits);
	if (opt.opt_d) opt.showOpt();
	
	//cout << "limits=" << limits << endl;//DEBUG
	JavaLCGMimic g;
	

	//
	// print help
	//
	if (opt.opt_h){
		return 0;
	}
	
	
	//
	// Generate
	//
	if (opt.opt_g > 0){//just generate seq
		long long seed = opt.opt_s<0 ? ((static_cast<long long>(chrono::steady_clock::now().time_since_epoch().count()) ^ g.multiplier) & g.mask) : opt.opt_s;
		
		//
		//java.util.Random's default constructor from uptime
		//
		srand(time(0));
		if (opt.opt_upt > 0) seed = ((opt.opt_upt * 1000000 + (rand() % 1000000) + ((rand() % opt.opt_su) + 1 + g.seedUniquifier)) ^ g.multiplier) & g.mask;
		/////

		g.forceSeed(seed);

		if (opt.opt_p > 0){ //generate opt_g passwords with length of opt_p
			PWChars pw;
			if (opt.opt_a != 0){ //alfabet is specified
				ifstream in(opt.opt_a);
				string al;
				in >> al;
				pw.setAlfabet(al);
			}
			
			vector<int> pn; 
			if (pw.getLimit() < opt.opt_l){
				cout << "Please, specify alfabet. Default alfabet is too short :(" << endl;
				return 10;
			}

			if (opt.opt_b > 0) //wind backward
				for (int j = 0; j < opt.opt_p * (opt.opt_b+1); j++) g.prevInt(opt.opt_l);
			
			for (int i = 0; i < (opt.opt_b > 0 ? opt.opt_b : opt.opt_g); i++){
				pn.clear();
				for (int j = 0; j < opt.opt_p; j++) pn.push_back(g.nextInt(opt.opt_l));
				cout << pw.getChars(pn) << endl;
			}
			cout << endl<<"Alfabet = " << pw.getAlfabet().substr(0, opt.opt_l) << endl;
		}
		else if (opt.opt_st){ //time in milliseconds initialization
			time_t t = time(0);
			srand(t);
			seed = 1000 * t + (rand() % 1000);
			g.forceSeed(seed);
			for (int i = 0; i < opt.opt_g; i++){
				cout << g.nextInt(opt.opt_l) << " ";
				//cout << "<" << g.seed << ">";
				//cout << g.nextInt(opt.opt_l);
				//cout << "<" << g.seed << "> ";
			}
		}
		else if (opt.opt_l == 0) { //nextInt() case
			for (int i = 0; i < opt.opt_g; i++){
				cout << g.nextInt() << " ";
				//cout << "<" << g.seed << ">";
				//cout << g.nextInt();
				//cout << "<" << g.seed << "> ";
			}
		}
		else if (opt.opt_l == -1) { //nextLong() case
			for (int i = 0; i < opt.opt_g; i++){
				cout << g.nextLong() << " ";
				//cout << "<" << g.seed << ">";
				//cout << g.nextLong();
				//cout << "<" << g.seed << "> ";
			}
		}
		else {
			for (int i = 0; i < opt.opt_g; i++){
				cout << g.nextInt(opt.opt_l) << " ";
				//cout << "<" << g.seed << ">";
				//cout << g.nextInt(opt.opt_l);
				//cout << "<" << g.seed << "> ";
			}
		}
		cout << endl << "init seed = " << seed << endl << "limit = " << opt.opt_l << endl << "end seed = " << g.seed << endl;

		return 0;
	}

	
	
	vector<int> av;
	vector<long long> avl;



	//
	// Test 
	//
	if (opt.opt_tst > 0){ //self test
		cout << "[+] Starting self test" << endl;
		clock_t start_all = clock();
		int fsize[] = {5,6,22,10}; //field size to quick change

		cout << setw(fsize[0]) << "N" 
			<< setw(fsize[1]) << "lim"
			<< setw(fsize[2]) << "seed" 
			<< setw(fsize[2]) << "next seed" 
			<< setw(fsize[3]) << "msec" 
			<< " Sequence" << endl;
		for (auto it = limits.begin(); it != limits.end(); it++){
			long long seed, next_seed = g.mask;
			for (int ii = 0; ii < opt.opt_tst; ii++){ //start test
				opt.opt_l = *it;

				seed = (static_cast<long long>(chrono::steady_clock::now().time_since_epoch().count())<<17) & next_seed;
				g.forceSeed(seed);

				av.clear();
				for (int i = 0; i < opt.opt_n; i++) av.push_back(g.nextInt(opt.opt_l));
				clock_t start_1 = clock();
				next_seed = brute(opt, av);
				cout << setw(fsize[0]) << ii
					<< setw(fsize[1]) << opt.opt_l
					<< setw(fsize[2]) << seed
					<< setw(fsize[2]) << next_seed
					<< setw(fsize[3]) << (clock() - start_1) 
					<<" "<< av << endl;
				
				if (opt.opt_ds){ //test without advanced options
					opt.opt_ds = false;
					next_seed = brute(opt, av);
					cout << setw(fsize[0]) << ii
						<< setw(fsize[1]-2) << opt.opt_l << "/s"
						<< setw(fsize[2]) << seed
						<< setw(fsize[2]) << next_seed
						<< setw(fsize[3]) << (clock() - start_1) 
						<< " " << av << endl;
					opt.opt_ds = true;
				}
			}
		}
		cout << "[+] Self test finished in " << (clock() - start_all) <<" msec"<< endl;
		
		return 0;
	}
	


	avl.clear();
	av.clear();
	

	//
	// Brute, default case
	//
	if (opt.opt_l == -1){ //nextLong case
		cout << "[+] Will perform nextLong() case" << endl;

		if (opt.opt_fn != 0){ //read from file
			ifstream fin(opt.opt_fn);
			cout << "[+] Read from file: " << opt.opt_fn << endl;
			long long inputsmth;
			int i = 0;
			while (i++ < opt.opt_n){
				fin >> inputsmth;
				avl.push_back(inputsmth);
			}
			fin.close();
		}
		else{ //read from stdin
			cout << "Enter " << opt.opt_n << " numbers you have>";
			long long inputsmth;
			int i = 0;
			while (i++ < opt.opt_n){
				cin >> inputsmth;
				avl.push_back(inputsmth);
			}
		}
		cout << "[+] Input: " << avl << endl;

		clock_t startt = clock();
		if (opt.opt_d) cout << "[DEBUG] Go to brutenextLong (" << opt.opt_l << ")" << endl;
		long long seed = brutenextLong(opt, avl); //BRUTE

		cout << endl << "[+] Main finished in " << (clock() - startt) / CLOCKS_PER_SEC << " sec" << endl;
		if (seed < 0){
			cout << "[+] Seed not found " << endl;
		}
		else {
			cout << "[+] Next seed = " << seed << endl;
			g.forceSeed(seed);
			cout << "[+] Next " << opt.opt_f << " elements: ";
			for (int i = 0; i < opt.opt_f; i++)
				cout << g.nextLong() << " ";
			cout << endl;
		}
	}
	else { //nextInt(limit), nextInt()
		cout << "[+] Will perform nextInt(), nextInt(limit) case" << endl;

		if (opt.opt_fn != 0){ //read from file
			ifstream fin(opt.opt_fn);
			cout << "[+] Read from file: " << opt.opt_fn << endl;
			int inputsmth;
			int i = 0;
			while (i++ < opt.opt_n){
				fin >> inputsmth;
				av.push_back(inputsmth);
			}
			fin.close();
		}
		else { //read from stdin
			if (opt.opt_p > 0){ //read passwords from stdin
				PWChars pw;
				if (opt.opt_a != 0){ //alfabet is specified
					ifstream in(opt.opt_a);
					string al;
					in >> al;
					pw.setAlfabet(al);
				}
				if (pw.getLimit() < opt.opt_l){
					cout << "Please, specify alfabet. Default alfabet is too short :(" << endl;
					return 10;
				}

				cout << "Enter password[s] you have (-n option is ignored)>";
				string inputsmth;
				cin >> inputsmth;
				cout << "[+] Input pw: " << inputsmth << endl;

				av = pw.getNumbers(inputsmth); //get numbers
				//opt.opt_l = pw.getLimit(); //update limit
				opt.opt_n = av.size(); //update number of elements

			}
			else{ //read numbers from stdin
				cout << "Enter " << opt.opt_n << " numbers you have>";
				int inputsmth;
				int i = 0;
				while (i++ < opt.opt_n){
					cin >> inputsmth;
					av.push_back(inputsmth);
				}
			}
		}
		cout << "[+] Input: " << av << endl;
	
		if (0 != check_limit(opt.opt_l, av)){
			cout << "ERROR: Sequence does not correspond limit" << endl;
			return 10;
		}

		//
		// Test for time consuming 
		//
		if (opt.opt_tst2 >= 0.0){
			cout << "[+] Starting time test" << endl;
			brute_dumb(opt, av);

			return 0;
		}
		///////////////////////////////////////////////////
		
		clock_t startt = clock();
		long long seed = brute(opt, av); //BRUTE combine

		cout << endl << "[+] Main finished in " << (clock() - startt) / CLOCKS_PER_SEC << " sec" << endl;
		if (seed < 0){
			cout << "[+] Seed not found " << endl;
		}
		else {
			cout << "[+] Limit = " << opt.opt_l <<
				endl << "[+] Next seed = " << seed << endl;
			g.forceSeed(seed);
			if (opt.opt_b != -1){ //show previous elements
				cout << "[+] Previous " << opt.opt_b << " elements: ";
				for (int i = opt.opt_n - 1; i > 0; i--) g.prevInt(opt.opt_l);//wind to the first
				for (int i = 0; i < opt.opt_b; i++) cout << g.prevInt(opt.opt_l) << " ";
			}
			else { //show next elements
				cout << "[+] Next " << opt.opt_f << " elements: ";
				for (int i = 0; i < opt.opt_f; i++) {
					if (opt.opt_l == 0) //nextInt
						cout << g.nextInt() << " ";
					else
						cout << g.nextInt(opt.opt_l) << " ";
				}
			}
			cout << endl;
		}
	}

	return 0;
}
