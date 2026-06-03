/**
 * @file methods/methods.hpp
 * @author Mikhail Lozhnikov
 *
 * Объявления функий для серверной части алгоритмов. Эти функции должны
 * считать JSON, который прислал клиент, выполнить алгоритм и отправить клиенту
 * JSON с результатом работы алгоритма.
 */

#ifndef METHODS_METHODS_HPP_
#define METHODS_METHODS_HPP_

#include <nlohmann/json.hpp>

namespace graph {

    /* Сюда нужно вставить объявление серверной части алгоритма. */

    /**
     * @brief Серверная часть алгоритма топологической сортировки.
     *
     * Принимает JSON с описанием ориентированного графа, выполняет
     * топологическую сортировку и записывает результат в выходной JSON.
     *
     * @param input Входной JSON с полями "vertices" и "edges".
     * @param output Выходной JSON с результатом или ошибкой.
     * @return 0 при успехе, 1 при ошибке (например, обнаружен цикл).
     */
    int TopologicalSortMethod(const nlohmann::json& input,
        nlohmann::json* output);

    /* Конец вставки. */

}  // namespace graph

#endif  // METHODS_METHODS_HPP_
