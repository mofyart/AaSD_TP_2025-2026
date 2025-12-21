#include <cassert>
#include <climits>
#include <iostream>

#include <set>
#include <vector>

class Graph {
    private:
        struct Vertex {
            int to;
            int weight;
        };

        std::vector<std::vector<Vertex>> graphRepresentation;
        size_t countVertices;

        bool updateParallelEdges(int from, int to, int weight) {
            for (size_t i = 0; i < graphRepresentation[from].size(); ++i) {
                if (graphRepresentation[from][i].to == to) {

                    graphRepresentation[from][i].weight = std::min(weight, graphRepresentation[from][i].weight);
                    return true;
                }
            }

            return false;
        }

        void dijkstraFindShortestPath(int from, std::vector<long long>& distances) {
            std::set<std::pair<long long, int>> setVertices;
            setVertices.insert({0, from});
            distances[from] = 0;

            while (!setVertices.empty()) {
                std::pair<long long, int> topVertex = *(setVertices.begin());
                setVertices.erase(topVertex);

                for (const auto& vertexTo : graphRepresentation[topVertex.second]) {
                    if (distances[vertexTo.to] == INT_MAX) {
                        distances[vertexTo.to] = distances[topVertex.second] + vertexTo.weight;
                        setVertices.insert({distances[vertexTo.to], vertexTo.to});
                    } else if (distances[vertexTo.to] > distances[topVertex.second] + vertexTo.weight) {
                        setVertices.erase({distances[vertexTo.to], vertexTo.to});
                        distances[vertexTo.to] = distances[topVertex.second] + vertexTo.weight;
                        setVertices.insert({ distances[vertexTo.to], vertexTo.to});
                    }
                }
            }
        }
    public:
        Graph(size_t countVerticesVal) : graphRepresentation(countVerticesVal), countVertices(countVerticesVal) {}

        void AddVertex(int from, int to, int weight) {
            assert(from >= 0 && to >= 0 && from < countVertices && to < countVertices);
            assert(weight >= 0);

            if (from == to) {
                return;
            }

            bool findEdge = updateParallelEdges(from, to, weight);

            if (!findEdge) {
                graphRepresentation[to].push_back({from, weight});
                graphRepresentation[from].push_back({to, weight});
            } else {
                updateParallelEdges(to, from, weight);
            }
        }

        long long FindShortestPath(int from, int to) {
            assert(from >= 0 && to >= 0 && from < countVertices && to < countVertices);

            if (from == to) {
                return 0;
            }

            std::vector<long long> distances(countVertices, INT_MAX);

            dijkstraFindShortestPath(from, distances);

            return distances[to];
        }
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    size_t N, M;
    std::cin >> N >> M;
    assert(!std::cin.fail());

    Graph graph(N);

    for (size_t i = 0; i < M; ++i) {
        int from, to, time;
        std::cin >> from >> to >> time;
        assert(!std::cin.fail());

        graph.AddVertex(from, to, time);
    }

    int begin, target;

    std::cin >> begin >> target;
    assert(!std::cin.fail());

    long long result = graph.FindShortestPath(begin, target);

    std::cout << result << '\n';

    return 0;
}
