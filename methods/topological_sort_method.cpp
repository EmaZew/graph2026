/**
 * @file methods/topological_sort_method.cpp
 * @brief Серверная часть алгоритма топологической сортировки.
 */

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "oriented_graph.hpp"
#include "methods.hpp"
#include "topological_sort.hpp"

namespace graph {

    int TopologicalSortMethod(const nlohmann::json& input,
        nlohmann::json* output) {
        try {
            OrientedGraph graph;

            // Парсинг вершин
            if (input.contains("vertices")) {
                for (const auto& v : input["vertices"]) {
                    graph.AddVertex(v.get<size_t>());
                }
            }

            // Парсинг рёбер
            if (input.contains("edges")) {
                for (const auto& edge : input["edges"]) {
                    if (edge.is_array() && edge.size() >= 2) {
                        graph.AddEdge(edge[0].get<size_t>(),
                            edge[1].get<size_t>());
                    }
                }
            }

            // Выполнение алгоритма
            std::vector<size_t> result = TopologicalSort(graph);

            // Формирование ответа
            (*output)["status"] = "success";
            (*output)["result"] = result;

            return 0;
        }
        catch (const std::exception& e) {
            (*output)["status"] = "error";
            (*output)["message"] = e.what();
            return 1;
        }
    }

}  // namespace graph
