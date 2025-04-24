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
        t_arrival = vector<int>(n + 1, INF);
        t_arrival[source] = ta;

        // Process stream
        for (Edge &e : edges) {
            // Edge not usable within [ta, tw]
            if (e.t > tw) break;
            if (e.t < ta || e.t + e.lambda > tw) continue;

            if (t_arrival[e.u] <= e.t) {
                t_arrival[e.v] = min(t_arrival[e.v], e.t + e.lambda);
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

    cout << "Foremost\n";
    for (int v = 1; v <= n; v++) {
        if (t_arrival[v] == INF) cout << "-1 ";
        else cout << t_arrival[v] << " ";
    }
    cout << "\n";

    return 0;
}