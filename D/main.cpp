#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <any>

//********************************************************************************************

template <typename Type>
const Type kZero = 0;

class IGraph {
public:
    inline static const uint32_t kINFINITY = 1000 * 1000 * 1000 * 1LL;

    using Vertex = uint32_t;

    using WeightT = uint32_t;

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

        bool operator<(const Edge<Type>& other) const {
            return std::make_pair(edge_info, std::make_pair(begin, end)) <
                   std::make_pair(other.edge_info, std::make_pair(other.begin, other.end));
        }
    };

protected:
    uint32_t q_vertex_ = 0;

    uint32_t q_edge_ = 0;

public:
    [[nodiscard]] uint32_t GetQVertex() const {
        return q_vertex_;
    }

    uint32_t GetQEdge() const {
        return q_edge_;
    }

    virtual std::any GetEdge(const uint32_t& index) const = 0;

    virtual void SortEdges() = 0;
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
    explicit GraphListEdge(const uint32_t& quantity_vertex) {
        q_vertex_ = quantity_vertex;
    }

    void AddWeightEdgeInNumberingFromOne(const Vertex& begin, const Vertex& end, const Type& edge_info) {
        Add(begin - 1, end - 1, edge_info);
    }

    void AddWeightEdgeInNumberingFromZero(const Vertex& begin, const Vertex& end, const Type& edge_info) {
        Add(begin, end, edge_info);
    }

    void AddEdgeInNumberingFromOne(const Vertex& begin, const Vertex& end) {
        Add(begin - 1, end - 1, kZero<Type>);
    }

    void AddEdgeInNumberingFromZero(const Vertex& begin, const Vertex& end) {
        Add(begin, end, kZero<Type>);
    }

    std::any GetEdge(const uint32_t& iteration) const override {
        return list_[iteration];
    }

    void SortEdges() override {
        std::sort(list_.begin(), list_.end());
    }
};

class DSU {
private:
    std::vector<uint32_t> parent_;
    std::vector<uint32_t> rank_;

public:
    explicit DSU(const uint32_t& quantity) {
        parent_.resize(quantity, IGraph::kINFINITY);
        for (uint32_t i = 0; i < quantity; ++i) {
            parent_[i] = i;
        }
        rank_.resize(quantity, 0);
    }

    uint32_t FindSet(const uint32_t& element) {
        if (element == parent_[element]) {
            return element;
        }
        parent_[element] = FindSet(parent_[element]);
        return parent_[element];
    }

    void UnionSets(const uint32_t& first_elem, const uint32_t& second_elem) {
        uint32_t new_first = FindSet(first_elem);
        uint32_t new_second = FindSet(second_elem);
        if (new_first != new_second) {
            if (rank_[new_first] < rank_[new_second]) {
                parent_[new_first] = new_second;
                if (rank_[new_first] == rank_[new_second]) {
                    ++rank_[new_second];
                }
            } else {
                parent_[new_second] = new_first;
                if (rank_[new_first] == rank_[new_second]) {
                    ++rank_[new_first];
                }
            }
        }
    }
};

uint32_t KruskalAlgorithm(IGraph& graph) {
    uint32_t min_weight = 0;

    DSU dsu(graph.GetQVertex());
    graph.SortEdges();

    for (uint32_t iteration = 0; iteration < graph.GetQEdge(); ++iteration) {
        auto any_edge = graph.GetEdge(iteration);
        auto edge = std::any_cast<IGraph::Edge<IGraph::WeightT>>(any_edge);
        IGraph::Vertex first_vertex = edge.begin;
        IGraph::Vertex second_vertex = edge.end;

        if (dsu.FindSet(first_vertex) != dsu.FindSet(second_vertex)) {
            min_weight += edge.edge_info;
            dsu.UnionSets(first_vertex, second_vertex);
        }
    }

    return min_weight;
}

std::vector<IGraph::Edge<IGraph::WeightT>> KruskalAlgorithmEdges(IGraph& graph) {
    std::vector<IGraph::Edge<IGraph::WeightT>> edge_list;

    DSU dsu(graph.GetQVertex());
    graph.SortEdges();

    for (uint32_t iteration = 0; iteration < graph.GetQEdge(); ++iteration) {
        auto any_edge = graph.GetEdge(iteration);
        auto edge = std::any_cast<IGraph::Edge<IGraph::WeightT>>(any_edge);
        IGraph::Vertex first_vertex = edge.begin;
        IGraph::Vertex second_vertex = edge.end;

        if (dsu.FindSet(first_vertex) != dsu.FindSet(second_vertex)) {
            edge_list.push_back(edge);
            dsu.UnionSets(first_vertex, second_vertex);
        }
    }

    return edge_list;
}

//********************************************************************************************

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    uint32_t q_vertex = 0;
    std::cin >> q_vertex;

    GraphListEdge<IGraph::WeightT> graph(q_vertex + 1);

    std::vector<std::vector<IGraph::Vertex>> matrix(q_vertex);
    for (uint32_t iteration = 0; iteration < q_vertex; ++iteration) {
        matrix[iteration].resize(q_vertex, 0);
    }

    for (uint32_t current_vertex = 0; current_vertex < q_vertex; ++current_vertex) {
        for (uint32_t next_vertex = 0; next_vertex < q_vertex; ++next_vertex) {
            std::cin >> matrix[current_vertex][next_vertex];
            if (next_vertex < current_vertex) {
                graph.AddWeightEdgeInNumberingFromZero(current_vertex, next_vertex,
                                                       matrix[current_vertex][next_vertex]);
            }
        }
    }

    for (uint32_t iteration = 0; iteration < q_vertex; ++iteration) {
        uint32_t cost = 0;
        std::cin >> cost;
        graph.AddWeightEdgeInNumberingFromZero(q_vertex, iteration, cost);
    }

    std::cout << KruskalAlgorithm(graph);

    return 0;
}

//********************************************************************************************
