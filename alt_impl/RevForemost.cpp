#include <bits/stdc++.h>

using namespace std;

const int INF = 2e9;

struct Edge {
    int u, v, lambda, t;
};

class ForemostPathFinder {
public:
    void compute(int n, vector<Edge>& edges, int source, int ta, int tw, vector<int>& t_arrival) {
        // Init arrival times
        t_arrival = vector<int>(n + 1, -INF);
        t_arrival[source] = tw;

        // Process stream
        for (int i = edges.size() - 1; i >= 0; i--) {
            Edge &e = edges[i];
            // Edge not usable within [ta, tw]
            if (e.t < ta) break;
            if (e.t + e.lambda > tw) continue;
    
            if (t_arrival[e.v] >= e.t + e.lambda) {
                t_arrival[e.u] = max(t_arrival[e.u], e.t);
            }
        }
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
    ForemostPathFinder path;
    path.compute(n, edges, x, ta, tw, t_arrival);

    cout << "ReverseForemost\n";
    for (int v = 1; v <= n; v++) {
        if (t_arrival[v] == -INF) cout << "-1 ";
        else cout << t_arrival[v] << " ";
    }
    cout << "\n";

    return 0;
}