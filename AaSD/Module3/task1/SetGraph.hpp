#include "IGraph.hpp"

class SetGraph : public IGraph {
    private:
        std::vector<std::unordered_set<int>> setGraph;
    public:
        SetGraph(size_t countVertex) : setGraph(countVertex) {}

        SetGraph(const IGraph& other);

        virtual void AddEdge(int from, int to);

        virtual size_t VerticesCount() const;

        std::vector<int> GetNextVertices(int vertex) const;
        std::vector<int> GetPrevVertices(int vertex) const;
};
