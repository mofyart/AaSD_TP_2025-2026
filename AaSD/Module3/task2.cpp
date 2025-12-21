#include <cassert>
#include <climits>
#include <iostream>

#include <vector>
#include <queue>

class Graph {
    private:
        std::vector<std::vector<std::pair<int, int>>> graphRepresentation;
        size_t countVertices;

        void bfs(int from, int to, std::vector<std::pair<int, int>>& distancesCounter) {
            std::queue<int> queueVertices;

            queueVertices.push(from);
            distancesCounter[from] = {0, 1};

            while (!queueVertices.empty()) {
                int topVertex = queueVertices.front();
                queueVertices.pop();

                for (const auto& nextVertex : graphRepresentation[topVertex]) {
                    if (distancesCounter[nextVertex.first].first > distancesCounter[topVertex].first + 1) {
                        distancesCounter[nextVertex.first] = {distancesCounter[topVertex].first + 1, nextVertex.second * distancesCounter[topVertex].second};

                        if (nextVertex.first != to) {
                            queueVertices.push(nextVertex.first);
                        }
                    } else if (distancesCounter[nextVertex.first].first == distancesCounter[topVertex].first + 1) {
                        distancesCounter[nextVertex.first].second += nextVertex.second * distancesCounter[topVertex].second;
                    }
                }
            }
        }

        bool updateParallelEdges(int from, int to) {
            for (size_t i = 0; i < graphRepresentation[from].size(); ++i) {
                if (graphRepresentation[from][i].first == to) {
                    ++graphRepresentation[from][i].second;
                    return true;
                }
            }

            return false;
        }
    public:
        Graph(size_t countVerticesVal) : graphRepresentation(countVerticesVal), countVertices(countVerticesVal) {}

        void AddEdge(int from, int to) {
            assert(from >= 0 && from < countVertices && to >= 0 && to < countVertices);

            bool findEdge = updateParallelEdges(from, to);

            if (!findEdge) {
                graphRepresentation[to].push_back({from, 1});
                graphRepresentation[from].push_back({to, 1});
            } else {
                updateParallelEdges(to, from);
            }
        }

        int FindSmallesCountPath(int from, int to) {
            std::vector<std::pair<int, int>> distancesCounter(countVertices, {INT_MAX, 0});

            bfs(from, to, distancesCounter);

            return distancesCounter[to].second;
        }
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    int v, n;
    std::cin >> v >> n;
    assert(!std::cin.fail());

    Graph graph(v);

    for (size_t i = 0; i < n; ++i) {
        int from, to;
        std::cin >> from >> to;

        assert(!std::cin.fail());

        graph.AddEdge(from, to);
    }

    int begin, target;
    std::cin >> begin >> target;
    assert(!std::cin.fail());

    int result = graph.FindSmallesCountPath(begin, target);

    std::cout << result << '\n';

    return 0;
}
