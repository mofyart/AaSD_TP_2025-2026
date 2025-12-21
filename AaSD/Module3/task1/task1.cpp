#include <iostream>

#include "ArcGraph.hpp"
#include "IGraph.hpp"
#include "ListGraph.hpp"
#include "MatrixGraph.hpp"
#include "SetGraph.hpp"

void BFS(int vertex, const IGraph& graph, std::vector<bool>& visited, const std::function<void(int)> &func) {
    std::queue<int> queueVertex;
    queueVertex.push(vertex);

    visited[vertex] = true;

    while(!queueVertex.empty()) {
        int topVertex = queueVertex.front();
        queueVertex.pop();

        func(topVertex);

        for (const auto& nextVertex : graph.GetNextVertices(topVertex)) {
            if (!visited[nextVertex]) {
                queueVertex.push(nextVertex);
                 visited[nextVertex] = true;
            }
        }
    }
}

void MainBFS(const IGraph& graph, const std::function<void(int)> &func) {
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (size_t i = 0; i < graph.VerticesCount(); ++i) {
        if (!visited[i]) {
            BFS(i, graph, visited, func);
        }
    }
}


void DFS(int vertex, const IGraph& graph, std::vector<bool>& visited, const std::function<void(int)> &func) {
    func(vertex);
    visited[vertex] = true;

    for (const auto& nextVertex : graph.GetNextVertices(vertex)) {
        if (!visited[nextVertex]) {
            DFS(nextVertex, graph, visited, func);
        }
    }
}

void MainDFS(const IGraph& graph, const std::function<void(int)> &func) {
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (size_t i = 0; i < graph.VerticesCount(); ++i) {
        if (!visited[i]) {
            DFS(i, graph, visited, func);
        }
    }
}


void TopologicalSort(int vertex, const IGraph& graph, std::vector<bool>& visited, const std::function<void(int)> &func) {
    visited[vertex] = true;

    for (const auto& nextVertex : graph.GetNextVertices(vertex)) {
        if (!visited[nextVertex]) {
            TopologicalSort(nextVertex, graph, visited, func);
        }
    }

    func(vertex);
}

void MainTopologicalSort(const IGraph& graph, const std::function<void(int)> &func) {
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (size_t i = 0; i < graph.VerticesCount(); ++i) {
        if (!visited[i]) {
            TopologicalSort(i, graph, visited, func);
        }
    }
}


bool GetEqualityVertex(std::vector<int> first, std::vector<int> second) {
    std::sort(first.begin(), first.end());
    std::sort(second.begin(), second.end());

    return first == second;
}

void RunTest(IGraph& graph) {
    assert(GetEqualityVertex(graph.GetNextVertices(0), {1, 5}));
    assert(GetEqualityVertex(graph.GetNextVertices(1), {2, 3,5,6}));
    assert(GetEqualityVertex(graph.GetNextVertices(2), {}));
    assert(GetEqualityVertex(graph.GetNextVertices(3), {2, 4, 6}));
    assert(GetEqualityVertex(graph.GetNextVertices(4), {}));
    assert(GetEqualityVertex(graph.GetNextVertices(5), {4, 6}));
    assert(GetEqualityVertex(graph.GetNextVertices(6), {4}));

    assert(GetEqualityVertex(graph.GetPrevVertices(0), {}));
    assert(GetEqualityVertex(graph.GetPrevVertices(1), {0}));
    assert(GetEqualityVertex(graph.GetPrevVertices(2), {1, 3}));
    assert(GetEqualityVertex(graph.GetPrevVertices(3), {1}));
    assert(GetEqualityVertex(graph.GetPrevVertices(4), {3, 5, 6}));
    assert(GetEqualityVertex(graph.GetPrevVertices(5), {0, 1}));
    assert(GetEqualityVertex(graph.GetPrevVertices(6), {1, 3, 5}));

    std::cout << "Excellent !!" << '\n';
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    ListGraph listGraph(7);

    listGraph.AddEdge(0, 1);
    listGraph.AddEdge(0, 5);
    listGraph.AddEdge(1, 2);
    listGraph.AddEdge(1, 3);
    listGraph.AddEdge(1, 5);
    listGraph.AddEdge(1, 6);
    listGraph.AddEdge(3, 2);
    listGraph.AddEdge(3, 4);
    listGraph.AddEdge(3, 6);
    listGraph.AddEdge(5, 4);
    listGraph.AddEdge(5, 6);
    listGraph.AddEdge(6, 4);

    RunTest(listGraph);

    MainBFS(listGraph, [](int element){std::cout << element << ' ';});
    std::cout << '\n';

    MainDFS(listGraph, [](int element){std::cout << element << ' ';});
    std::cout << '\n';

    MainTopologicalSort(listGraph, [](int element){std::cout << element << ' ';});
    std::cout << '\n';
    std::cout << '\n';


    MatrixGraph matrixGraph(listGraph);
    RunTest(matrixGraph);

    MainBFS(matrixGraph, [](int element){std::cout << element << ' ';});
    std::cout << '\n';

    MainDFS(matrixGraph, [](int element){std::cout << element << ' ';});
    std::cout << '\n';

    MainTopologicalSort(matrixGraph, [](int element){std::cout << element << ' ';});
    std::cout << '\n';
    std::cout << '\n';


    SetGraph setGraph(matrixGraph);
    RunTest(setGraph);

    MainBFS(setGraph, [](int element){std::cout << element << ' ';});
    std::cout << '\n';

    MainDFS(setGraph, [](int element){std::cout << element << ' ';});
    std::cout << '\n';

    MainTopologicalSort(setGraph, [](int element){std::cout << element << ' ';});
    std::cout << '\n';
    std::cout << '\n';


    ArcGraph arcGraph(setGraph);
    RunTest(arcGraph);

    MainBFS(arcGraph, [](int element){std::cout << element << ' ';});
    std::cout << '\n';

    MainDFS(arcGraph, [](int element){std::cout << element << ' ';});
    std::cout << '\n';

    MainTopologicalSort(arcGraph, [](int element){std::cout << element << ' ';});
    std::cout << '\n';

    return 0;
}
