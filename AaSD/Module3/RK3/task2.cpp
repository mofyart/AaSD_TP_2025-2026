#include <iostream>
#include <unordered_set>
#include <vector>

using namespace std;

class Graph {
    private:
        std::vector<std::unordered_set<int>> graph;

    public:
        Graph(size_t countVerices) : graph(countVerices) {}

        void AddVertices(int v, int u) {
            graph[u].insert(v);
            graph[v].insert(u);
        }

        int IsGameltonPath(std::vector<int>& path) {
            if (path.size() != graph.size()) {
                return 0;
            }

            std::vector<bool> visited(graph.size(), false);
            int v = path[0];
            visited[v] = true;

            for (size_t i = 1; i < path.size(); ++i) {
                if (graph[v].count(path[i]) && !visited[i]) {
                    v = path[i];
                    visited[v] = true;
                } else {
                    return 0;
                }
            }

            return 1;
        }
};

int main() {
    int n, m, k;
    std::cin >> n >> m >> k;

    Graph graph(n);

    for (size_t i = 0; i < m; ++i) {
        int v, u;
        std::cin >> v >> u;

        graph.AddVertices(u, v);
    }

    std::vector<int> path;

    for (size_t i = 0; i < k; ++i) {
        int v;
        std::cin >> v;

        path.push_back(v);
    }

    std::cout << graph.IsGameltonPath(path) << '\n';
    return 0;
}
