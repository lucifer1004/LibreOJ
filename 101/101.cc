#include <climits>
#include <iostream>
#include <queue>
#include <vector>

typedef long long ll;

struct Edge {
  int from, to, capacity, flow;
};

class Dinic {
  int n, s, t, m = 0;
  std::vector<Edge> edges;
  std::vector<std::vector<int>> adj;
  std::vector<int> depth, curr;
  std::vector<bool> vis;

  bool can_extend() {
    vis.assign(n + 1, false);
    std::queue<int> q;
    q.push(s);
    vis[s] = true;
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (int i : adj[u]) {
        Edge e = edges[i];
        if (!vis[e.to] && e.capacity > e.flow) {
          vis[e.to] = true;
          depth[e.to] = depth[u] + 1;
          q.push(e.to);
        }
      }
    }
    return vis[t];
  }

  int extend(int u, int a) {
    if (u == t || a == 0)
      return a;
    int flow = 0;
    int f;
    for (int &i = curr[u]; i < adj[u].size(); ++i) {
      Edge &e = edges[adj[u][i]];
      if (depth[e.to] == depth[u] + 1 &&
          (f = extend(e.to, std::min(a, e.capacity - e.flow))) > 0) {
        e.flow += f;
        edges[adj[u][i] ^ 1].flow -= f;
        flow += f;
        a -= f;
        if (a == 0)
          break;
      }
    }
    return flow;
  }

public:
  Dinic(int n, int s, int t) : n(n), s(s), t(t) {
    adj.resize(n + 1);
    depth.resize(n + 1);
  }

  void add_edge(int u, int v, int capacity) {
    edges.emplace_back(Edge{u, v, capacity, 0});
    edges.emplace_back(Edge{v, u, 0, 0});
    adj[u].emplace_back(m++);
    adj[v].emplace_back(m++);
  }

  ll max_flow() {
    ll flow = 0;
    while (can_extend()) {
      curr.assign(n + 1, 0);
      flow += extend(s, INT_MAX);
    }
    return flow;
  }
};

int main() {
  int n, m, s, t;
  std::cin >> n >> m >> s >> t;
  Dinic dinic(n, s, t);
  for (int i = 0; i < m; ++i) {
    int u, v, capacity;
    std::cin >> u >> v >> capacity;
    dinic.add_edge(u, v, capacity);
  }
  std::cout << dinic.max_flow();
}