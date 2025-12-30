#include "DSU.hpp"

DSU::DSU(size_t size) : parents(size), ranks(size, 1) {
    for (size_t i = 0; i < size; ++i) {
        parents[i] = i;
    }
}

size_t DSU::FindSet(size_t vertex) {
    if (parents[vertex] == vertex) {
        return vertex;
    }

    parents[vertex] = FindSet(parents[vertex]);
    return parents[vertex];
}

void DSU::UnionSet(size_t firstVertex, size_t secondVertex) {
    firstVertex = FindSet(firstVertex);
    secondVertex = FindSet(secondVertex);

    if (ranks[firstVertex] > ranks[secondVertex]) {
        ranks[firstVertex] += ranks[secondVertex];

        parents[secondVertex] = parents[firstVertex];
    } else {
        ranks[secondVertex] += ranks[firstVertex];

        parents[firstVertex] = parents[secondVertex];
    }
}
