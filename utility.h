/*
    specification of graph-related definitions and helper methods
*/

#pragma once

#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <cassert>

const int INF = 1.1e9;

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
int randint(int l, int r) {
    return std::uniform_int_distribution<int>(l, r)(rng);
}

struct Edge {
    int u, v, t, lambda;
};

void streamPresentation_ize(int &n, std::vector<Edge> &edges) { // sorts the edges :)
    std::sort(edges.begin(), edges.end(), [&](const Edge &e1, const Edge &e2) {
        return e1.t < e2.t;
    });
}

std::pair<int, std::vector<Edge>> randomGraph(int lower_n = 1, int upper_n = 20, int lower_m = 0, int upper_m = 30, int T = 100, int L = 20) {
    int n = randint(lower_n, upper_n);
    int m = randint(lower_m, upper_m);
    std::vector<Edge> edges;
    for (int i = 0; i < m; i++) {
        int u = randint(1, n);
        int v = randint(1, n);
        int t = randint(0, T);
        int lambda = randint(1, L);
        edges.push_back(Edge{u, v, t, lambda});
    }
    return make_pair(n, edges);
}

std::pair<int, std::vector<Edge>> loadGraph(std::filesystem::path filepath) {
    std::ifstream inp(filepath);
    std::string goat;
    inp >> goat >> goat >> goat >> goat;
    int n, m;
    inp >> m >> n >> n;
    std::vector<Edge> edges;
    for (int i = 0; i < m; i++) {
        int u, v, t, lambda;
        inp >> u >> v >> lambda >> t;
        assert(lambda < INF && t < INF);
        edges.push_back({u, v, t, lambda});
    }
    inp.close();
    return make_pair(n, edges);
}

bool isPathValid(const int &n, const std::vector<Edge> &edges, const std::vector<Edge> &path) {
    for (int i = 0; i + 1 < path.size(); i++) {
        const Edge &e1 = path[i];
        const Edge &e2 = path[i + 1];
        if (e1.v != e2.u) return false;
        if (e1.t + e1.lambda > e2.t) return false;
    }
    return true;
}
