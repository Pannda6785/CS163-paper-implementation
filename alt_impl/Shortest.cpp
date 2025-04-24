#include <bits/stdc++.h>

using namespace std;

const int INF = 2e9;

struct Edge {
    int u, v, lambda, t;
};

class ShortestPathFinder {
private:
    struct Path {
        int d, a;
        Edge e;
        Path() {}
        Path(int d, int a, Edge e) : d(d), a(a), e(e) {}
        bool operator<(const Path& rhs) const {
            return a < rhs.a;
        }
    };

    using PathSet = set<Path>;
    using Iter = PathSet::iterator;

    Iter find(const PathSet& L, int t) const {
        return L.upper_bound(Path(0, t, Edge()));
    }

    void add(PathSet& L, int d, int a, const Edge& e) {
        Iter it = find(L, a);
        if (it != L.begin() && prev(it)->d <= d) return; // Dominated
        if (it != L.begin() && prev(it)->a == a) --it;
        Iter j = it;
        while (j != L.end() && j->d >= d) ++j;
        L.insert(L.erase(it, j), Path(d, a, e));
    }

public:
    void compute(int n, vector<Edge>& edges, int source, int ta, int tw, vector<int>& shortest_time) {
        vector<PathSet> L(n + 1);
        vector<int> arg_a(n + 1, -1);
        shortest_time.assign(n + 1, INF);
        shortest_time[source] = 0;

        // Sort edges by departure time
        sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
            return a.t < b.t;
        });

        for (const Edge& e : edges) {
            if (e.t > tw) break;
            if (e.t < ta || e.t + e.lambda > tw) continue;
            if (e.v == source) continue;

            // Direct path from source
            if (e.u == source) {
                if (L[e.u].empty()) {
                    L[e.u].emplace(0, e.t, e);
                }
            }

            Iter it = find(L[e.u], e.t);
            if (it == L[e.u].begin()) continue;

            int d = prev(it)->d + e.lambda;
            int a = e.t + e.lambda;

            add(L[e.v], d, a, e);

            if (d < shortest_time[e.v]) {
                shortest_time[e.v] = d;
                arg_a[e.v] = a;
            }
        }

        for (int i = 1; i <= n; ++i)
            if (shortest_time[i] == INF) shortest_time[i] = -1;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    
    int n, m;
    cin >> n >> m;

    int x, ta, tw;
    cin >> x >> ta >> tw;

    vector<Edge> edges(m);
    for (int i = 0; i < m; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].lambda >> edges[i].t;
    }

    vector<int> t_arrival;
    ShortestPathFinder path;
    path.compute(n, edges, x, ta, tw, t_arrival);

    cout << "Fastest\n";
    for (int v = 1; v <= n; v++) {
        if (t_arrival[v] == -INF) cout << "-1 ";
        else cout << t_arrival[v] << " ";
    }
    cout << "\n";

    return 0;
}