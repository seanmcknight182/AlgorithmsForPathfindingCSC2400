#include <iostream>
#include <vector>
#include <string>
#include "graph.h"
#include "aco.h"

//  main.cpp — Ant Colony Optimization shortest path finder
//
//  Usage:
//    ./aco [path/to/thePoints.dat]
//
//  If no argument is given, the program looks for thePoints.dat
//  in the current directory (the default output of dataGeneration).
//
//  The program:
//    1. Loads the graph from the .dat file
//    2. Runs ACO from node 0 (0,0) to node 299 (99,99)
//    3. Prints the best path found and its total Manhattan distance

int main(int argc, char* argv[]) {
    // Determine the data file path from the command line, or use default
    std::string dataFile = "thePoints.dat";
    if (argc >= 2) {
        dataFile = argv[1];
    }

    // Load graph
    std::vector<Node> graph;
    try {
        graph = loadGraph(dataFile);
    } catch (const std::exception& e) {
        std::cerr << "Error loading graph: " << e.what() << "\n";
        std::cerr << "Make sure you have run dataGeneration first to produce "
                  << dataFile << ".\n";
        return 1;
    }

    std::cout << "Graph loaded: " << graph.size() << " nodes.\n";
    std::cout << "Start: node 0 at ("
              << graph[0].x << ", " << graph[0].y << ")\n";
    std::cout << "Goal : node 299 at ("
              << graph[299].x << ", " << graph[299].y << ")\n\n";

    // Configure ACO parameters
    AcoParams params;
    params.numAnts       = 30;    // ants exploring per iteration
    params.numIterations = 200;   // total iterations to run
    params.phInfluence   = 1.0;   // how strongly pheromone influences choice
    params.distInfluence = 3.0;   // how strongly distance heuristic influences choice
    params.rho           = 0.1;   // pheromone evaporation rate per iteration
    params.Q             = 100.0; // pheromone deposit scaling constant
    params.initialPh     = 1.0;   // starting pheromone level on every edge

    std::cout << "Running ACO with:\n"
              << "  Ants per iteration : " << params.numAnts       << "\n"
              << "  Iterations         : " << params.numIterations  << "\n"
              << "  phInfluence (pheromone)  : " << params.phInfluence          << "\n"
              << "  distInfluence  (distance)  : " << params.distInfluence           << "\n"
              << "  Rho   (evaporation): " << params.rho            << "\n"
              << "  Q     (deposit)    : " << params.Q              << "\n\n";

    // Run ACO
    AntColonyOptimization aco(graph, /*start=*/0, /*goal=*/299, params);
    AcoResult result = aco.run();

    // Print results
    std::cout << "\n=== ACO Complete ===\n";

    if (result.path.empty()) {
        std::cout << "No valid path from node 0 to node 299 was found.\n"
                  << "This may indicate the graph is disconnected. "
                  << "Try regenerating thePoints.dat.\n";
        return 1;
    }

    std::cout << "Best path length (Manhattan distance): "
              << result.totalCost << "\n";
    std::cout << "Path node count  : " << result.path.size() << " nodes\n\n";

    std::cout << "Path (node index -> x,y):\n";
    for (size_t i = 0; i < result.path.size(); ++i) {
        int idx = result.path[i];
        std::cout << "  [" << i << "] Node " << idx
                  << " (" << graph[idx].x << ", " << graph[idx].y << ")\n";
    }

    // Theoretical minimum Manhattan distance from (0,0) to (99,99)
    std::cout << "\nTheoretical minimum (direct Manhattan): 198\n";

    return 0;
}
