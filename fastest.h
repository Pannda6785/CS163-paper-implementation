#include "utility.h"

#include <set>
#include <vector>
using std::vector;

/*
    Algorithm 3
    Receives the number of nodes, the edges in stream presentation, the time interval [ta, tw] and the target x.
        This will compute the fastest time+path from x to all other nodes in O(|V| + |E|)
    Reports the fastest time+path from x to any specified destination in O(path_length) (path length is no greater than |V|)
*/
class Fastest {   
    int source;
    vector<int> time;
    vector<Edge> parent;
    
    void compute(const int &n, const vector<Edge> &edges, int ta, int tw, int source) {
        fill(time.begin(), time.end(), INF);
        time[source] = 0;
        vector<std::set<std::pair<int, int>>> L(n + 1); // store spans (pairs of (arrival_time, start_time))

        for (const Edge &e : edges) {
            if (e.t > tw) break; // edge departs too late, future edges won't be used
            if (e.t < ta || e.t + e.lambda > tw) continue; // edge is outside of bounded timespan

            
            if (e.u == source) {
                L[source].insert({e.t, e.t});
            }

            std::set<std::pair<int, int>>::iterator it = L[e.u].lower_bound(std::make_pair(e.t + 1, -1));
            if (it == L[e.u].begin()) continue; // all paths to u arrive at later than t, u cannot be used to update v with this edge
            --it; // now it points to the latest arrival path not later than t (also implies latest starting time)
            
            std::pair<int, int> new_span = std::make_pair(e.t + e.lambda, it->second);
            if (time[e.v] > new_span.first - new_span.second) {
                time[e.v] = new_span.first - new_span.second;
            }
            
            // to update L[v]. by assuming L's are always maintained so that no dominated span (a, s) is present, 
            // we will be inserting new_span into L[e.v] so we first remove all spans dominated by new_span
            while (true) {
                std::set<std::pair<int, int>>::iterator it = L[e.v].lower_bound(std::make_pair(new_span.first, -1)); 
                if (it != L[e.v].end() && it->second <= new_span.second) {
                    L[e.v].erase(it);
                } else {
                    break;
                }
            }
            /* only if the above process doesn't remove anything then it is possible that new_span is dominated by some already present span
               we check and add if new_span is not dominated */ {
                std::set<std::pair<int, int>>::iterator it = L[e.v].lower_bound(std::make_pair(new_span.first + 1, -1)); 
                if (it == L[e.v].begin() || prev(it)->second < new_span.second) { 
                    L[e.v].insert(new_span);
                } 
            }
        }
    }

public:
    Fastest(const int &n, const vector<Edge> &edges, int ta, int tw, int source) : source(source), time(n + 1), parent(n + 1) {
        compute(n, edges, ta, tw, source);
    }
    std::pair<int, vector<Edge>> getFastestPath(int target) {
        if (time[target] == INF) { 
            // raise no path
            return make_pair(INF, vector<Edge>());
        }
        vector<Edge> path;
        for (int node = target; node != source; node = parent[node].u) {
            path.push_back(parent[node]); // the edge leading to 'node'
        }
        reverse(path.begin(), path.end());
        return make_pair(time[target], path);
    }
    vector<int> getAllFastestTime() {
        return time;
    }
};