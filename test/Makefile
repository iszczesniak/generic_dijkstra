TESTS = generic_label

#CXXFLAGS = -g -Wno-deprecated
CXXFLAGS = -O3 -Wno-deprecated

CXXFLAGS := -std=c++20 -I ../ -I units

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
	g++ -MM -I../ *.cc > dependencies

include dependencies
