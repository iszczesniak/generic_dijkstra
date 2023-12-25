TESTS = generic_label generic_permanent generic_permanent2	\
	generic_tentative

CXXFLAGS = -g -Wno-deprecated
#CXXFLAGS = -O3 -Wno-deprecated

CXXFLAGS := $(CXXFLAGS) -std=c++2b
CXXFLAGS := $(CXXFLAGS) -I ../
CXXFLAGS := $(CXXFLAGS) -I props
CXXFLAGS := $(CXXFLAGS) -I units
CXXFLAGS := $(CXXFLAGS) -I units/test

# Use the C++ linker
LINK.o = $(LINK.cc)

# Run the tests.
run: $(TESTS)
	@for i in $(TESTS); do echo "Running" $$i; ./$$i; done

count:
	wc -l *.hpp *.cc

clean:
	rm -rf *~
	rm -rf *.o
	rm -rf $(TESTS)

depend:
	g++ -MM $(CXXFLAGS) -I../ *.cc > dependencies

include dependencies
