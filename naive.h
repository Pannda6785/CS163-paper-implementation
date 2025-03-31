#include "utility.h"

#include <vector>
using std::vector;

namespace naive {
    
    vector<vector<Edge>> findAllValidPath(const int &n, const vector<Edge> &edges, int ta = 0, int tw = INF, int source = -1, int target = -1) { // does not generate empty path
        vector<int> order(edges.size());
        iota(order.begin(), order.end(), 0);
        vector<vector<Edge>> paths;
        do {
            vector<Edge> path;
            for (int i : order) {
                path.push_back(edges[i]);
                if (isPathValid(n, edges, path, ta, tw, source, target)) paths.push_back(path);
            }
        } while (std::next_permutation(order.begin(), order.end()));
        return paths;
    }

    vector<int> getAllForemostTime(const int &n, const vector<Edge> &edges, int ta, int tw, int x) {
        vector<int> time(n + 1, INF);
        time[x] = ta;
        for (const vector<Edge> &path : findAllValidPath(n, edges, ta, tw, x, -1)) {
            time[path.back().v] = std::min(time[path.back().v], path.back().t + path.back().lambda);
        }
        return time;
    } 

    vector<int> getAllReverseForemostTime(const int &n, const vector<Edge> &edges, int ta, int tw, int x) {
        vector<int> time(n + 1, -INF);
        time[x] = tw;
        for (const vector<Edge> &path : findAllValidPath(n, edges, ta, tw, -1, x)) {
            time[path.front().u] = std::max(time[path.front().u], path.front().t);
        }
        return time;
    } 

    vector<int> getAllFastestTime(const int &n, const vector<Edge> &edges, int ta, int tw, int x) {
        vector<int> time(n + 1, INF);
        time[x] = 0;
        for (const vector<Edge> &path : findAllValidPath(n, edges, ta, tw, x, -1)) {
            time[path.back().v] = std::min(time[path.back().v], path.back().t + path.back().lambda - path.front().t);
        }
        return time;
    } 

}
