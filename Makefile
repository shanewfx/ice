CXX = g++
CXXFLAGS = -Wall
all: ice_reactor.o

.cc.o:
	$(CXX) -c $(CXXFLAGS) -o $@ $<

clean:
	rm -rf *.o
