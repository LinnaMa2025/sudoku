#!!! This file is re-written by AI !!!

APP := sudoku

CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -O2

SRC := $(wildcard **/*.cpp)
OBJ := $(SRC:.cpp=.o)

all: $(APP)

$(APP): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(APP)

.PHONY: all clean