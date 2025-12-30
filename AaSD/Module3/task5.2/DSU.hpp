#include <vector>

class DSU {
    private:
        std::vector<int> parents;
        std::vector<int> ranks;
    public:
        DSU(size_t size);

        size_t FindSet(size_t vertex);

        void UnionSet(size_t firstVertex, size_t secondVertex);
};
