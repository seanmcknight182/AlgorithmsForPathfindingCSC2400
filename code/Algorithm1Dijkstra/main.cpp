#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <limits>
#include <queue>
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std;

struct Point {
    int x, y;
};

struct Edge {
    int to;
    int weight;
};

int Distance(const Point& a, const Point& b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

int main() {
    ifstream file("thePoints.dat");
    if (!file) {
        cerr << "Could not open file\n";
        return 1;
    }

    int basicOPCount = 0;
    int totalWeight = 0;

    // =========================
    // READ N (FIRST LINE)
    // =========================
    int N;
    file >> N;

    string line;
    getline(file, line); // consume newline after N

    vector<Point> points;
    vector<vector<Edge>> graph;

    points.reserve(N);
    graph.reserve(N);

    // =========================
    // READ GRAPH DATA
    // =========================
    for (int i = 0; i < N; i++) {

        if (!getline(file, line)) {
            cerr << "Unexpected end of file\n";
            return 1;
        }

        if (line.empty()) {
            i--;
            continue;
        }

        stringstream ss(line);

        Point p;
        int n1, n2, n3, n4;

        if (!(ss >> p.x >> p.y >> n1 >> n2 >> n3 >> n4)) {
            cerr << "Malformed line at node " << i << "\n";
            return 1;
        }

        points.push_back(p);
        graph.push_back({});

        int neigh[4] = {n1, n2, n3, n4};

        for (int j = 0; j < 4; j++) {
            int nb = neigh[j];
            if (nb >= 0 && nb < N) {
                graph[i].push_back({nb, 0}); // temporary weight
            }
        }
    }

    file.close();

    // =========================
    // COMPUTE EDGE WEIGHTS
    // =========================
    for (int i = 0; i < N; i++) {
        for (auto &edge : graph[i]) {
            edge.weight = Distance(points[i], points[edge.to]);
        }
    }

    int start = 0;

    vector<int> dist(N, numeric_limits<int>::max());
    vector<int> parent(N, -1);

    using P = pair<int, int>;
    priority_queue<P, vector<P>, greater<P>> pq;

    // =========================
    // TIMER START
    // =========================
    auto time1 = chrono::high_resolution_clock::now();

    dist[start] = 0;
    pq.push({0, start});

    // =========================
    // DIJKSTRA
    // =========================
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;

        for (auto &edge : graph[u]) {
            basicOPCount++;

            int v = edge.to;
            int w = edge.weight;

            int nd = d + w;

            if (nd < dist[v]) {
                dist[v] = nd;
                parent[v] = u;
                pq.push({nd, v});
            }
        }
    }

    // =========================
    // TIMER END (MS FLOAT)
    // =========================
    auto time2 = chrono::high_resolution_clock::now();

    chrono::duration<double, milli> duration = time2 - time1;
    double completionTime = duration.count();

    int target = N - 1;
    totalWeight = dist[target];

    // =========================
    // OUTPUT
    // =========================
    string theFileName = "0_" + to_string(N) + ".txt";
    ofstream outputfile(theFileName);

    if (!outputfile) {
        cerr << "Could not write output file\n";
        return 1;
    }

    outputfile << "time_ms,basic_op_count,weight\n";
    outputfile << completionTime << "," << basicOPCount << "," << totalWeight << "\n";

    outputfile.close();

    cout << "Done. Results written to " << theFileName << "\n";

    return 0;
}
