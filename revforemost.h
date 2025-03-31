#include "utility.h"

#include <vector>
using std::vector;

/*
    Algorithm 6
    Receives the number of nodes, the edges in stream presentation, the time interval [ta, tw] and the target x.
        This will compute the reverse-foremost time+path from all other nodes to x in O(|V| + |E|)
    Reports the reverse-foremost time+path from any specified origin to x in O(path_length) (path length is no greater than |V|)
*/
class ReverseForemost {   
    int target;
    vector<int> time;
    vector<Edge> parent;
public:
    ReverseForemost(const int &n, const vector<Edge> &edges, int ta, int tw, int target) : target(target), time(n + 1), parent(n + 1) {
        fill(time.begin(), time.end(), -INF);
        time[target] = tw;
        for (int edge_id = (int)edges.size() - 1; edge_id >= 0; edge_id--) {
            const Edge &e = edges[edge_id];
            if (e.t < ta) break; // edge departs too early, further edges in streaming presentation won't be used
            if (e.t + e.lambda > time[e.v]) continue; // edge arrives later than v's latest time allowed
            if (e.t > time[e.u]) { // from node u, can use this edge to go to v (and still be able to reach target)
                time[e.u] = e.t;
                parent[e.u] = e;
            }
        }
    }
    std::pair<int, vector<Edge>> getReverseForemostPath(int source) const {
        if (time[source] == -INF) { 
            // raise no path
            return make_pair(-INF, vector<Edge>());
        }
        vector<Edge> path;
        for (int node = source; node != target; node = parent[node].v) {
            path.push_back(parent[node]); // the edge outgoing from 'node'
        }
        return make_pair(time[source], path);
    }
    vector<int> getAllReverseForemostTime() const {
        return time;
    }
};