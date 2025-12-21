#include "ArcGraph.hpp"

ArcGraph::ArcGraph(const IGraph& other) {
    countVertex = other.VerticesCount();

    for (size_t i = 0; i < countVertex; ++i) {
        for (const auto& vertex : other.GetNextVertices(i)) {
            arcGraph.push_back({i, vertex});
        }
    }

    std::sort(arcGraph.begin(), arcGraph.end());
}

void ArcGraph::AddEdge(int from, int to) {
    size_t quantityVertices = arcGraph.size();

    assert(from >= 0 && to >= 0 && from < quantityVertices && to < quantityVertices);

    arcGraph.push_back({from, to});
    std::sort(arcGraph.begin(), arcGraph.end());
}

size_t ArcGraph::VerticesCount() const {
    if (!arcGraph.size()) {
        return 0;
    }

    return countVertex;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    std::vector<int> nextVertices;

    for (const auto& edge : arcGraph) {
        if (vertex == edge.first) {
            nextVertices.push_back(edge.second);
        } else if (vertex < edge.first) {
            break;
        }
    }

    return nextVertices;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    std::vector<int> prevVertices;

    for (const auto& edge : arcGraph) {
        if (vertex == edge.second) {
            prevVertices.push_back(edge.first);
        }
    }

    return prevVertices;
}
