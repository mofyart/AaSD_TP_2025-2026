#include "SetGraph.hpp"

SetGraph::SetGraph(const IGraph& other) {
    size_t quantityVerteces = other.VerticesCount();
    setGraph.resize(quantityVerteces);

    for (size_t i = 0; i < quantityVerteces; ++i) {
        for (const auto& vertex : other.GetNextVertices(i)) {
            setGraph[i].insert(vertex);
        }
    }
}

void SetGraph::AddEdge(int from, int to) {
    size_t quantityVertices = setGraph.size();

    assert(from >= 0 && to >= 0 && from < quantityVertices && to < quantityVertices);

    setGraph[from].insert(to);
}

size_t SetGraph::VerticesCount() const {
    return setGraph.size();
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    assert(vertex < setGraph.size());

    std::vector<int> nextVertices;

    for (const auto& vertexVal : setGraph[vertex]) {
        nextVertices.push_back(vertexVal);
    }

    return nextVertices;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    size_t quantityVertex = setGraph.size();

    assert(vertex < quantityVertex);

    std::vector<int> prevVertex;

    for (size_t i = 0; i < quantityVertex; ++i) {
        if (vertex != i && setGraph[i].count(vertex)) {
            prevVertex.push_back(i);
        }
    }

    return prevVertex;
}
