#include "utility.h"

#include <algorithm>
#include <vector>
using std::vector;

/*
    Algorithm 1
    Receives the number of nodes, the edges in stream presentation, the time interval [ta, tw] and the source x.
        This will compute the foremost time+path from x to all other nodes in O(|V| + |E|)
    Reports the foremost time+path from x to any specified destination in O(path_length) (path length is no greater than |V|)
*/
class Foremost {   
    int source;
    vector<int> time;
    vector<Edge> parent;
public:
    Foremost(const int &n, const vector<Edge> &edges, int ta, int tw, int source) : source(source), time(n + 1), parent(n + 1) {
        fill(time.begin(), time.end(), INF);
        time[source] = ta;
        for (const Edge &e : edges) {
            if (e.t > tw) break; // further edges in streaming presentation won't be used
            if (e.t + e.lambda > tw) continue; // edge arrives too late
            if (e.t < time[e.u]) continue; // edge departs earlier than u's earliest arrived time
            if (e.t + e.lambda < time[e.v]) { // from node u, use the edge to go to node v
                time[e.v] = e.t + e.lambda;
                parent[e.v] = e;
            }
        }
    }
    std::pair<int, vector<Edge>> getForemostPath(int target) {
        if (time[target] == INF) { 
            // raise no path
            return make_pair(INF, vector<Edge>());
        }
        vector<Edge> path;
        for (int node = target; node != source; node = parent[node].u) {
            path.push_back(parent[node]); // the edge leading to u 
        }
        reverse(path.begin(), path.end());
        return make_pair(time[target], path);
    }
    vector<int> getAllForemostTime() {
        return time;
    }
};