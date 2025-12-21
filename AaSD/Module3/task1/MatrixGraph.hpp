#include "IGraph.hpp"

class MatrixGraph : public IGraph {
    private:
        std::vector<std::vector<int>> matrixGraph;
    public:
        MatrixGraph(int size) : matrixGraph(size, std::vector<int>(size, 0)) {}

        MatrixGraph(const IGraph& other);

        void AddEdge(int from, int to) override;

        size_t VerticesCount() const override;

        virtual std::vector<int> GetNextVertices(int vertex) const override;
        virtual std::vector<int> GetPrevVertices(int vertex) const override;
};
