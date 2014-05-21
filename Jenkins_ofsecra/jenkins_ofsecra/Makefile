CC = g++ -std=c++11 -pthread
vpath %.cpp src
vpath %.h src

all: jenkins_ofsecra

jenkins_ofsecra: JavaLCGMimic.o md5.o Options.o utils.o main.o
	$(CC) -o $@ $^

md5.o: md5.cpp
	$(CC) -c $<

Options.o: Options.cpp
	$(CC) -c $<

utils.o: utils.cpp
	$(CC) -c $<

main.o: main.cpp
	$(CC) -c $<

JavaLCGMimic.o: JavaLCGMimic.cpp
	$(CC) -c $<

clean:
	rm *.o jenkins_ofsecra

