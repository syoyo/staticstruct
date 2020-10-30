CXX=clang++
CXXFLAGS=-std=c++11 -Weverything -Werror -Wno-padded -Wno-c++98-compat

all:
	$(CXX) $(CXXFLAGS) -o example staticstruct.cc example.cc
