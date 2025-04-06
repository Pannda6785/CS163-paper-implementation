#include "utility.h"

#include <vector>
using std::vector;

/*
    Algorithm 3
    Receives the number of nodes, the edges in stream presentation, the time interval [ta, tw] and the source x.
        This will compute the fastest time+path from x to all other nodes in O(|V| + |E|)
    Reports the fastest time+path from x to any specified destination in O(path_length*log(outdegx)) (path length is no greater than |V|)
*/
class Fastest {   
    int source;
    vector<int> time;
    vector<int> arg_a;
    vector<vector<std::tuple<int, int, Edge>>> L; // store tuples of (start_time, arrival_time, edge lead to this)

    inline int find(const vector<std::tuple<int, int, Edge>> &L, int t) { // find the last i with L[i].a <= t, or -1 if none
        /* binary search */
        // int l = -1, r = L.size();
        // while (l + 1 < r) {
        //     int m = (l + r) / 2;
        //     if (std::get<1>(L[m]) <= t) {
        //         l = m;
        //     } else {
        //         r = m;
        //     }
        // }
        // return l;

        /* brute */
        int i = (int)L.size() - 1;
        for (; i >= 0 && std::get<1>(L[i]) > t; --i); // if arrival time is still too great then reduce it
        return i;
    }

    inline void add(vector<std::tuple<int, int, Edge>> &L, int s, int a, Edge e) {
        int i = find(L, a);
        if (i != -1 && std::get<0>(L[i]) >= s) return; // dominated
        i++;
        int j = i;
        for (; i < L.size() && std::get<1>(L[0]) <= s; ++i);
        L.emplace(L.erase(L.begin() + j, L.begin() + i), s, a, e);
    }

public:
    Fastest(const int &n, const vector<Edge> &edges, int ta, int tw, int source) : source(source), time(n + 1), arg_a(n + 1), L(n + 1) {
        fill(time.begin(), time.end(), INF);
        time[source] = 0;

        for (const Edge &e : edges) {
            if (e.t > tw) break; // edge departs too late, future edges won't be used
            if (e.t < ta || e.t + e.lambda > tw) continue; // edge is outside of bounded timespan
            if (e.v == source) continue; // an edge leading to source won't be used in the computation for any node
            
            if (e.u == source) {
                // since e.t is in sorted order, only need to check the latest span (instantaneous span)
                if (L[e.u].empty() || std::get<1>(L[e.u].back()) != e.t) { 
                     L[e.u].emplace_back(e.t, e.t, e);
                }
            }

            // find the best span from u
            int i = find(L[e.u], e.t);
            if (i == -1) continue; // all paths to u arrive later then e.t
            int s = std::get<0>(L[e.u][i]), a = e.t + e.lambda;

            // (try to) add to L[e.v]
            add(L[e.v], s, a, e);
            
            if (a - s < time[e.v]) {
                time[e.v] = a - s;
                arg_a[e.v] = a;
            }
        }
    }
    
    std::pair<int, vector<Edge>> getFastestPath(int target) {
        if (time[target] == INF) { 
            // raise no path
            return make_pair(INF, vector<Edge>());
        }
        vector<Edge> path;
        int t = arg_a[target];
        for (int node = target; node != source; ) {
            int i = find(L[node], t);
            path.push_back(std::get<2>(L[node][i]));
            t = std::get<2>(L[node][i]).t;
            node = std::get<2>(L[node][i]).u;
        } 

        reverse(path.begin(), path.end());
        return make_pair(time[target], path);
    }

    vector<int> getAllFastestTime() {
        return time;
    }
};