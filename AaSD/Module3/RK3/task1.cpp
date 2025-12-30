#include <iostream>
#include <queue>

using namespace std;

class Graph {
    private:
        std::vector<std::vector<int>> graph;

    public:
        Graph(size_t countVerices) : graph(countVerices) {}

        void AddVertices(int v, int u) {
            graph[u].push_back(v);
            graph[v].push_back(u);
        }

        int FindCountComponents() {
            std::vector<bool> visited(graph.size(), false);
            std::queue<int> queueVertices;

            int result = 0;

            for (size_t i = 0; i < graph.size(); ++i) {
                if (!visited[i]) {
                    ++result;
                    queueVertices.push(i);

                    while (!queueVertices.empty()) {
                        int v = queueVertices.front();
                         visited[v] = true;
                        queueVertices.pop();

                        for (const auto& to : graph[i]) {
                            if (!visited[to]) {
                                queueVertices.push(to);
                            }
                        }
                    }
                }
            }

            return result;
        }
};

int main() {
    int n, m;
    std::cin >> n >> m;

    Graph graph(n);

    for (size_t i = 0; i < m; ++i) {
        int v, u;
        std::cin >> v >> u;

        graph.AddVertices(u, v);
    }

    std::cout << graph.FindCountComponents() << '\n';
    return 0;
}
