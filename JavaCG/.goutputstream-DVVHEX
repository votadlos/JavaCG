# 
# use /usr/local/lib64/libstdc++.so.6.0.18 and gcc 4.8.2
#
#

CC = /usr/local/bin/g++ -g -std=c++11 -pthread -static

vpath %.cpp JavaCG
vpath %.h   JavaCG
vpath %.hpp JavaCG

OPATH = Release



all: javacg

javacg: JavaLCGMimic.o JavaLCGSub.o Options.o PWChars.o SafeLong.o Utils.o Brute.o main.o
	cd $(OPATH); $(CC) -o $@ $^

JavaLCGMimic.o: JavaLCGMimic.cpp
	$(CC) -c $< -o $(OPATH)/$@

JavaLCGSub.o: JavaLCGSub.cpp
	$(CC) -c $< -o $(OPATH)/$@

main.o: main.cpp
	$(CC) -c $< -o $(OPATH)/$@

Options.o: Options.cpp
	$(CC) -c $< -o $(OPATH)/$@

PWChars.o: PWChars.cpp 
	$(CC) -c $< -o $(OPATH)/$@

SafeLong.o: SafeLong.cpp
	$(CC) -c $< -o $(OPATH)/$@

Utils.o: Utils.cpp
	$(CC) -c $< -o $(OPATH)/$@

Brute.o: Brute.cpp
	$(CC) -c $< -o $(OPATH)/$@

clean:
	rm $(OPATH)/javacg $(OPATH)/*.o 

