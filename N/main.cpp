#include <iostream>
#include <vector>

using WeightT = int32_t;

const int32_t kInfinity = 1000 * 1000 * 1000 * 1LL;

class IGraph {
protected:
    uint32_t q_vertex_ = 0;

public:
    uint32_t GetQVertex() const {
        return q_vertex_;
    }

    virtual std::vector<std::vector<WeightT>> GetMatrix() const = 0;
};

class GraphMatrix final : public IGraph {
private:
    std::vector<std::vector<WeightT>> matrix_;

public:
    explicit GraphMatrix(const uint32_t& quantity) {
        std::vector<WeightT> tmp(quantity, kInfinity);
        matrix_.resize(quantity, tmp);

        q_vertex_ = quantity;
    }

    void FillMatrix(const std::vector<std::vector<WeightT>>& matrix) {
        for (uint32_t i = 0; i < q_vertex_; ++i) {
            for (uint32_t j = 0; j < q_vertex_; ++j) {
                matrix_[i][j] = matrix[i][j];
            }
        }
    }

    std::vector<std::vector<WeightT>> GetMatrix() const override {
        return matrix_;
    }
};

std::vector<std::vector<WeightT>> FloydWarshallAlgorithm(GraphMatrix& graph) {
    auto matrix = graph.GetMatrix();
    for (uint32_t t = 0; t < graph.GetQVertex(); ++t) {
        for (uint32_t i = 0; i < graph.GetQVertex(); ++i) {
            for (uint32_t j = 0; j < graph.GetQVertex(); ++j) {
                matrix[i][j] = std::min(matrix[i][j], matrix[i][t] + matrix[t][j]);
            }
        }
    }

    return matrix;
}

void PrintMatrix(std::vector<std::vector<WeightT>>& matrix) {
    for (uint32_t i = 0; i < matrix.size(); ++i) {
        for (uint32_t j = 0; j < matrix[0].size(); ++j) {
            std::cout << matrix[i][j] << ' ';
        }
        std::cout << std::endl;
    }
}

int main() {
    uint32_t q_vertex = 0;
    std::cin >> q_vertex;

    GraphMatrix graph(q_vertex);

    std::vector<std::vector<WeightT>> matrix;

    for (uint32_t i = 0; i < q_vertex; ++i) {
        std::vector<WeightT> tmp;
        for (uint32_t j = 0; j < q_vertex; ++j) {
            WeightT weight = 0;
            std::cin >> weight;
            tmp.push_back(weight);
        }
        matrix.push_back(tmp);
    }

    graph.FillMatrix(matrix);

    auto answer = FloydWarshallAlgorithm(graph);
    PrintMatrix(answer);

    return 0;
}