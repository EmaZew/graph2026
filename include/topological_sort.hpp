/**
 * @file include/topological_sort.hpp
 * @brief Реализация алгоритма топологической сортировки.
 */

#ifndef INCLUDE_TOPOLOGICAL_SORT_HPP_
#define INCLUDE_TOPOLOGICAL_SORT_HPP_

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>
#include <functional>

namespace graph {

    /**
     * @brief Топологическая сортировка ориентированного графа.
     *
     * Алгоритм использует DFS с обнаружением циклов через три цвета
     * вершин (белый, серый, чёрный).
     *
     * @param graph Ориентированный граф (должен иметь методы Vertices()
     *              и Edges(id)).
     * @return Вектор вершин в топологическом порядке.
     * @throws std::runtime_error Если граф содержит цикл.
     *
     * Сложность: O(V + E), где V — количество вершин, E — количество рёбер.
     *
     * Алгоритм основан на методе, описанном на
     * http://e-maxx.ru/algo/topological_sort
     */
    template <typename GraphType>
    std::vector<size_t> TopologicalSort(const GraphType& graph) {
        std::vector<size_t> result;
        // 0 - белый (не посещена), 1 - серый (в процессе обхода),
        // 2 - чёрный (завершена)
        std::unordered_map<size_t, int> color;

        // Инициализация: все вершины белые
        for (const auto& v : graph.Vertices()) {
            color[v] = 0;
        }

        // DFS функция
        std::function<void(size_t)> dfs = [&](size_t v) {
            color[v] = 1;  // Помечаем как серый

            for (const auto& to : graph.Edges(v)) {
                if (color[to] == 0) {
                    dfs(to);
                }
                else if (color[to] == 1) {
                    // Обнаружен цикл: нашли серую вершину
                    throw std::runtime_error(
                        "Graph contains a cycle, "
                        "topological sort is impossible");
                }
            }

            color[v] = 2;  // Помечаем как чёрную
            result.push_back(v);
            };

        // Запуск DFS для всех непосещённых вершин
        for (const auto& v : graph.Vertices()) {
            if (color[v] == 0) {
                dfs(v);
            }
        }

        // Разворачиваем результат (вершины добавлялись в порядке
        // убывания времён выхода)
        std::reverse(result.begin(), result.end());
        return result;
    }

}  // namespace graph

#endif  // INCLUDE_TOPOLOGICAL_SORT_HPP_
