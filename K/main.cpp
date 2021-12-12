#include <iostream>
#include <vector>
#include <any>

//********************************************************************************************

struct Time {
    int32_t time_in;
    int32_t time_out;
};

class IGraph {
public:
    inline static const int32_t kINFINITY = 1000 * 1000 * 1000 * 1LL;

    using Vertex = int32_t;

    using WeightT = int32_t;

    template <typename Type>
    struct Edge {
        Vertex begin;
        Vertex end;

        Type edge_info;

        explicit Edge(const Vertex& first, const Vertex& second, const Type& info) {
            begin = first;
            end = second;

            edge_info = info;
        }
    };

protected:
    int32_t q_vertex_ = 0;

    int32_t q_edge_ = 0;

public:
    [[nodiscard]] int32_t GetQVertex() const {
        return q_vertex_;
    }

    int32_t GetQEdge() const {
        return q_edge_;
    }

    virtual std::any GetEdge(const int32_t& index) const = 0;
};

template <typename Type>
class GraphListEdge final : public IGraph {
private:
    std::vector<Edge<Type>> list_;

    void Add(const Vertex& begin, const Vertex& end, const Type& edge_info) {
        Edge<Type> edge(begin, end, edge_info);
        list_.push_back(edge);
        ++q_edge_;
    }

public:
    explicit GraphListEdge(const int32_t& quantity_vertex) {
        q_vertex_ = quantity_vertex;
    }

    void AddWeightEdgeInNumberingFromOne(const Vertex& begin, const Vertex& end, const Type& edge_info) {
        Add(begin - 1, end - 1, edge_info);
    }

    void AddWeightEdgeInNumberingFromZero(const Vertex& begin, const Vertex& end, const Type& edge_info) {
        Add(begin, end, edge_info);
    }

    std::any GetEdge(const int32_t& index) const override {
        return list_[index];
    }
};

IGraph::WeightT BellmanFordAlgorithm(const IGraph& graph, const IGraph::Vertex& start, const IGraph::Vertex& finish) {
    std::vector<IGraph::WeightT> dist(graph.GetQVertex(), IGraph::kINFINITY);
    dist[start] = 0;

    bool is_edge_in_processing = true;

    while (is_edge_in_processing) {
        is_edge_in_processing = false;
        for (int32_t iteration = 0; iteration < graph.GetQEdge(); ++iteration) {
            auto any_edge = graph.GetEdge(iteration);
            auto edge = std::any_cast<IGraph::Edge<Time>>(any_edge);
            if (dist[edge.begin] < IGraph::kINFINITY) {
                if (dist[edge.begin] <= edge.edge_info.time_in && edge.edge_info.time_out < dist[edge.end]) {
                    dist[edge.end] = edge.edge_info.time_out;
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

    GraphListEdge<Time> graph(q_vertex);

    IGraph::Vertex start = 0;
    IGraph::Vertex finish = 0;
    std::cin >> start >> finish;

    int32_t q_edge = 0;
    std::cin >> q_edge;

    for (int32_t i = 0; i < q_edge; ++i) {
        IGraph::Vertex begin = 0;
        IGraph::WeightT time_in = 0;
        std::cin >> begin >> time_in;

        IGraph::Vertex end = 0;
        IGraph::WeightT time_out = 0;
        std::cin >> end >> time_out;

        graph.AddWeightEdgeInNumberingFromOne(begin, end, {time_in, time_out});
    }

    std::cout << BellmanFordAlgorithm(graph, start - 1, finish - 1) << std::endl;

    return 0;
}
