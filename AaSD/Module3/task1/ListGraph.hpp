#include "IGraph.hpp"

class ListGraph : public IGraph {
    private:
        std::vector<std::vector<int>> listGraph;
    public:
        ListGraph(size_t countVertex) : listGraph(countVertex) {}

        ListGraph(const IGraph& objectGraph);

        void AddEdge(int from, int to) override;

        size_t VerticesCount() const override;

        virtual std::vector<int> GetNextVertices(int vertex) const override;
        virtual std::vector<int> GetPrevVertices(int vertex) const override;
};
