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
    vector<vector<std::pair<int, int>>> L; // store spans (pairs of (arrival_time, start_time))
    
    void compute(const int &n, const vector<Edge> &edges, int ta, int tw, int source) {
        fill(time.begin(), time.end(), INF);
        time[source] = 0;

        for (const Edge &e : edges) {
            if (e.t > tw) break; // edge departs too late, future edges won't be used
            if (e.t < ta || e.t + e.lambda > tw) continue; // edge is outside of bounded timespan

            std::pair<int, int> v = {e.t, e.t};
            auto it = lower_bound(L[e.u].begin(), L[e.u].end(), v, [&](const auto &x, const auto &y) { return x.first < y.first; });
            if (e.u == source) {
                if (it == L[e.u].end() || it->first != e.t) {
                    it = L[source].insert(it, v);
                    it++;
                }
            }
            
            if (it != L[e.u].end() && it->first == e.t) ++it;
            if (it == L[e.u].begin()) continue; // all paths to u arrive at later than t, u cannot be used to update v with this edge
            --it; // now it points to the latest arrival path not later than t (also implies latest starting time)
            
            std::pair<int, int> new_span = std::make_pair(e.t + e.lambda, it->second);
            if (time[e.v] > new_span.first - new_span.second) {
                time[e.v] = new_span.first - new_span.second;
            }
            
            /*
            to update L[v]. by assuming L's are always maintained so that no dominated span (a, s) is present, 
            we will be inserting new_span into L[e.v] so we first remove all spans dominated by new_span
            */
            it = lower_bound(L[e.v].begin(), L[e.v].end(), new_span, [&](const auto &x, const auto &y) { return x.first < y.first; }); 
            // auto endit = lower_bound(it, L[e.v].end(), new_span, [&](const auto &x, const auto &y) { return x.second <= y.second; }); 
            auto endit = it;
            while (endit != L[e.v].end() && endit->second <= new_span.second) ++endit;
            L[e.v].erase(it, endit);
            it = endit;
            /* only if the above process doesn't remove anything then it is possible that new_span is dominated by some already present span
               we check and add if new_span is not dominated */ 
            if (it != L[e.v].end() && it != L[e.v].begin() && it->first > new_span.first) it = prev(it); 
            if (it == L[e.v].begin() || prev(it)->second < new_span.second) { 
                auto pos = lower_bound(L[e.v].begin(), L[e.v].end(), new_span);
                L[e.v].insert(pos, new_span);
            } 
            
        }
    }

public:
    Fastest(const int &n, const vector<Edge> &edges, int ta, int tw, int source) : source(source), time(n + 1), parent(n + 1), L(n + 1) {
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