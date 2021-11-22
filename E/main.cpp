#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <stack>
#include <memory>

//********************************************************************************************

using Vertex = int32_t;

using WeightT = int32_t;

const int32_t kInfinity = 1000 * 1000 * 1000 * 1LL;

//********************************************************************************************

class IGraph {
protected:
    int32_t q_vertex_ = 0;

    virtual void Add(const Vertex& a, const Vertex& b, const Vertex& w) = 0;

public:
    [[nodiscard]] int32_t GetQVertex() const {
        return q_vertex_;
    }

    void AddEdge(const Vertex& begin, const Vertex& end, const WeightT& weight) {
        Add(begin - 1, end - 1, weight);
    }

    virtual std::vector<Vertex> GetNeighbors(const Vertex& v) const = 0;

    virtual Vertex GetVertex(const Vertex& begin, const Vertex& end) const = 0;

    virtual void ChangeVertex(const Vertex& begin, const Vertex& end, const int32_t& diff) = 0;
};

class GraphMatrix final : public IGraph {
private:
    std::vector<std::vector<Vertex>> matrix_;

    void Add(const Vertex& a, const Vertex& b, const WeightT& w) override {
        matrix_[a][b] = w;
    }

public:
    explicit GraphMatrix(const Vertex& quantity) {
        std::vector<Vertex> tmp(quantity, 0);
        matrix_.resize(quantity, tmp);
        q_vertex_ = quantity;
    }

    [[nodiscard]] std::vector<Vertex> GetNeighbors(const Vertex& v) const override {
        return matrix_[v];
    }

    [[nodiscard]] Vertex GetVertex(const Vertex& begin, const Vertex& end) const override {
        return matrix_[begin][end];
    }

    void ChangeVertex(const Vertex& begin, const Vertex& end, const int32_t& diff) override {
        matrix_[begin][end] += diff;
    }
};

//********************************************************************************************

void DFS(IGraph& graph, const Vertex& start, std::vector<bool>& visit, std::vector<Vertex>& parent) {
    std::stack<Vertex> stack;
    stack.push(start);
    visit[start] = true;

    while (!stack.empty()) {
        Vertex v = stack.top();
        stack.pop();
        auto neighbors = graph.GetNeighbors(v);
        for (Vertex u = 0; u < graph.GetQVertex(); ++u) {
            if (neighbors[u] > 0 && u != parent[v]) {
                if (!visit[u]) {
                    stack.push(u);
                    parent[u] = v;
                    visit[u] = true;
                }
            }
        }
    }
}

int32_t FordFulkersonAlgorithm(IGraph& graph) {
    Vertex start = 0;
    Vertex finish = graph.GetQVertex() - 1;

    int32_t answer = 0;

    std::vector<bool> visit(graph.GetQVertex(), false);
    std::vector<Vertex> parent(graph.GetQVertex(), kInfinity);

    while (true) {
        DFS(graph, start, visit, parent);
        if (!visit[finish]) {
            break;
        }

        WeightT flow = kInfinity;

        for (Vertex current = finish; current != start; current = parent[current]) {
            Vertex prev = parent[current];
            flow = std::min(flow, graph.GetVertex(prev, current));
        }

        for (Vertex current = finish; current != start; current = parent[current]) {
            Vertex prev = parent[current];

            graph.ChangeVertex(prev, current, -flow);
            graph.ChangeVertex(current, prev, flow);
        }

        answer += flow;

        parent.clear();
        visit.clear();

        parent.resize(graph.GetQVertex(), kInfinity);
        visit.resize(graph.GetQVertex(), false);
    }

    return answer;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int32_t q_vertex = 0;
    int32_t q_edge = 0;
    std::cin >> q_vertex >> q_edge;

    GraphMatrix graph(q_vertex);

    for (int32_t i = 0; i < q_edge; ++i) {
        Vertex begin = 0;
        Vertex end = 0;
        WeightT weight = 0;
        std::cin >> begin >> end >> weight;
        graph.AddEdge(begin, end, weight);
    }

    auto answer = FordFulkersonAlgorithm(graph);
    std::cout << answer << std::endl;

    return 0;
}

//********************************************************************************************
