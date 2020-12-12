CXX = g++
CXXFLAGS = -Wall -std=c++11 

OBJECTS = nqueens.o

nqueens: $(OBJECTS)
	$(CXX) -g $(CXXFLAGS) -o nqueens nqueens.cpp
	
nqueens.o: nqueens.cpp
	$(CXX) -g $(CXXFLAGS) -c nqueens.cpp

clean: 
	rm -f *.o
	rm nqueens
