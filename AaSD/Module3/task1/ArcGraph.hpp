#include "IGraph.hpp"

class ArcGraph : public IGraph {
    private:
        std::vector<std::pair<int, int>> arcGraph;
        size_t countVertex;
    public:
        ArcGraph(size_t countVertexVal) : countVertex(countVertexVal) {}

        ArcGraph(const IGraph& other);

        void AddEdge(int from, int to);

        size_t VerticesCount() const;

        std::vector<int> GetNextVertices(int vertex) const;
        std::vector<int> GetPrevVertices(int vertex) const;
};
