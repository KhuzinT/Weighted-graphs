#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <stack>
#include <memory>
//********************************************************************************************

class IGraph {
public:
    using Vertex = int32_t;

    using WeightT = int32_t;

    inline static const int32_t kINFINITY = 1000 * 1000 * 1000 * 1LL;

protected:
    int32_t q_vertex_ = 0;

    virtual void Add(const Vertex& begin, const Vertex& end, const WeightT& weight) = 0;

public:
    [[nodiscard]] int32_t GetQVertex() const {
        return q_vertex_;
    }

    void AddWeightEdgeInNumberingFromOne(const Vertex& begin, const Vertex& end, const WeightT& weight) {
        Add(begin - 1, end - 1, weight);
    }

    void AddWeightEdgeInNumberingFromZero(const Vertex& begin, const Vertex& end, const WeightT& weight) {
        Add(begin, end, weight);
    }

    void AddEdgeInNumberingFromOne(const Vertex& begin, const Vertex& end) {
        Add(begin - 1, end - 1, 0);
    }

    void AddEdgeInNumberingFromZero(const Vertex& begin, const Vertex& end) {
        Add(begin, end, 0);
    }

    virtual std::vector<Vertex> GetNeighbors(const Vertex& current_vertex) const = 0;

    virtual Vertex GetFlowOfEdgeFromBeginToEnd(const Vertex& begin, const Vertex& end) const = 0;

    virtual void ChangeFlowOfEdgeFromBeginToEndByTheAmountOfDiff(const Vertex& begin, const Vertex& end,
                                                                 const WeightT& diff) = 0;
};

class GraphMatrix final : public IGraph {
private:
    std::vector<std::vector<Vertex>> matrix_;

    void Add(const Vertex& begin, const Vertex& end, const WeightT& weight) override {
        matrix_[begin][end] = weight;
    }

public:
    explicit GraphMatrix(const int32_t& quantity) {
        matrix_.resize(quantity, std::vector<Vertex>(quantity, 0));
        q_vertex_ = quantity;
    }

    explicit GraphMatrix(const IGraph& other_graph) : IGraph(other_graph) {
        matrix_ = dynamic_cast<const GraphMatrix*>(&other_graph)->matrix_;
    }

    [[nodiscard]] std::vector<Vertex> GetNeighbors(const Vertex& current_vertex) const override {
        std::vector<Vertex> neighbors;
        for (int32_t iteration = 0; iteration < q_vertex_; ++iteration) {
            if (matrix_[current_vertex][iteration] > 0) {
                neighbors.push_back(iteration);
            }
        }
        return neighbors;
    }

    [[nodiscard]] Vertex GetFlowOfEdgeFromBeginToEnd(const Vertex& begin, const Vertex& end) const override {
        return matrix_[begin][end];
    }

    void ChangeFlowOfEdgeFromBeginToEndByTheAmountOfDiff(const Vertex& begin, const Vertex& end,
                                                         const int32_t& diff) override {
        matrix_[begin][end] += diff;
    }
};

//********************************************************************************************

void DfsToFindFlowInGraph(IGraph& graph, const IGraph::Vertex& start, std::vector<bool>& visit,
                          std::vector<IGraph::Vertex>& parent) {
    std::stack<IGraph::Vertex> stack;
    stack.push(start);
    visit[start] = true;

    while (!stack.empty()) {
        IGraph::Vertex current_vertex = stack.top();
        stack.pop();
        auto neighbors = graph.GetNeighbors(current_vertex);
        for (auto& new_vertex : neighbors) {
            if (new_vertex != parent[current_vertex]) {
                if (!visit[new_vertex]) {
                    stack.push(new_vertex);
                    parent[new_vertex] = current_vertex;
                    visit[new_vertex] = true;
                }
            }
        }
    }
}

IGraph::WeightT GetCurrentFlow(const IGraph& graph, const IGraph::Vertex& start, const IGraph::Vertex& finish,
                               const std::vector<IGraph::Vertex>& parent) {
    IGraph::WeightT flow = IGraph::kINFINITY;

    for (IGraph::Vertex current = finish; current != start; current = parent[current]) {
        IGraph::Vertex prev = parent[current];
        flow = std::min(flow, graph.GetFlowOfEdgeFromBeginToEnd(prev, current));
    }

    return flow;
}

void ChangeCurrentFlowInGraph(IGraph& graph, const IGraph::Vertex& start, const IGraph::Vertex& finish,
                              const std::vector<IGraph::Vertex>& parent, const IGraph::WeightT& flow) {
    for (IGraph::Vertex current = finish; current != start; current = parent[current]) {
        IGraph::Vertex prev = parent[current];

        graph.ChangeFlowOfEdgeFromBeginToEndByTheAmountOfDiff(prev, current, -flow);
        graph.ChangeFlowOfEdgeFromBeginToEndByTheAmountOfDiff(current, prev, flow);
    }
}

int32_t FordFulkersonAlgorithm(const IGraph& graph, const IGraph::Vertex& start, const IGraph::Vertex& finish) {
    GraphMatrix graph_copy(graph);
    int32_t max_flow = 0;

    std::vector<bool> visit(graph_copy.GetQVertex(), false);
    std::vector<IGraph::Vertex> parent(graph_copy.GetQVertex(), IGraph::kINFINITY);

    while (true) {
        DfsToFindFlowInGraph(graph_copy, start, visit, parent);
        if (!visit[finish]) {
            break;
        }

        auto flow = GetCurrentFlow(graph_copy, start, finish, parent);
        ChangeCurrentFlowInGraph(graph_copy, start, finish, parent, flow);

        max_flow += flow;

        parent.assign(graph_copy.GetQVertex(), IGraph::kINFINITY);
        visit.assign(graph_copy.GetQVertex(), false);
    }

    return max_flow;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int32_t q_vertex = 0;
    int32_t q_edge = 0;
    std::cin >> q_vertex >> q_edge;

    GraphMatrix graph(q_vertex);

    for (int32_t iteration = 0; iteration < q_edge; ++iteration) {
        IGraph::Vertex begin = 0;
        IGraph::Vertex end = 0;
        IGraph::WeightT weight = 0;
        std::cin >> begin >> end >> weight;
        graph.AddWeightEdgeInNumberingFromOne(begin, end, weight);
    }

    auto answer = FordFulkersonAlgorithm(graph, 0, graph.GetQVertex() - 1);
    std::cout << answer << std::endl;

    return 0;
}

//********************************************************************************************
