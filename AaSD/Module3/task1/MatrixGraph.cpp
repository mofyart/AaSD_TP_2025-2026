#include "MatrixGraph.hpp"

MatrixGraph::MatrixGraph(const IGraph& other) {
    size_t quantityVertices = other.VerticesCount();

    matrixGraph.resize(quantityVertices, std::vector<int>(quantityVertices, 0));

    for (size_t i = 0; i < quantityVertices; ++i) {
        for (const auto& vertex : other.GetNextVertices(i)) {
            matrixGraph[i][vertex] = 1;
        }
    }
}

void MatrixGraph::AddEdge(int from, int to) {
    size_t quantityVertices = matrixGraph.size();

    assert(from >= 0 && to >= 0 && from < quantityVertices && to < quantityVertices);

    matrixGraph[from][to] = 1;
}

size_t MatrixGraph::VerticesCount() const {
    return matrixGraph.size();
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    assert(vertex < matrixGraph.size());

    std::vector<int> nextVertices;

    for (size_t i = 0; i < matrixGraph[vertex].size(); ++i) {
        if (matrixGraph[vertex][i]) {
            nextVertices.push_back(i);
        }
    }

    return nextVertices;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    size_t quantityVerteces = matrixGraph.size();

    assert(vertex < quantityVerteces);

    std::vector<int> prevVertices;

    for (size_t i = 0; i < quantityVerteces; ++i) {
        if (vertex != i && matrixGraph[i][vertex]) {
            prevVertices.push_back(i);
        }
    }

    return prevVertices;
}
