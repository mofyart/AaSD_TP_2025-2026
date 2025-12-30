#include <cassert>
#include <iostream>

#include <vector>
#include <set>

struct Edge {
    long long weight;

    int firstVertex;
    int secondVertex;

    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

class DSU {
    private:
        std::vector<int> parents;
        std::vector<int> ranks;
    public:
        DSU(size_t size);

        size_t FindSet(size_t vertex);

        bool UnionSet(ssize_t firstVertex, size_t secondVertex);
};

DSU::DSU(size_t size) : parents(size), ranks(size, 1) {
    for (size_t i = 0; i < size; ++i) {
        parents[i] = i;
    }
}

size_t DSU::FindSet(size_t vertex) {
    if  (parents[vertex] == vertex) {
        return vertex;
    }

    parents[vertex] = FindSet(parents[vertex]);
    return parents[vertex];
}

bool DSU::UnionSet(ssize_t firstVertex, size_t secondVertex) {
    firstVertex = FindSet(firstVertex);
    secondVertex = FindSet(secondVertex);

    if (ranks[firstVertex] > ranks[secondVertex]) {
        ranks[firstVertex] += ranks[secondVertex];

        parents[secondVertex] = parents[firstVertex];

        return ranks[firstVertex] == ranks.size() ? true  : false;
    } else {
        ranks[secondVertex] += ranks[firstVertex];

        parents[firstVertex] = parents[secondVertex];

        return ranks[secondVertex] == ranks.size() ? true : false;
    }

    return false;
}

long long FindSumLengthBacknoneTree(std::multiset<Edge>& edges, DSU& dsu) {
    long long result = 0;

    for (const auto& edge : edges) {
        size_t firstParent = dsu.FindSet(edge.firstVertex - 1);
        size_t secondParent = dsu.FindSet(edge.secondVertex - 1);

        if (firstParent != secondParent) {
            result += edge.weight;

            if (dsu.UnionSet(firstParent, secondParent)) {
                break;
            }
        }
    }

    return result;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    int countVertexes, countEdges;

    std::cin >> countVertexes >> countEdges;

    DSU dsu(countVertexes);

    std::multiset<Edge> setEdges;

    for (size_t i = 0; i < countEdges; ++i) {
        int firstVertex, secondVertex;
        long long weight;

        std::cin >> firstVertex >> secondVertex >> weight;
        assert(!std::cin.fail());

        setEdges.insert({weight, firstVertex, secondVertex});
    }

    long long result = FindSumLengthBacknoneTree(setEdges, dsu);

    std::cout << result << '\n';

    return 0;
}
