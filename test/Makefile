PROGS = generic_label generic_permanent generic_permanent2	\
generic_tentative

SRCS_generic_label = generic_label.cc
SRCS_generic_permanent = generic_permanent.cc
SRCS_generic_permanent2 = generic_permanent2.cc
SRCS_generic_tentative = generic_tentative.cc
MAN =

CXX = clang++-17

#CXXFLAGS += -g -Wno-deprecated
CXXFLAGS += -O3 -Wno-deprecated

CXXFLAGS += -std=c++23
CXXFLAGS += -I ../
CXXFLAGS += -I props
CXXFLAGS += -I units
CXXFLAGS += -I units/test

# Run the tests.
all: $(PROGS)
	@for i in $(PROGS); do echo "Running" $$i; ./$$i; done

.include <bsd.prog.mk>
