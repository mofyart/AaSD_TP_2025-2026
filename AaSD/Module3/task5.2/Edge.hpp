struct Edge {
    double weight;

    int firstVertex;
    int secondVertex;

    bool operator<(const Edge& other) const;
};
