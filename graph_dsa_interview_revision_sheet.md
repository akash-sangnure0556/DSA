# Graph DSA Interview Revision Sheet (C++)

A practical graph revision sheet for Easy/Medium coding interviews.
Focus first on traversal, connected components, grids, cycle detection,
topological sort, shortest paths, and DSU.

------------------------------------------------------------------------

## 1. Graph fundamentals

### Important terms

-   **Vertex/node:** an entity in the graph.
-   **Edge:** connection between two vertices.
-   **Directed graph:** edge `u -> v` does not imply `v -> u`.
-   **Undirected graph:** edge `{u,v}` works both ways.
-   **Weighted graph:** edges carry a cost/weight.
-   **Degree (undirected):** number of edges touching a vertex.
-   **In-degree / out-degree (directed):** incoming / outgoing edges.
-   **Connected component:** a maximal set of mutually reachable
    vertices in an undirected graph.
-   **Path:** sequence of vertices connected by edges; **cycle:** path
    that returns to its starting vertex.
-   **DAG:** directed acyclic graph; topological ordering exists only
    for a DAG.

### Adjacency list --- usually best for interviews

For an undirected graph:

``` cpp
int V, E;
cin >> V >> E;
vector<vector<int>> adj(V);

for (int i = 0; i < E; ++i) {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u); // omit this line for directed graphs
}
```

-   Space: `O(V + E)`.
-   Iterating neighbours is efficient; usually preferred for sparse
    graphs.
-   For a weighted graph use `vector<vector<pair<int,int>>> adj(V);` and
    store `{neighbor, weight}`.

### Adjacency matrix

``` cpp
vector<vector<int>> adj(V, vector<int>(V, 0));
for (int i = 0; i < E; ++i) {
    int u, v;
    cin >> u >> v;
    adj[u][v] = 1;
    adj[v][u] = 1; // omit for directed graphs
}
```

-   Space: `O(V^2)`; checking whether an edge exists is `O(1)`.
-   Correct syntax is `vector<vector<int>> adj(V, vector<int>(V, 0));`.
-   Do not write `adj[u].push_back[v]`; the syntax is
    `adj[u].push_back(v)`.

### BFS traversal

``` cpp
vector<int> bfs(int start, const vector<vector<int>>& adj) {
    int n = adj.size();
    vector<int> vis(n, 0), order;
    queue<int> q;
    vis[start] = 1;             // mark when enqueuing
    q.push(start);

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        order.push_back(node);

        for (int nei : adj[node]) {
            if (!vis[nei]) {
                vis[nei] = 1;
                q.push(nei);
            }
        }
    }
    return order;
}
```

-   BFS explores level by level and finds shortest path in an
    **unweighted** graph.
-   Time: `O(V + E)` with an adjacency list; space: `O(V)`.
-   This traverses only the component containing `start`.

### DFS traversal

``` cpp
void dfs(int node, const vector<vector<int>>& adj,
         vector<int>& vis, vector<int>& order) {
    vis[node] = 1;
    order.push_back(node);

    for (int nei : adj[node]) {
        if (!vis[nei]) dfs(nei, adj, vis, order);
    }
}
```

Call it with `dfs(start, adj, vis, order)`. Recursive DFS can overflow
the call stack on very deep graphs; iterative DFS is an alternative.

### Disconnected graphs / connected components

``` cpp
int components = 0;
vector<int> vis(V, 0);

for (int i = 0; i < V; ++i) {
    if (!vis[i]) {
        ++components;
        dfs(i, adj, vis, order);
    }
}
```

**Key idea:** starting DFS/BFS from vertex `0` does not visit
disconnected components. Use an outer loop over every vertex when the
whole graph must be covered.

------------------------------------------------------------------------

## 2. Grid graph pattern

Use four directions for up/down/left/right:

``` cpp
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

for (int k = 0; k < 4; ++k) {
    int nr = r + dr[k];
    int nc = c + dc[k];

    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
        // neighbour is inside the grid
    }
}
```

