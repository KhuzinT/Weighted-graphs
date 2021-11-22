#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>

//********************************************************************************************

using Vertex = uint64_t;

using WeightT = uint64_t;

const uint64_t kInfinity = 1000 * 1000 * 1000 * 1LL;

//********************************************************************************************

struct Neighbor {
    Vertex vertex;
    WeightT weight;

    Neighbor() {
        vertex = 0;
        weight = 0;
    }

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

        if (vertex < other.vertex) {
            return true;
        }

        return false;
    }
};

class GraphList {
private:
    std::vector<std::vector<Neighbor>> list_;

    uint64_t q_vertex_ = 0;

    void Add(const Vertex& begin, const Vertex& end, const WeightT& weight) {
        Neighbor v(end, weight);
        Neighbor u(begin, weight);

        list_[begin].push_back(v);
        list_[end].push_back(u);
    }

public:
    explicit GraphList(const uint64_t& quantity) {
        q_vertex_ = quantity;
        std::vector<Neighbor> tmp;
        list_.resize(quantity, tmp);
    }

    void AddEdge(const Vertex& begin, const Vertex& end, const WeightT& weight) {
        Add(begin - 1, end - 1, weight);
    }

    uint64_t GetQVertex() const {
        return q_vertex_;
    }

    [[nodiscard]] std::vector<Neighbor> GetNeighbors(const Vertex& v) const {
        return list_[v];
    }
};

//********************************************************************************************

uint64_t PrimAlgorithm(GraphList& g) {
    std::vector<WeightT> dist(g.GetQVertex(), kInfinity);
    std::vector<Neighbor> prev(g.GetQVertex());

    dist[0] = 0;
    std::set<Neighbor> queue;
    for (uint64_t v = 0; v < g.GetQVertex(); ++v) {
        queue.insert(Neighbor(v, dist[v]));
    }

    while (!queue.empty()) {
        Vertex v = queue.begin()->vertex;
        queue.erase(queue.begin());

        auto neighbors = g.GetNeighbors(v);
        for (auto& end_of_edge : neighbors) {
            Vertex u = end_of_edge.vertex;
            WeightT w = end_of_edge.weight;

            Neighbor current(u, dist[u]);
            if (queue.end() != queue.find(current) && w < dist[u]) {
                queue.erase(current);

                Neighbor tmp(v, w);
                prev[u] = tmp;
                dist[u] = w;
                current.weight = w;

                queue.insert(current);
            }
        }
    }

    uint64_t answer = 0;
    for (uint64_t i = 0; i < g.GetQVertex(); ++i) {
        answer += prev[i].weight;
    }
    return answer;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    uint64_t q_vertex = 0;
    uint64_t q_edge = 0;

    std::cin >> q_vertex >> q_edge;

    GraphList g(q_vertex);

    for (uint64_t i = 0; i < q_edge; ++i) {
        Vertex begin = 0;
        Vertex end = 0;
        WeightT weight = 0;
        std::cin >> begin >> end >> weight;
        g.AddEdge(begin, end, weight);
    }

    auto answer = PrimAlgorithm(g);

    std::cout << answer << std::endl;
}

//********************************************************************************************
