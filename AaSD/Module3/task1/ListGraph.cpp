#include "ListGraph.hpp"
#include <cmath>

ListGraph::ListGraph(const IGraph& other) {
    size_t quantityVerteces = other.VerticesCount();
    listGraph.resize(quantityVerteces);

    for (size_t i = 0; i < quantityVerteces; ++i) {
        for (const auto& vertex : other.GetNextVertices(i)) {
            listGraph[i].push_back(vertex);
        }
    }
}

void ListGraph::AddEdge(int from, int to) {
    size_t quantityVertices = listGraph.size();

    assert(from >= 0 && to >= 0 && from < quantityVertices && to < quantityVertices);

    listGraph[from].push_back(to);
}

size_t ListGraph::VerticesCount() const {
    return listGraph.size();
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    assert(vertex < listGraph.size());

    return listGraph[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
    int quantityVertices = listGraph.size();

    assert(vertex < quantityVertices);

    std::vector<int> prevVertices;

    for (size_t i = 0; i < quantityVertices; ++i) {
        if (vertex != i) {
            for (const auto& vertexVal : listGraph[i]) {
                if (vertex == vertexVal) {
                    prevVertices.push_back(i);
                    break;
                }
            }
        }
    }

    return prevVertices;
}
