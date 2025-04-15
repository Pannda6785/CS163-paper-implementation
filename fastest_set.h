#include "graphUtil.h"

#include <set>
#include <vector>
using std::vector;

/*
    Algorithm 3
    Receives the number of nodes, the edges in stream presentation, the source x, and the time interval [ta, tw].
        This will compute the fastest time+path from x to all other nodes.
    Also reports the fastest time+path from x to any specified destination.
    >> This implementation runs in O(|V|+|E|logmin(S,C)) worst-case, path retrieval is O(path_length*logmin(S,C))
*/
class Fastest {   
    struct Path { // presents a possible path to a node
        int s, a; // starting time and arrival time of the path
        Edge e; // last edge used in the path
        Path() {}
        Path(int s, int a, Edge e) : s(s), a(a), e(e) {}
        bool operator<(const Path &rhs) const {
            return a < rhs.a;
        }
    };
    int source;
    vector<int> time;
    vector<int> arg_a;
    vector<std::set<Path>> L;
    using ite = std::set<Path>::iterator;

    // find the smallest i so that L[i].a > t
    inline ite find(const std::set<Path> &L, int t) const {
        return L.upper_bound(Path(int(), t, Edge()));
    }

    // domination condition: X dominate Y if X.a <= Y.a and X.s >= Y.s
    inline void add(std::set<Path> &L, int s, int a, Edge e) {
        ite i = find(L, a);
        if (i != L.begin() && prev(i)->s >= s) return; // (s, a) is dominated
        if (i != L.begin() && prev(i)->a == a) --i;
        ite j = i;
        while (j != L.end() && j->s <= s) ++j;
        L.insert(L.erase(i, j), Path(s, a, e));
    }

public:
    /* compute the fastest times and means to retrieve the path upon construction of the class instance */
    Fastest(const int &n, const vector<Edge> &edges, int source, int ta, int tw) : source(source), time(n + 1), arg_a(n + 1), L(n + 1) {
        fill(time.begin(), time.end(), INF);
        time[source] = 0;

        for (const Edge &e : edges) {
            if (e.t > tw) break; // break the loop early because further edges in the stream depart too late and won't be used
            if (e.t < ta || e.t + e.lambda > tw) continue; // edge is outside of required time interval
            if (e.v == source) continue; // an edge leading to source won't be used in the computation for any node
            
            if (e.u == source) {
                // since e.t is in sorted order, only need to check the latest span (instantaneous span)
                if (L[e.u].empty() || L[e.u].rbegin()->a != e.t) { 
                     L[e.u].emplace(e.t, e.t, e);
                }
            }

            // find the best path to u that can update v
            ite i = find(L[e.u], e.t);
            if (i == L[e.u].begin()) continue; // all paths to u arrive later than e.t so we can't use the edge e
            int s = prev(i)->s, a = e.t + e.lambda;

            // try to add (s, a) to L[v]
            add(L[e.v], s, a, e);
            
            // update time[v]
            if (a - s < time[e.v]) {
                time[e.v] = a - s;
                arg_a[e.v] = a;
            }
        }
    }
    
    /* return all the fastest times to each vertex */
    vector<int> getAllFastestTime() const {
        return time;
    }
    
    /* returns the fastest time and the path as a list of edges, if there is no available path then time will be INF */
    std::pair<int, vector<Edge>> getFastestPath(int target) const {
        if (time[target] == INF) { // no path
            return make_pair(INF, vector<Edge>());
        }

        vector<Edge> path;
        int t = arg_a[target];
        for (int node = target; node != source; ) {
            ite i = find(L[node], t);
            Edge e = prev(i)->e;
            path.push_back(e);
            t = e.t;
            node = e.u;
        } 
        
        reverse(path.begin(), path.end());
        return make_pair(time[target], path);
    }

};