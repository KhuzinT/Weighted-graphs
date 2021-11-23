#include <iostream>
#include <vector>

//********************************************************************************************

using Vertex = int64_t;

using WeightT = int64_t;

const int64_t kInfinity = 1000 * 1000 * 1000 * 1LL;

//********************************************************************************************

struct Edge {
    Vertex begin;
    Vertex end;

    WeightT weight;

    explicit Edge(const Vertex& a, const Vertex& b, const WeightT& w) {
        begin = a;
        end = b;
        weight = w;
    }
};

class GraphListEdge {
private:
    uint64_t q_edge_ = 0;
    uint64_t q_vertex_ = 0;

    std::vector<Edge> list_;

    void Add(const Vertex& begin, const Vertex& end, const WeightT& weight) {
        Edge edge(begin, end, weight);
        list_.push_back(edge);
        ++q_edge_;
    }

public:
    explicit GraphListEdge(const uint64_t& quantity_vertex) {
        q_vertex_ = quantity_vertex;
    }

    uint64_t GetQVertex() const {
        return q_vertex_;
    }

    uint64_t GetQEdge() const {
        return q_edge_;
    }

    void AddEdge(const Vertex& begin, const Vertex& end, const WeightT& weight) {
        Add(begin - 1, end - 1, weight);
    }

    Edge GetEdge(const uint64_t& index) const {
        return list_[index];
    }
};

WeightT BellmanFordAlgorithm(GraphListEdge& graph, const Vertex& start, const Vertex& finish, const uint64_t& count_edge) {
    std::vector<WeightT> dist(graph.GetQVertex(), kInfinity);
    dist[start] = 0;

    for (uint64_t i = 0; i < count_edge; ++i) {
        std::vector<WeightT> tmp = dist;
        for (uint64_t j = 0; j < graph.GetQEdge(); ++j) {
            Edge edge = graph.GetEdge(j);
            dist[edge.end] = std::min(dist[edge.end], tmp[edge.begin] + edge.weight);
        }
    }

    return dist[finish];
}

int main() {
    uint64_t q_vertex = 0;
    uint64_t q_edge = 0;
    std::cin >> q_vertex >> q_edge;

    GraphListEdge graph(q_vertex);

    uint64_t count_edge = 0;
    std::cin >> count_edge;

    Vertex start = 0;
    Vertex finish = 0;
    std::cin >> start >> finish;

    for (uint64_t i = 0; i < q_edge; ++i) {
        Vertex begin = 0;
        Vertex end = 0;
        WeightT weight = 0;
        std::cin >> begin >> end >> weight;
        graph.AddEdge(begin, end, weight);
    }

    auto answer = BellmanFordAlgorithm(graph, start - 1, finish - 1, count_edge);
    if (answer == kInfinity) {
        std::cout << -1 << std::endl;
    } else {
        std::cout << answer << std::endl;
    }

    return 0;
}