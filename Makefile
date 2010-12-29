CXX = g++
CXXFLAGS = -Wall
OBJS = ice_reactor.o test.o

all: $(OBJS) test

test: $(OBJS)
	$(CXX) -o $@ $^

.cc.o:
	$(CXX) -c $(CXXFLAGS) -o $@ $<

clean:
	rm -rf *.o test
