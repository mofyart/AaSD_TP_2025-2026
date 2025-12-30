#include "Edge.hpp"

bool Edge::operator<(const Edge& other) const {
    return weight < other.weight;
}
