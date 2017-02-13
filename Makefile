CXX = g++
CXXFLAGS = -Wall -fopenmp -O3
PROGS = WEVOTE ABUNDANCE

.PHONY: all install clean

all: $(PROGS)

install: $(PROGS)

install: $(PROGS)
	cp $(PROGS) ../

clean:
	rm -f $(PROGS) *.o

WEVOTE: helpers.o create_db.o
ABUNDANCE: helpers.o create_db.o
helpers.o: helpers.cpp helpers.hpp
	$(CXX) $(CXXFLAGS) -c helpers.cpp

create_db.o: create_db.cpp create_db.hpp
	$(CXX) $(CXXFLAGS) -c create_db.cpp
	