For eight directions, use all combinations of `dr, dc` in `{-1,0,1}`
except `(0,0)`.

**Grid checklist** 1. Get `rows = grid.size()`, `cols = grid[0].size()`
(if non-empty). 2. Check bounds **before** indexing a neighbour. 3. Mark
a cell visited when adding/entering it, to avoid repeated work. 4.
Decide whether to use a separate `vis` matrix or modify the grid in
place.

### LeetCode 733 --- Flood Fill (Easy)

``` cpp
class Solution {
    void paint(vector<vector<int>>& image, int r, int c, int oldColor,
               int newColor) {
        int m = image.size(), n = image[0].size();
        if (r < 0 || r >= m || c < 0 || c >= n) return;
        if (image[r][c] != oldColor) return;

        image[r][c] = newColor; // marking in-place prevents revisits
        paint(image, r - 1, c, oldColor, newColor);
        paint(image, r + 1, c, oldColor, newColor);
        paint(image, r, c - 1, oldColor, newColor);
        paint(image, r, c + 1, oldColor, newColor);
    }

public:
    vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int color) {
        if (image[sr][sc] == color) return image;
        paint(image, sr, sc, image[sr][sc], color);
        return image;
    }
};
```

**4-step solution** 1. Save the starting pixel's original colour. 2. If
original colour equals the new colour, return immediately. 3. DFS to
four neighbours; stop when out of bounds or the pixel differs from the
original colour. 4. Recolour each accepted pixel before recursing. Time
`O(rows*cols)`, recursion space up to `O(rows*cols)`.

### LeetCode 200 --- Number of Islands (Medium)

``` cpp
class Solution {
    void dfs(vector<vector<char>>& g, int r, int c) {
        int m = g.size(), n = g[0].size();
        if (r < 0 || r >= m || c < 0 || c >= n || g[r][c] != '1') return;
        g[r][c] = '0';
        dfs(g, r - 1, c); dfs(g, r + 1, c);
        dfs(g, r, c - 1); dfs(g, r, c + 1);
    }
public:
    int numIslands(vector<vector<char>>& grid) {
        if (grid.empty()) return 0;
        int count = 0;
        for (int r = 0; r < (int)grid.size(); ++r)
            for (int c = 0; c < (int)grid[0].size(); ++c)
                if (grid[r][c] == '1') {
                    ++count;
                    dfs(grid, r, c);
                }
        return count;
    }
};
```

**4-step solution** 1. Scan every cell in the grid. 2. When a land cell
`'1'` is found, increment the island count. 3. DFS/BFS from it and mark
all connected land as water/visited. 4. Each island is counted once.
Time `O(rows*cols)`; this version modifies the grid.

### LeetCode 994 --- Rotting Oranges (Medium)

``` cpp
class Solution {
public:
    int orangesRotting(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size(), fresh = 0, minutes = 0;
        queue<pair<int,int>> q;
        for (int r = 0; r < m; ++r)
            for (int c = 0; c < n; ++c) {
                if (grid[r][c] == 2) q.push({r,c});
                else if (grid[r][c] == 1) ++fresh;
            }

        int dr[] = {-1,1,0,0}, dc[] = {0,0,-1,1};
        while (!q.empty() && fresh > 0) {
            int sz = q.size();
            ++minutes;
            while (sz--) {
                auto [r,c] = q.front(); q.pop();
                for (int k = 0; k < 4; ++k) {
                    int nr = r + dr[k], nc = c + dc[k];
                    if (nr >= 0 && nr < m && nc >= 0 && nc < n &&
                        grid[nr][nc] == 1) {
                        grid[nr][nc] = 2;
                        --fresh;
                        q.push({nr,nc});
                    }
                }
            }
        }
        return fresh == 0 ? minutes : -1;
    }
};
```

**4-step solution** 1. Put every initially rotten orange in the queue
and count fresh oranges. 2. Process the queue one level at a time; one
level represents one minute. 3. Rot adjacent fresh oranges, mark them
immediately, decrement `fresh`, and enqueue them. 4. Return minutes if
`fresh == 0`, otherwise `-1`. Time `O(rows*cols)`.

