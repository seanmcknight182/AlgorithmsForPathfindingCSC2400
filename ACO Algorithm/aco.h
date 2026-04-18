#pragma once
#include "graph.h"
#include <vector>
#include <limits>
#include <random>
#include <algorithm>
#include <iostream>
#include <numeric>

//  Ant Colony Optimization — How It Works

//  Parameters used here and what they control:
//    numAnts       — how many ants explore per iteration
//    numIteratrations — how long to run before stopping
//    phInfluence          — pheromone influence; higher → more exploitation
//    distInfluence           — heuristic influence; higher → greedier choices
//    RHO            — evaporation rate (0 < ρ < 1)
//    Q              — pheromone deposit constant (scales deposit amount)
//    TAU_INIT       — starting pheromone on all edges

struct AcoParams {
    int    numAnts       = 30;
    int    numIterations = 200;
    double phInfluence   = 1.0;   // pheromone exponent
    double distInfluence = 3.0;   // heuristic exponent
    double rho           = 0.1;   // evaporation rate
    double Q             = 100.0; // deposit constant
    double initialPh     = 1.0;   // initial pheromone on every edge
};

struct AcoResult {
    std::vector<int> path;        // sequence of node indices from start to goal
    double           totalCost;   // sum of Manhattan distances along the path
    int              iterations;  // how many iterations were run
};

class AntColonyOptimization {
public:
    AntColonyOptimization(const std::vector<Node>& graph,
                          int startNode,
                          int goalNode,
                          const AcoParams& params = AcoParams{})
        : graph_(graph),
          start_(startNode),
          goal_(goalNode),
          params_(params),
          rng_(std::random_device{}())
    {
        initPheromones();
    }

    // Run the ACO and return the best path found.
    AcoResult run() {
        AcoResult best;
        best.totalCost = std::numeric_limits<double>::infinity();
        best.iterations = params_.numIterations;

        for (int iter = 0; iter < params_.numIterations; ++iter) {

            // Each ant constructs a tour
            std::vector<std::vector<int>> allPaths;
            std::vector<double>           allCosts;

            for (int ant = 0; ant < params_.numAnts; ++ant) {
                std::vector<int> path;
                double cost = 0.0;
                if (constructTour(path, cost)) {
                    allPaths.push_back(path);
                    allCosts.push_back(cost);

                    if (cost < best.totalCost) {
                        best.totalCost = cost;
                        best.path      = path;
                    }
                }
            }

            // Update pheromones
            evaporate();
            depositPheromones(allPaths, allCosts);

            if ((iter + 1) % 50 == 0) {
                std::cout << "  Iteration " << (iter + 1)
                          << " | Best so far: " << best.totalCost << "\n";
            }
        }

        return best;
    }

private:
    const std::vector<Node>& graph_;
    int                      start_;
    int                      goal_;
    AcoParams                params_;
    std::mt19937             rng_;

    // pheromone_[u][i] = pheromone on edge from u to u's i-th neighbor
    std::vector<std::vector<double>> pheromone_;

    void initPheromones() {
        pheromone_.resize(graph_.size());
        for (size_t u = 0; u < graph_.size(); ++u) {
            pheromone_[u].assign(graph_[u].neighbors.size(), params_.initialPh);
        }
    }

    bool constructTour(std::vector<int>& path, double& cost) {
        std::vector<bool> visited(graph_.size(), false);
        path.clear();
        cost = 0.0;

        int current = start_;
        path.push_back(current);
        visited[current] = true;

        // Safety cap: prevent infinite loops on malformed graphs
        const int maxSteps = static_cast<int>(graph_.size()) * 2;

        for (int step = 0; step < maxSteps; ++step) {
            if (current == goal_) return true;

            int next = chooseNext(current, visited);
            if (next == -1) return false; // stuck — no valid moves

            cost += manhattanDist(graph_[current], graph_[next]);
            visited[next] = true;
            path.push_back(next);
            current = next;
        }

        return (current == goal_);
    }

    int chooseNext(int current, const std::vector<bool>& visited) {
        const auto& neighbors = graph_[current].neighbors;
        if (neighbors.empty()) return -1;

        // Compute unnormalized weights for each allowed neighbor
        std::vector<double> weights;
        std::vector<int>    candidates;

        for (size_t i = 0; i < neighbors.size(); ++i) {
            int v = neighbors[i];
            if (visited[v]) continue;

            double dist = manhattanDist(graph_[current], graph_[v]);
            if (dist <= 0.0) dist = 1e-9; // avoid divide-by-zero

            double tau = pheromone_[current][i];
            double eta = 1.0 / dist;   // heuristic: shorter edge is better

            double weight = std::pow(tau, params_.phInfluence) *
                            std::pow(eta, params_.distInfluence);

            candidates.push_back(v);
            weights.push_back(weight);
        }

        if (candidates.empty()) return -1;

        // Roulette-wheel selection
        std::discrete_distribution<int> dist(weights.begin(), weights.end());
        return candidates[dist(rng_)];
    }

    void evaporate() {
        for (auto& row : pheromone_) {
            for (double& tau : row) {
                tau *= (1.0 - params_.rho);
                if (tau < 1e-9) tau = 1e-9; // floor to prevent extinction
            }
        }
    }

    void depositPheromones(const std::vector<std::vector<int>>& paths,
                           const std::vector<double>&           costs)
    {
        for (size_t a = 0; a < paths.size(); ++a) {
            double deposit = params_.Q / costs[a];
            const auto& path = paths[a];

            for (size_t step = 0; step + 1 < path.size(); ++step) {
                int u = path[step];
                int v = path[step + 1];

                // Find which neighbor slot v occupies in u's adjacency list
                const auto& nbrs = graph_[u].neighbors;
                for (size_t i = 0; i < nbrs.size(); ++i) {
                    if (nbrs[i] == v) {
                        pheromone_[u][i] += deposit;
                        break;
                    }
                }
            }
        }
    }
};
