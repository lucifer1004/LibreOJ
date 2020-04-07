#include <climits>
#include <cstdio>
#include <cstring>
#include <functional>
#include <queue>
#include <vector>
#define MAXN 1205
#define MAXM 120005
#define INF 0x3f3f3f3f

int capacity[MAXN][MAXN], flow[MAXN][MAXN];
int height[MAXM], excess[MAXM], gap[MAXM];
bool vis[MAXM];

struct cmp {
  bool operator()(int a, int b) const { return height[a] < height[b]; }
};

class HLPP {
  int n, s, t;
  std::priority_queue<int, std::vector<int>, cmp> pq;

  void push(int u, int v) {
    int d = std::min(excess[u], capacity[u][v] - flow[u][v]);
    flow[u][v] += d;
    flow[v][u] -= d;
    excess[u] -= d;
    excess[v] += d;
    if (v != s && v != t && !vis[v]) {
      vis[v] = true;
      pq.push(v);
    }
  }

  void relabel(int u) {
    int d = INT_MAX;
    for (int v = 1; v <= n; ++v) {
      if (capacity[u][v] > flow[u][v])
        d = std::min(d, height[v]);
    }
    if (d != INT_MAX)
      height[u] = d + 1;
  }

  bool bfs_init() {
    memset(height, 0x3f, sizeof(height));
    height[t] = 0;
    std::queue<int> q;
    q.push(t);
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (int i = 1; i <= n; ++i)
        if (capacity[i][u] > 0 && height[i] == INF) {
          height[i] = height[u] + 1;
          q.push(i);
        }
    }
    return height[s] != INF;
  }

public:
  HLPP(int n, int s, int t) : n(n), s(s), t(t) {
    memset(capacity, 0, sizeof(capacity));
    memset(flow, 0, sizeof(flow));
    memset(excess, 0, sizeof(excess));
    memset(vis, 0, sizeof(vis));
    memset(gap, 0, sizeof(gap));
  }

  void add_edge(int u, int v, int cap) { capacity[u][v] = cap; }

  int max_flow() {
    if (!bfs_init())
      return 0;
    excess[s] = INT_MAX;
    height[s] = n;
    for (int i = 1; i <= n; ++i)
      if (height[i] != INT_MAX)
        gap[height[i]]++;
    for (int v = 1; v <= n; ++v) {
      if (!capacity[s][v])
        continue;
      push(s, v);
    }
    while (!pq.empty()) {
      int u = pq.top();
      pq.pop();
      vis[u] = false;
      while (true) {
        for (int v = 1; v <= n && excess[u] > 0; ++v) {
          if (capacity[u][v] > flow[u][v] && height[u] == height[v] + 1)
            push(u, v);
        }
        if (excess[u] == 0)
          break;
        if (--gap[height[u]] == 0) {
          for (int i = 1; i <= n; ++i)
            if (i != s && i != t && height[i] > height[u] && height[i] < n + 1)
              height[i] = n + 1;
        }
        relabel(u);
        gap[height[u]]++;
      }
    }

    return excess[t];
  }
};

int main() {
  int n, m, s, t;
  scanf("%d%d%d%d", &n, &m, &s, &t);
  HLPP hlpp(n, s, t);
  for (int i = 0; i < m; ++i) {
    int u, v, capacity;
    scanf("%d%d%d", &u, &v, &capacity);
    hlpp.add_edge(u, v, capacity);
  }
  printf("%d", hlpp.max_flow());
}