------------------------------------------------------------------------

## 3. Core graph interview problems

### LeetCode 1971 --- Find if Path Exists in Graph (Easy)

``` cpp
class Solution {
public:
    bool validPath(int n, vector<vector<int>>& edges, int source, int destination) {
        vector<vector<int>> adj(n);
        for (auto& e : edges) {
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
        }

        vector<int> vis(n, 0);
        queue<int> q;
        q.push(source);
        vis[source] = 1;

        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (u == destination) return true;
            for (int v : adj[u]) {
                if (!vis[v]) {
                    vis[v] = 1;
                    q.push(v);
                }
            }
        }
        return false;
    }
};
```

**4-step solution** 1. Build an undirected adjacency list from `edges`.
2. Start BFS from `source`, marking vertices when enqueued. 3. If
`destination` is reached, return `true`. 4. If the queue empties first,
return `false`. Time `O(V+E)`.

### Number of Provinces --- LeetCode 547 (Medium)

``` cpp
class Solution {
    void dfs(int u, vector<vector<int>>& g, vector<int>& vis) {
        vis[u] = 1;
        for (int v = 0; v < (int)g.size(); ++v)
            if (g[u][v] == 1 && !vis[v]) dfs(v, g, vis);
    }
public:
    int findCircleNum(vector<vector<int>>& isConnected) {
        int n = isConnected.size(), provinces = 0;
        vector<int> vis(n, 0);
        for (int i = 0; i < n; ++i) {
            if (!vis[i]) {
                ++provinces;
                dfs(i, isConnected, vis);
            }
        }
        return provinces;
    }
};
```

**4-step solution** 1. `isConnected` is already an adjacency matrix; `1`
means a direct connection. 2. Iterate over every city; if unvisited, it
begins a new province. 3. Increment the answer and DFS through all
cities reachable from that city. 4. Return the number of DFS starts.
Time `O(V^2)` for the matrix.

### LeetCode 133 --- Clone Graph (Medium)

``` cpp
/* LeetCode provides Node with val and vector<Node*> neighbors. */
class Solution {
    unordered_map<Node*, Node*> cloned;

    Node* dfs(Node* node) {
        if (!node) return nullptr;
        if (cloned.count(node)) return cloned[node];

        Node* copy = new Node(node->val);
        cloned[node] = copy; // store before visiting neighbours; handles cycles
        for (Node* nei : node->neighbors)
            copy->neighbors.push_back(dfs(nei));
        return copy;
    }
public:
    Node* cloneGraph(Node* node) {
        cloned.clear();
        return dfs(node);
    }
};
```

**4-step solution** 1. Map each original node pointer to its clone
pointer. 2. If a node already has a clone, return it; this handles
cycles and shared neighbours. 3. Create and store the clone **before**
recursively cloning its neighbours. 4. Connect the cloned neighbours and
return the clone. Time `O(V+E)`.

### Undirected cycle detection --- DFS pattern

``` cpp
bool hasCycleDFS(int u, int parent, vector<vector<int>>& adj, vector<int>& vis) {
    vis[u] = 1;
    for (int v : adj[u]) {
        if (!vis[v]) {
            if (hasCycleDFS(v, u, adj, vis)) return true;
        } else if (v != parent) {
            return true;
        }
    }
    return false;
}
// Call for every unvisited vertex: hasCycleDFS(i, -1, adj, vis)
```

**4-step solution** 1. DFS from each unvisited vertex so disconnected
components are covered. 2. Pass the parent vertex into each DFS call. 3.
An unvisited neighbour is explored recursively. 4. A visited neighbour
other than the parent indicates a cycle. For multigraphs with parallel
edges, use edge IDs or a more careful implementation.

### Directed cycle detection --- DFS colouring pattern

