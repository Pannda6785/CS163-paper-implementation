#include "utility.h"

#include <vector>
using std::vector;

namespace naive {
    
    vector<vector<Edge>> findAllValidPath(const int &n, const vector<Edge> &edges) {
        vector<int> order(edges.size());
        iota(order.begin(), order.end(), 0);
        vector<vector<Edge>> paths;
        do {
            vector<Edge> path;
            for (int i : order) {
                path.push_back(edges[i]);
                if (isPathValid(n, edges, path)) paths.push_back(path);
            }
        } while (std::next_permutation(order.begin(), order.end()));
        return paths;
    }

    vector<int> getAllForemostTime(const int &n, const vector<Edge> &edges, int ta, int tw, int x) {
        vector<int> time(n + 1, INF);
        time[x] = ta;
        for (const vector<Edge> &path : findAllValidPath(n, edges)) {
            if (path.front().t < ta) continue;
            if (path.back().t + path.back().lambda > tw) continue;
            if (path.front().u != x) continue;
            time[path.back().v] = std::min(time[path.back().v], path.back().t + path.back().lambda);
        }
        return time;
    } 

}
