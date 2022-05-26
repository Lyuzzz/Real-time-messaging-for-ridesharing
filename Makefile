# CXX Make variable for compiler
CXX=g++
# Make variable for compiler options
#	-std=c++11  C/C++ variant to use, e.g. C++ 2011
#	-g          include information for symbolic debugger e.g. gdb 
CXXFLAGS=-std=c++11 -g

# pthread compiler
PTHREADFLAG=-pthread
# Rules format:
# target : dependency1 dependency2 ... dependencyN
#     Command to make target, uses default rules if not specified

# First target is the one executed if you just type make
# make target specifies a specific target
# $^ is an example of a special variable.  It substitutes all dependencies
rideshare : io.o consumer.o producer.o main.o
	$(CXX) $(PTHREADFLAG) -o rideshare $^

io.o : io.h io.cpp

consumer.o : consumer.h consumer.cpp

producer.o : producer.h producer.cpp

main.o : io.h io.cpp consumer.h consumer.cpp producer.h producer.cpp ridesharing.h shareData.h main.cpp

clean :
	rm *.o

