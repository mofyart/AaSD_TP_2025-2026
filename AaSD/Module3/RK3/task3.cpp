#include <iostream>
#include <limits>
#include <climits>

#include <set>
#include <vector>

using namespace std;

class Graph {
    private:
        size_t countUniverces;
        long long aWeight;
        long long bWeight;

        void dijkstraFindShortestPath(int from, int end, std::vector<long long>& distances) {
            std::set<std::pair<long long, long long>> setUniverces;
            distances[from] = 0;

            setUniverces.insert({0, from});

            while (!setUniverces.empty()) {
                long long weight = (*setUniverces.begin()).first;
                long long v = (*setUniverces.begin()).second;

                if (v == end) {
                    return;
                }

                setUniverces.erase({weight, v});

                if (distances[(v + 1) % countUniverces] == std::numeric_limits<long long>::max()) {
                    distances[(v + 1) % countUniverces] = weight + aWeight;
                    setUniverces.insert({distances[(v + 1) % countUniverces], (v + 1) % countUniverces});
                } else if (distances[(v + 1) % countUniverces] > weight + aWeight) {
                    setUniverces.erase({distances[(v + 1) % countUniverces], (v + 1) % countUniverces});
                    distances[(v + 1) % countUniverces] = weight + aWeight;
                    setUniverces.insert({distances[(v + 1) % countUniverces], (v + 1) % countUniverces});
                }

                if (distances[(v * v + 1) % countUniverces] == std::numeric_limits<long long>::max()) {
                    distances[(v * v + 1) % countUniverces] = weight + bWeight;
                    setUniverces.insert({distances[(v * v + 1) % countUniverces], (v * v + 1) % countUniverces});
                } if (distances[(v * v + 1) % countUniverces] > weight + bWeight) {
                    setUniverces.erase({distances[(v * v + 1) % countUniverces], (v * v + 1) % countUniverces});
                    distances[(v * v + 1) % countUniverces] = weight + bWeight;
                    setUniverces.insert({distances[(v * v + 1) % countUniverces], (v * v + 1) % countUniverces});
                }
            }
        }
    public:
        Graph(size_t value, long long aVal, long long bVal) : countUniverces(value), aWeight(aVal), bWeight(bVal) {}

        long long FindMinButtles(int start, int end) {
            if (start == end)  {
                return 0;
            }

            std::vector<long long> distances(countUniverces, std::numeric_limits<long long>::max());

            dijkstraFindShortestPath(start, end, distances);

            return distances[end];
        }
};

int main() {
    long long a, b;
    int M, x, y;
    std:cin >> a >> b >> M >> x >> y;

    Graph graph(M, a, b);

    std::cout << graph.FindMinButtles(x, y) << '\n';

    return 0;
}
