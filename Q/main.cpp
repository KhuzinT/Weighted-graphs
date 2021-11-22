#include <iostream>
#include <algorithm>
#include <vector>
#include <set>

//********************************************************************************************

using Vertex = uint32_t;

using DistT = uint32_t;

using WeightT = uint32_t;

const uint32_t kPoison = 1000 * 1000 * 1000 * 1LL;

//********************************************************************************************

struct Neighbor {
    Vertex vertex;
    WeightT weight;

    explicit Neighbor(const Vertex& v, const WeightT& w) {
        vertex = v;
        weight = w;
    }

    bool operator<(const Neighbor& other) const {
        if (weight < other.weight) {
            return true;
        }
        if (weight > other.weight) {
            return false;
        }
        return vertex < other.vertex;
    }
};

//********************************************************************************************

class IGraph {
protected:
    uint32_t q_vertex_ = 0;

    bool is_oriented_ = false;

    virtual void Add(const Vertex& a, const Vertex& b, const WeightT& w) = 0;

public:
    [[nodiscard]] uint32_t GetQVertex() const {
        return q_vertex_;
    }

    void AddEdge(const Vertex& begin, const Vertex& end, const WeightT& weight) {
        Add(begin - 1, end - 1, weight);
    }

    virtual const std::vector<Neighbor>& GetNeighbors(const Vertex& v) const = 0;
};

class GraphList final : public IGraph {
private:
    std::vector<std::vector<Neighbor>> list_;

    void Add(const Vertex& a, const Vertex& b, const WeightT& w) override {
        list_[a].emplace_back(b, w);
        if (!is_oriented_) {
            list_[b].emplace_back(a, w);
        }
    }

public:
    explicit GraphList(const Vertex& quantity, bool oriented = false) {
        for (Vertex i = 0; i < quantity; ++i) {
            std::vector<Neighbor> tmp;
            list_.push_back(tmp);
        }
        q_vertex_ = quantity;
        is_oriented_ = oriented;
    }

    [[nodiscard]] const std::vector<Neighbor>& GetNeighbors(const Vertex& v) const override {
        return list_[v];
    }
};

//********************************************************************************************

DistT DijkstraAlgorithm(GraphList& graph, const Vertex& start, const Vertex& finish) {
    std::vector<DistT> dist(graph.GetQVertex(), kPoison);
    dist[start] = 0;

    std::set<Neighbor> queue;
    queue.insert(Neighbor(start, dist[start]));

    while (!queue.empty()) {
        Vertex current = queue.begin()->vertex;
        queue.erase(queue.begin());
        auto neighbors = graph.GetNeighbors(current);
        for (uint32_t i = 0; i < neighbors.size(); ++i) {
            Vertex next = neighbors[i].vertex;
            WeightT next_weight = neighbors[i].weight;
            if (dist[current] + next_weight < dist[next]) {
                queue.erase(Neighbor(next, dist[next]));
                dist[next] = dist[current] + next_weight;
                queue.insert(Neighbor(next, dist[next]));
            }
        }
    }

    return dist[finish];
}

DistT ShortPath(GraphList& graph, const Vertex& start, const Vertex& finish) {
    return DijkstraAlgorithm(graph, start - 1, finish - 1);
}

int main() {

    uint32_t level = 0;
    std::cin >> level;

    WeightT level_up = 0;
    WeightT level_down = 0;
    std::cin >> level_up >> level_down;

    WeightT in_lift = 0;
    WeightT from_lift = 0;
    std::cin >> in_lift >> from_lift;

    uint32_t count_lift = 0;
    std::cin >> count_lift;

    uint32_t max_level = 0;
    std::vector<std::vector<Vertex>> lift_info;

    for (uint32_t i = 0; i < count_lift; ++i) {
        std::vector<Vertex> tmp;
        uint32_t count = 0;
        std::cin >> count;
        for (uint32_t j = 0; j < count; ++j) {
            uint32_t current_level = 0;
            std::cin >> current_level;
            max_level = std::max(max_level, current_level);
            tmp.push_back(current_level);
        }
        lift_info.push_back(tmp);
    }

    max_level = std::max(max_level, level);
    GraphList graph(max_level + count_lift, true);

    Vertex not_used_vertex = max_level + 1;
    for (uint32_t i = 0; i < lift_info.size(); ++i) {
        for (uint32_t j = 0; j < lift_info[i].size(); ++j) {
            graph.AddEdge(not_used_vertex, lift_info[i][j], from_lift);
            graph.AddEdge(lift_info[i][j], not_used_vertex, in_lift);
        }
        ++not_used_vertex;
    }

    if (max_level != 1) {
        for (uint32_t current_level = 1; current_level <= max_level; ++current_level) {
            if (current_level == 1) {
                graph.AddEdge(current_level, current_level + 1, level_up);
            } else if (current_level == max_level) {
                graph.AddEdge(current_level, current_level - 1, level_down);
            } else {
                graph.AddEdge(current_level, current_level + 1, level_up);
                graph.AddEdge(current_level, current_level - 1, level_down);
            }
        }
    }

    std::cout << ShortPath(graph, 1, level) << std::endl;

    return 0;
}
