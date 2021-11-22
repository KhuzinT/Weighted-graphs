#include <iostream>
#include <vector>

//********************************************************************************************

using Vertex = int32_t;

using WeightT = int32_t;

const int32_t kInfinity = 1000 * 1000 * 1000 * 1LL;

//********************************************************************************************

struct Edge {
    Vertex begin;
    Vertex end;

    WeightT time_in;
    WeightT time_out;

    explicit Edge(const Vertex& a, const WeightT& in, const Vertex& b, const WeightT& out) {
        begin = a;
        end = b;
        time_in = in;
        time_out = out;
    }
};

class GraphListEdge {
private:
    uint32_t q_edge_ = 0;
    uint32_t q_vertex_ = 0;

    std::vector<Edge> list_;

    void Add(const Vertex& begin, const WeightT& time_in, const Vertex& end, const WeightT& time_out) {
        Edge edge(begin, time_in, end, time_out);
        list_.push_back(edge);
        ++q_edge_;
    }

public:
    explicit GraphListEdge(const uint32_t& quantity_vertex) {
        q_vertex_ = quantity_vertex;
    }

    uint32_t GetQVertex() const {
        return q_vertex_;
    }

    uint32_t GetQEdge() const {
        return q_edge_;
    }

    void AddEdge(const Vertex& begin, const WeightT& time_in, const Vertex& end, const WeightT& time_out) {
        Add(begin - 1, time_in, end - 1, time_out);
    }

    Edge GetEdge(const uint32_t& index) const {
        return list_[index];
    }
};

WeightT BellmanFordAlgorithm(GraphListEdge& graph, const Vertex& start, const Vertex& finish) {
    std::vector<WeightT> dist(graph.GetQVertex(), kInfinity);
    dist[start] = 0;

    bool is_edge_in_processing = true;

    while (is_edge_in_processing) {
        is_edge_in_processing = false;
        for (uint32_t i = 0; i < graph.GetQEdge(); ++i) {
            auto edge = graph.GetEdge(i);
            if (dist[edge.begin] < kInfinity) {
                if (dist[edge.begin] <= edge.time_in && edge.time_out < dist[edge.end]) {
                    dist[edge.end] = edge.time_out;
                    is_edge_in_processing = true;
                }
            }
        }
    }

    return dist[finish];
}

int main() {
    int32_t q_vertex = 0;
    std::cin >> q_vertex;

    GraphListEdge graph(q_vertex);

    Vertex start = 0;
    Vertex finish = 0;
    std::cin >> start >> finish;

    int32_t q_edge = 0;
    std::cin >> q_edge;

    for (int32_t i = 0; i < q_edge; ++i) {
        Vertex begin = 0;
        WeightT time_in = 0;

        Vertex end = 0;
        WeightT time_out = 0;
        std::cin >> begin >> time_in;
        std::cin >> end >> time_out;
        graph.AddEdge(begin, time_in, end, time_out);
    }

    std::cout << BellmanFordAlgorithm(graph, start - 1, finish - 1) << std::endl;

    return 0;
}