``` cpp
bool directedCycle(int u, vector<vector<int>>& adj,
                   vector<int>& state) {
    state[u] = 1; // 1 = in current recursion path
    for (int v : adj[u]) {
        if (state[v] == 1) return true;
        if (state[v] == 0 && directedCycle(v, adj, state)) return true;
    }
    state[u] = 2; // 2 = fully processed
    return false;
}
// state: 0 = unvisited, 1 = active, 2 = completed
// Call from every vertex with state[i] == 0.
```

**4-step solution** 1. Use three states: `0` unvisited, `1` active in
current DFS path, `2` finished. 2. Visit outgoing neighbours recursively
when their state is `0`. 3. An edge to state `1` goes back into the
current path, so a cycle exists. 4. Mark a node state `2` after all
neighbours finish. Time `O(V+E)`.

### LeetCode 785 --- Is Graph Bipartite? (Medium)

``` cpp
class Solution {
public:
    bool isBipartite(vector<vector<int>>& graph) {
        int n = graph.size();
        vector<int> color(n, -1);
        for (int s = 0; s < n; ++s) {
            if (color[s] != -1) continue;
            queue<int> q;
            q.push(s);
            color[s] = 0;
            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (int v : graph[u]) {
                    if (color[v] == -1) {
                        color[v] = 1 - color[u];
                        q.push(v);
                    } else if (color[v] == color[u]) return false;
                }
            }
        }
        return true;
    }
};
```

**4-step solution** 1. Start BFS from each uncoloured component and
assign its start colour `0`. 2. Every neighbour must have the opposite
colour. 3. If an edge joins two vertices of the same colour, return
`false`. 4. If all components are coloured consistently, return `true`.
Time `O(V+E)`.

### Topological sort --- Kahn's algorithm (BFS)

``` cpp
vector<int> topoSort(int V, vector<vector<int>>& adj) {
    vector<int> indegree(V, 0);
    for (int u = 0; u < V; ++u)
        for (int v : adj[u]) ++indegree[v];

    queue<int> q;
    for (int i = 0; i < V; ++i)
        if (indegree[i] == 0) q.push(i);

    vector<int> order;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (int v : adj[u])
            if (--indegree[v] == 0) q.push(v);
    }
    if ((int)order.size() != V) return {}; // cycle; no topological ordering
    return order;
}
```

**4-step solution** 1. Count incoming edges (`indegree`) for every
vertex. 2. Enqueue all vertices with indegree zero. 3. Pop a vertex,
append it to the order, and decrement its neighbours' indegrees. 4. If
all vertices were processed, the order is valid; fewer than `V` means a
cycle. Time `O(V+E)`.

### LeetCode 207 --- Course Schedule (Medium)

``` cpp
class Solution {
public:
    bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
        vector<vector<int>> adj(numCourses);
        vector<int> indegree(numCourses, 0);
        for (auto& p : prerequisites) {
            int course = p[0], pre = p[1];
            adj[pre].push_back(course);
            ++indegree[course];
        }

        queue<int> q;
        for (int i = 0; i < numCourses; ++i)
            if (indegree[i] == 0) q.push(i);

        int done = 0;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            ++done;
            for (int v : adj[u])
                if (--indegree[v] == 0) q.push(v);
        }
        return done == numCourses;
    }
};
```

**4-step solution** 1. Treat each prerequisite as a directed edge
`pre -> course`. 2. Calculate indegrees and enqueue courses that have no
prerequisites. 3. Use Kahn's algorithm to remove courses and unlock
dependent courses. 4. If all courses are processed, return `true`;
otherwise a cycle blocks completion. Time `O(V+E)`.

### Shortest path in an unweighted graph --- BFS template

``` cpp
vector<int> shortestUnweighted(int n, vector<vector<int>>& adj, int src) {
    vector<int> dist(n, -1);
    queue<int> q;
    q.push(src);
    dist[src] = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    return dist;
}
```

