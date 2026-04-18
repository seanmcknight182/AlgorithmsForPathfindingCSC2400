# Makefile for CSC 2400 Group Project
# Builds dataGeneration (graph generator) and aco (Ant Colony Optimization)
#
# Usage:
#   make            — build both programs
#   make run        — generate the graph, then run ACO on it
#   make clean      — remove built binaries and data file

CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra

.PHONY: all run clean

all: dataGeneration aco

# Compile the data generator (unchanged from original)
dataGeneration: dataGeneration.cpp
	$(CXX) $(CXXFLAGS) -o dataGeneration dataGeneration.cpp

# Compile the ACO solver (header-only modules, so only main.cpp is a .cpp file)
aco: main.cpp graph.h aco.h
	$(CXX) $(CXXFLAGS) -o aco main.cpp

# Generate the graph then immediately run ACO against it
run: dataGeneration aco
	./dataGeneration
	./aco thePoints.dat

clean:
	rm -f dataGeneration aco thePoints.dat
