#include "DSU.hpp"
#include "Edge.hpp"

#include <cassert>
#include <climits>
#include <iostream>

#include <set>
#include <unordered_set>
#include <vector>

#include <random>

void DFSInternal(std::vector<std::vector<int>>& tree, std::vector<int>& preorderedTree, std::vector<bool>& visited, int vertex) {
    visited[vertex] = true;
    preorderedTree.push_back(vertex);

    for (const auto& toVertex : tree[vertex]) {
        if (!visited[toVertex]) {
            DFSInternal(tree, preorderedTree, visited, toVertex);
        }
    }
}

std::vector<int> DFS(std::vector<std::vector<int>>& tree) {
    std::vector<int> preorderedTree;
    std::vector<bool> visited(tree.size(), false);

    DFSInternal(tree, preorderedTree, visited, 0);

    preorderedTree.push_back(0);

    return preorderedTree;
}

double CalculateLengthEdges(std::vector<int>& preorderedTree, std::vector<Edge>& edges, size_t countVertexes) {
    int vertex = preorderedTree[0];

    double sum = 0;

    for (size_t i = 1; i < countVertexes + 1; ++i) {
        int nextVertex = preorderedTree[i];

        for (size_t j = 0; j < edges.size(); ++j) {
            if ((edges[j].firstVertex == vertex && edges[j].secondVertex == nextVertex) ||
            (edges[j].firstVertex == nextVertex && edges[j].secondVertex == vertex)) {
                sum += edges[j].weight;
                break;
            }
        }

        vertex = nextVertex;
    }

    return sum;
}

double FindSumLengthBacknoneTree(std::set<Edge>& setEdges, std::vector<Edge>& edges, DSU& dsu, size_t countVertexes) {
    double result = 0;

    std::vector<std::vector<int>> tree(countVertexes);

    for (const auto& edge : setEdges) {
        size_t firstParent = dsu.FindSet(edge.firstVertex);
        size_t secondParent = dsu.FindSet(edge.secondVertex);

        if (firstParent != secondParent) {
            dsu.UnionSet(firstParent, secondParent);

            tree[edge.firstVertex].push_back(edge.secondVertex);
            tree[edge.secondVertex].push_back(edge.firstVertex);
        }
    }

    std::vector<int> preorderedTree = DFS(tree);

    return CalculateLengthEdges(preorderedTree, edges, countVertexes);
}

double FindApproximateSumLength(std::vector<Edge>& edges, size_t countVertexes) {
    DSU dsu(countVertexes);

    std::set<Edge> setEdges;

    for (size_t i = 0; i < edges.size(); ++i) {
        setEdges.insert(edges[i]);
    }

    return FindSumLengthBacknoneTree(setEdges, edges, dsu, countVertexes);
}



void FindInternalExactSumLength(std::vector<Edge>& edges, std::unordered_set<int> usedVertexes,
    size_t vertex, size_t countVertexes, double curSum, double& minSum, size_t startVertex) {

    if (curSum >= minSum) return;

    if (usedVertexes.size() == countVertexes) {
        for (size_t i = 0; i < edges.size(); ++i) {
            if ((edges[i].firstVertex == vertex && edges[i].secondVertex == startVertex) ||
        (edges[i].firstVertex == startVertex && edges[i].secondVertex == vertex)) {
                curSum += edges[i].weight;
                minSum = std::min(curSum, minSum);
                break;
            }
        }

        return;
    }

    for (size_t i = 0; i < countVertexes; ++i) {
        if (!usedVertexes.count(i)) {
            size_t newVertex = i;
            double addedWeight = 0;

            for (size_t j = 0; j < edges.size(); ++j) {
                if ((edges[j].firstVertex == vertex && edges[j].secondVertex == newVertex) ||
                (edges[j].firstVertex == newVertex && edges[j].secondVertex == vertex)) {
                    addedWeight = edges[j].weight;
                    break;
                }
            }

            usedVertexes.insert(newVertex);
            FindInternalExactSumLength(edges, usedVertexes, newVertex, countVertexes, curSum + addedWeight, minSum, startVertex);
            usedVertexes.erase(newVertex);
        }
    }
}

double FindExactSumLength(std::vector<Edge>& edges, size_t countVertexes) {
    double result = std::numeric_limits<double>::max();


    std::unordered_set<int> usedVertexes;

    usedVertexes.insert(0);

    FindInternalExactSumLength(edges, usedVertexes, 0, countVertexes, 0, result, 0);

    return result;
}



std::vector<Edge> FindWeightsEdges(std::vector<std::pair<double, double>>& vertexes) {
    std::vector<Edge> edges;

    for (int i = 0; i < vertexes.size() - 1; ++i) {
        for (int j = i + 1; j < vertexes.size(); ++j) {
            double weight = std::sqrt(std::pow((vertexes[i].first - vertexes[j].first), 2) + std::pow((vertexes[i].second - vertexes[j].second), 2));

            edges.push_back({weight, i, j});
        }
    }

    return edges;
}

std::vector<double> FindCoefficientsAproximation(size_t countVertexes, size_t K, std::mt19937& gen, std::uniform_real_distribution<>& dist) {
    std::vector<double> results;

    for (size_t i = 0; i < K; ++i) {
        std::vector<std::pair<double, double>> vertexes(countVertexes);

        for (size_t j = 0; j < countVertexes; ++j) {
            double s = 0;
            double u = 0;
            double v = 0;

            while (s == 0 || s > 1) {
                u = dist(gen);
                v = dist(gen);

                s = u * u + v * v;
            }

            double x = u * std::sqrt((-2.0 * std::log(s)) / s);
            double y = v * std::sqrt((-2.0 * std::log(s)) / s);

            vertexes[j] = {x, y};
        }

        std::vector<Edge> edges = FindWeightsEdges(vertexes);

        double exactResult = FindExactSumLength(edges, countVertexes);
        double approximateResult = FindApproximateSumLength(edges, countVertexes);

        results.push_back(std::max(exactResult / approximateResult, approximateResult / exactResult));
    }

    return results;
}



void FindDeviations(std::vector<double>& coefficientsAproximation, double& averageResult, double& averageQuadricResult) {
    double sum = 0;

    for (size_t i = 0; i < coefficientsAproximation.size(); ++i) {
        sum += coefficientsAproximation[i];
    }
    averageResult = sum / coefficientsAproximation.size();

    sum = 0;

    for (size_t i = 0; i < coefficientsAproximation.size(); ++i) {
        sum += std::pow((coefficientsAproximation[i] - averageResult), 2);
    }
    averageQuadricResult = std::sqrt(sum / coefficientsAproximation.size());
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    std::cout << "Введите количество экспериметов, которое вы хотите провести для кажого числа вершин" << '\n';
    int K = 0;
    std::cin >> K;
    assert(!std::cin.fail());

    std::cout << '\n';

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(-1.0, 1.0);

    std::cout << "Среднее значение и среднее квдартичное отклонение коэффициента апроксимации" << '\n';

    for (size_t countVertexes = 2; countVertexes < 11; ++countVertexes) {
        std::vector<double> coefficientsAproximation = FindCoefficientsAproximation(countVertexes, K, gen, dist);

        double averageResult, averageQuadricResult;

        FindDeviations(coefficientsAproximation, averageResult, averageQuadricResult);

        std::cout << "N=" << countVertexes << ": "  << averageResult << "  " << averageQuadricResult << '\n';
    }

    return 0;
}
