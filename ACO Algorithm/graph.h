#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cmath>

// Represents one node in the graph.
// Loaded directly from the .dat file produced by dataGeneration.cpp.
// Each line in that file is: x y neighbor1 neighbor2 neighbor3 neighbor4
// A neighbor value of -1 means "no edge".
struct Node {
    int x, y;
    std::vector<int> neighbors; // indices of adjacent nodes (up to 4)
};

// Returns Manhattan distance between two nodes (the edge weight used throughout).
inline double manhattanDist(const Node& a, const Node& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

// Loads the graph written by dataGeneration.cpp::outputPoints().
// Expected format (one line per node, 300 lines total):
//   x y n1 n2 n3 n4
// where n1..n4 are neighbor indices (-1 = absent).
inline std::vector<Node> loadGraph(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open graph file: " + filePath);
    }

    std::vector<Node> graph;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        Node node;
        int n1, n2, n3, n4;

        if (!(iss >> node.x >> node.y >> n1 >> n2 >> n3 >> n4)) {
            throw std::runtime_error("Malformed line in graph file: " + line);
        }

        // Only add valid (non -1) neighbors
        for (int n : {n1, n2, n3, n4}) {
            if (n != -1) node.neighbors.push_back(n);
        }

        graph.push_back(node);
    }

    if (graph.empty()) {
        throw std::runtime_error("Graph file is empty or could not be parsed.");
    }

    return graph;
}
