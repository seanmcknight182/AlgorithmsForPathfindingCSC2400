CXX = g++
CXXFLAGS = -std=c++17 -O2

ALGO_DIRS = ACO_Algorithm A_Star_Algorithm Algorithm1Dijkstra Bellman_Ford_Algorithm
DATA_GEN = dataGen
ALGO_EXE = runAlgo

SIZES = 200 400 800 1600 3200 6400

all: run

# Compile everything
compile: $(DATA_GEN) algos

$(DATA_GEN): dataGeneration.cpp
	$(CXX) $(CXXFLAGS) dataGeneration.cpp -o $(DATA_GEN)

algos:
	for dir in $(ALGO_DIRS); do \
		$(CXX) $(CXXFLAGS) $$dir/main.cpp -o $$dir/$(ALGO_EXE); \
	done

# Full pipeline
run: compile loop python clean

# 🔁 Main loop: generate → copy → run (for each size)
loop:
	for n in $(SIZES); do \
		echo "===== Running with $$n points ====="; \
		./$(DATA_GEN) $$n; \
		for dir in $(ALGO_DIRS); do \
			echo "Copying dataset to $$dir"; \
			cp thePoints.dat $$dir/thePoints.dat; \
		done; \
		for dir in $(ALGO_DIRS); do \
			echo "Running $$dir"; \
			(cd $$dir && ./$(ALGO_EXE) $$n); \
		done; \
	done

# Run Python plotting
python:
	python3 script.py

# Cleanup executables
clean:
	rm -f $(DATA_GEN)
	for dir in $(ALGO_DIRS); do \
		rm -f $$dir/$(ALGO_EXE); \
	done
