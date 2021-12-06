#include <iostream>
#include <algorithm>
#include <vector>
#include <set>

const uint32_t kINFINITY = 1000 * 1000 * 1000 * 1LL;

//********************************************************************************************

class IGraph {
public:
    using Vertex = uint32_t;

    using DistT = uint32_t;

    using WeightT = uint32_t;

    struct IncomingEdge {
        Vertex vertex;
        WeightT weight;

        explicit IncomingEdge(const Vertex& current_vertex, const WeightT& current_weight) {
            vertex = current_vertex;
            weight = current_weight;
        }

        bool operator<(const IncomingEdge& other) const {
            return std::make_pair(weight, vertex) < std::make_pair(other.weight, other.vertex);
        }
    };

protected:
    uint32_t q_vertex_ = 0;

    bool is_oriented_ = false;

public:
    [[nodiscard]] uint32_t GetQVertex() const {
        return q_vertex_;
    }

    virtual void Add(const Vertex& begin, const Vertex& end, const WeightT& weight) = 0;

    void AddWeightEdge(const Vertex& begin, const Vertex& end, const WeightT& weight) {
        Add(begin - 1, end - 1, weight);
    }

    void AddEdge(const Vertex& begin, const Vertex& end) {
        Add(begin - 1, end - 1, 0);
    }

    virtual const std::vector<IncomingEdge>& GetNeighbors(const Vertex& vertex) const = 0;
};

class GraphList final : public IGraph {
private:
    std::vector<std::vector<IncomingEdge>> list_;

    void Add(const Vertex& begin, const Vertex& end, const WeightT& weight) override {
        list_[begin].emplace_back(end, weight);
        if (!is_oriented_) {
            list_[end].emplace_back(begin, weight);
        }
    }

public:
    explicit GraphList(const Vertex& quantity, bool oriented = false) {
        for (Vertex i = 0; i < quantity; ++i) {
            std::vector<IncomingEdge> tmp;
            list_.push_back(tmp);
        }
        q_vertex_ = quantity;
        is_oriented_ = oriented;
    }

    [[nodiscard]] const std::vector<IncomingEdge>& GetNeighbors(const Vertex& vertex) const override {
        return list_[vertex];
    }
};

//********************************************************************************************

IGraph::DistT DijkstraAlgorithm(IGraph& graph, const IGraph::Vertex& start, const IGraph::Vertex& finish) {
    std::vector<IGraph::DistT> dist(graph.GetQVertex(), kINFINITY);
    dist[start] = 0;

    std::set<IGraph::IncomingEdge> queue;
    queue.insert(IGraph::IncomingEdge(start, dist[start]));

    while (!queue.empty()) {
        IGraph::Vertex current = queue.begin()->vertex;
        queue.erase(queue.begin());
        auto neighbors = graph.GetNeighbors(current);
        for (uint32_t i = 0; i < neighbors.size(); ++i) {
            IGraph::Vertex next = neighbors[i].vertex;
            IGraph::WeightT next_weight = neighbors[i].weight;
            if (dist[current] + next_weight < dist[next]) {
                queue.erase(IGraph::IncomingEdge(next, dist[next]));
                dist[next] = dist[current] + next_weight;
                queue.insert(IGraph::IncomingEdge(next, dist[next]));
            }
        }
    }

    return dist[finish];
}

IGraph::DistT ShortPath(IGraph& graph, const IGraph::Vertex& start, const IGraph::Vertex& finish) {
    return DijkstraAlgorithm(graph, start - 1, finish - 1);
}

int main() {

    uint32_t level = 0;
    std::cin >> level;

    IGraph::WeightT level_up = 0;
    IGraph::WeightT level_down = 0;
    std::cin >> level_up >> level_down;

    IGraph::WeightT in_lift = 0;
    IGraph::WeightT from_lift = 0;
    std::cin >> in_lift >> from_lift;

    uint32_t count_lift = 0;
    std::cin >> count_lift;

    uint32_t max_level = 0;
    std::vector<std::vector<IGraph::Vertex>> lift_info;

    for (uint32_t lift_iteration = 0; lift_iteration < count_lift; ++lift_iteration) {
        std::vector<IGraph::Vertex> tmp;
        uint32_t count = 0;
        std::cin >> count;
        for (uint32_t iteration = 0; iteration < count; ++iteration) {
            uint32_t current_level = 0;
            std::cin >> current_level;
            max_level = std::max(max_level, current_level);
            tmp.push_back(current_level);
        }
        lift_info.push_back(tmp);
    }

    max_level = std::max(max_level, level);
    GraphList graph(max_level + count_lift, true);

    IGraph::Vertex not_used_vertex = max_level + 1;
    for (uint32_t lift_iteration = 0; lift_iteration < lift_info.size(); ++lift_iteration) {
        for (uint32_t iteration = 0; iteration < lift_info[lift_iteration].size(); ++iteration) {
            graph.AddWeightEdge(not_used_vertex, lift_info[lift_iteration][iteration], from_lift);
            graph.AddWeightEdge(lift_info[lift_iteration][iteration], not_used_vertex, in_lift);
        }
        ++not_used_vertex;
    }

    if (max_level != 1) {
        for (uint32_t current_level = 1; current_level <= max_level; ++current_level) {
            if (current_level == 1) {
                graph.AddWeightEdge(current_level, current_level + 1, level_up);
            } else if (current_level == max_level) {
                graph.AddWeightEdge(current_level, current_level - 1, level_down);
            } else {
                graph.AddWeightEdge(current_level, current_level + 1, level_up);
                graph.AddWeightEdge(current_level, current_level - 1, level_down);
            }
        }
    }

    std::cout << ShortPath(graph, 1, level) << std::endl;

    return 0;
}