**4-step solution** 1. Initialise every distance to `-1` (unreachable)
and source distance to `0`. 2. BFS visits vertices in increasing
distance from the source. 3. The first time a neighbour is discovered,
set `dist[nei] = dist[u] + 1`. 4. Return distances; `-1` means
unreachable. Time `O(V+E)`.

### Dijkstra --- shortest paths with non-negative weights

``` cpp
vector<long long> dijkstra(int n,
    vector<vector<pair<int,int>>>& adj, int src) {
    const long long INF = LLONG_MAX / 4;
    vector<long long> dist(n, INF);
    priority_queue<pair<long long,int>,
        vector<pair<long long,int>>,
        greater<pair<long long,int>>> pq;

    dist[src] = 0;
    pq.push({0, src});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d != dist[u]) continue; // stale heap entry
        for (auto [v, w] : adj[u]) {
            if (d + w < dist[v]) {
                dist[v] = d + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}
```

**4-step solution** 1. Set source distance to zero and all others to
infinity; push `{distance,node}` into a min-heap. 2. Pop the smallest
distance; skip it if it is stale. 3. Relax every edge: if `d + weight`
improves a neighbour, update and push it. 4. Return distances. Use only
for non-negative edge weights; typical complexity `O((V+E) log V)`.

### Disjoint Set Union (DSU / Union-Find)

Useful for connectivity queries, cycle detection in undirected graphs,
and Kruskal's MST.

``` cpp
class DSU {
    vector<int> parent, sz;
public:
    DSU(int n) : parent(n), sz(n, 1) {
        iota(parent.begin(), parent.end(), 0);
    }
    int find(int x) {
        if (parent[x] == x) return x;
        return parent[x] = find(parent[x]); // path compression
    }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        parent[b] = a;
        sz[a] += sz[b];
        return true;
    }
};
```

**4-step solution** 1. Every vertex starts as its own set. 2. `find(x)`
returns the representative and compresses the path. 3. `unite(a,b)`
joins different sets by size; returns `false` if already connected. 4.
With path compression and union by size, operations are almost constant
amortised time.

------------------------------------------------------------------------

## 4. More high-value Easy/Medium graph questions to practise

  --------------------------------------------------------------------------
  LeetCode         Problem                          Level Core pattern
  ---------------- ---------------- --------------------- ------------------
  1971             Find if Path                      Easy BFS/DFS
                   Exists in Graph                        

  733              Flood Fill                        Easy Grid DFS/BFS

  547              Number of                       Medium Components /
                   Provinces                              matrix DFS

  133              Clone Graph                     Medium DFS + map

  200              Number of                       Medium Grid components
                   Islands                                

  994              Rotting Oranges                 Medium Multi-source BFS

  695              Max Area of                     Medium Grid DFS + area
                   Island                                 

  463              Island Perimeter                  Easy Grid counting

  841              Keys and Rooms                  Medium Reachability
                                                          DFS/BFS

  797              All Paths From                  Medium DAG
                   Source to Target                       DFS/backtracking

  785              Is Graph                        Medium BFS two-colouring
                   Bipartite?                             

  207              Course Schedule                 Medium Directed cycle /
                                                          topo

  210              Course Schedule                 Medium Topological
                   II                                     ordering

  802              Find Eventual                   Medium Reverse graph /
                   Safe States                            DFS states

  1091             Shortest Path in                Medium Grid BFS, 8
                   Binary Matrix                          directions

  127              Word Ladder            Hard (optional) BFS on implicit
                                                          graph

  743              Network Delay                   Medium Dijkstra
                   Time                                   

  684              Redundant                       Medium DSU / cycle
                   Connection                             

  1584             Min Cost to                     Medium MST / Prim or
                   Connect All                            Kruskal
                   Points                                 

  417              Pacific Atlantic                Medium Reverse
                   Water Flow                             multi-source
                                                          DFS/BFS
  --------------------------------------------------------------------------

### Four-line approach reminders for additional problems

-   **695 Max Area of Island:** scan cells; when land is found, DFS and
    count cells; mark visited; keep maximum area.
