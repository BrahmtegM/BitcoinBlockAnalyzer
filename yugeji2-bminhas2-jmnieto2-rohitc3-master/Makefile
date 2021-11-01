# Executable Name
EXENAME = main

# Object Types
OBJS = main.o graph.o

# Compilation Flags
CXX = clang++
CXXFLAGS = $(CS225) -std=c++14 -stdlib=libc++ -c -g -Ofast -Wall -Wextra -pedantic
LD = clang++
LDFLAGS = -std=c++14 -stdlib=libc++ -lc++abi -lm

# Custom Clang Enforcement
include make/customClangEnforcement.mk

# Define Makefile Rules
.PHONY: all test clean output_msg

all : $(EXENAME)

output_msg: ; $(CLANG_VERSION_MSG)

$(EXENAME): output_msg $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(EXENAME)

main.o : main.cpp graph.cpp graph.h 
	$(CXX) $(CXXFLAGS) main.cpp

graph.o : graph.cpp graph.h
	$(CXX) $(CXXFLAGS) graph.cpp

test: output_msg catch/catchmain.cpp tests/tests.cpp graph.cpp
	$(LD) catch/catchmain.cpp tests/tests.cpp graph.cpp $(LDFLAGS) -o test

clean:
	-rm -f *.o $(EXENAME) test