-   **463 Island Perimeter:** for each land cell add 4; subtract 2 for
    each land neighbour to the right and below (or count exposed sides).
-   **841 Keys and Rooms:** DFS/BFS from room 0 using keys; return true
    only if every room was visited.
-   **797 All Paths:** DFS from node 0; append node to current path;
    when reaching target, save path; backtrack after exploring each
    branch.
-   **210 Course Schedule II:** use Kahn's algorithm; return the
    produced order if its size is `numCourses`; otherwise return empty
    vector.
-   **802 Eventual Safe States:** reverse edges and process terminal
    nodes using outdegree/Kahn's algorithm, or use three-state DFS to
    detect cycles.
-   **1091 Shortest Path in Binary Matrix:** if start/end blocked return
    `-1`; BFS with 8 directions; mark visited on enqueue; return
    distance at target.
-   **684 Redundant Connection:** process edges with DSU; first edge
    whose endpoints already share a root is redundant.
-   **1584 Min Cost to Connect All Points:** model Manhattan distances
    as weighted edges; use Prim's algorithm to avoid storing all edges,
    or Kruskal for smaller inputs.
-   **417 Pacific Atlantic Water Flow:** run reverse DFS/BFS from each
    ocean's border; cells reached by both searches are the answer.

------------------------------------------------------------------------

## 5. Interview traps and common mistakes

1.  **Visited timing:** mark a node visited when pushing/enqueuing it,
    not only when popping it.
2.  **Disconnected graph:** a traversal from one start node does not
    cover every component.
3.  **Directed vs undirected:** add the reverse edge only for undirected
    graphs.
4.  **BFS shortest path:** valid for unweighted/equal-weight edges, not
    arbitrary weighted graphs.
5.  **Dijkstra:** does not work correctly with negative edge weights;
    consider Bellman--Ford for negative weights.
6.  **Topological sort:** only applies to directed acyclic graphs; check
    `order.size() == V` for a cycle using Kahn's algorithm.
7.  **Grid bounds:** validate row and column before indexing.
8.  **Flood fill:** if `oldColor == newColor`, return immediately or
    recursion may never stop.
9.  **Clone graph:** save the clone in the map before visiting
    neighbours, or cycles can cause infinite recursion.
10. **Matrix vs list:** adjacency matrix uses `O(V^2)` space; adjacency
    list uses `O(V+E)`.
11. **Indexing:** confirm whether vertices are labelled `0..V-1` or
    `1..V`; size arrays accordingly.
12. **Complexity:** adjacency-list BFS/DFS is `O(V+E)`; matrix traversal
    often costs `O(V^2)`.

## 6. Complexity cheat sheet

  Algorithm                                             Time   Extra space (typical)
  -------------------------------- ------------------------- -----------------------
  Build adjacency list                              `O(V+E)`                `O(V+E)`
  BFS / DFS (adjacency list)                        `O(V+E)`                  `O(V)`
  BFS / DFS (adjacency matrix)                      `O(V^2)`                  `O(V)`
  Grid DFS/BFS                                      `O(R*C)`     `O(R*C)` worst case
  Topological sort                                  `O(V+E)`                  `O(V)`
  Unweighted shortest path (BFS)                    `O(V+E)`                  `O(V)`
  Dijkstra with min-heap                    `O((V+E) log V)`                `O(V+E)`
  DSU operations                     Almost `O(1)` amortised                  `O(V)`

## 7. Recommended revision order

1.  Adjacency list/matrix, BFS, DFS, connected components.
2.  Flood Fill, Find Path, Number of Provinces, Number of Islands.
3.  Rotting Oranges and shortest path in an unweighted graph.
4.  Undirected/directed cycle detection and Bipartite Graph.
5.  Topological Sort, Course Schedule I/II, Eventual Safe States.
6.  Dijkstra and DSU; then optional MST and Word Ladder.

**Interview habit:** Before coding, state the graph representation,
traversal, visited-state meaning, disconnected-component handling, and
expected time/space complexity.
