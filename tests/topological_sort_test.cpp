/**
 * @file tests/topological_sort_test.cpp
 * @brief Тесты для алгоритма топологической сортировки.
 */

#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include "oriented_graph.hpp"
#include "test.hpp"
#include "test_core.hpp"
#include "topological_sort.hpp"

using graph::OrientedGraph;
using graph::TopologicalSort;

// Вспомогательная функция для проверки равенства
static void CheckEqual(size_t actual, size_t expected,
    const std::string& msg) {
    if (actual != expected) {
        std::cerr << "FAIL: " << msg
            << " (Expected: " << expected
            << ", Got: " << actual << ")" << std::endl;
        throw std::runtime_error("Test failed");
    }
}

// Вспомогательная функция для проверки условия
static void CheckTrue(bool condition, const std::string& msg) {
    if (!condition) {
        std::cerr << "FAIL: " << msg << std::endl;
        throw std::runtime_error("Test failed");
    }
}

void TestTopologicalSort() {
    TestSuite suite("TestTopologicalSort");

    try {
        // Тест 1: Линейный граф 1 -> 2 -> 3
        {
            OrientedGraph g;
            g.AddVertex(1);
            g.AddVertex(2);
            g.AddVertex(3);
            g.AddEdge(1, 2);
            g.AddEdge(2, 3);

            auto result = TopologicalSort(g);
            CheckEqual(result.size(), 3, "LinearGraph size");
            CheckEqual(result[0], 1, "LinearGraph [0]");
            CheckEqual(result[1], 2, "LinearGraph [1]");
            CheckEqual(result[2], 3, "LinearGraph [2]");
            std::cout << "PASS: LinearGraphTest" << std::endl;
        }

        // Тест 2: Граф-ромб
        {
            OrientedGraph g;
            g.AddVertex(1);
            g.AddVertex(2);
            g.AddVertex(3);
            g.AddVertex(4);
            g.AddEdge(1, 2);
            g.AddEdge(1, 3);
            g.AddEdge(2, 4);
            g.AddEdge(3, 4);

            auto result = TopologicalSort(g);
            CheckEqual(result.size(), 4, "DiamondGraph size");
            CheckEqual(result[0], 1, "DiamondGraph start");
            CheckEqual(result[3], 4, "DiamondGraph end");
            std::cout << "PASS: DiamondGraphTest" << std::endl;
        }

        // Тест 3: Несколько источников
        {
            OrientedGraph g;
            g.AddVertex(1);
            g.AddVertex(2);
            g.AddVertex(3);
            g.AddVertex(4);
            g.AddEdge(1, 3);
            g.AddEdge(2, 3);
            g.AddEdge(3, 4);

            auto result = TopologicalSort(g);
            CheckEqual(result.size(), 4, "MultipleSources size");

            std::unordered_map<size_t, size_t> pos;
            for (size_t i = 0; i < result.size(); ++i) {
                pos[result[i]] = i;
            }

            CheckTrue(pos[1] < pos[3], "MultipleSources 1<3");
            CheckTrue(pos[2] < pos[3], "MultipleSources 2<3");
            CheckTrue(pos[3] < pos[4], "MultipleSources 3<4");
            std::cout << "PASS: MultipleSourcesTest" << std::endl;
        }

        // Тест 4: Одна вершина
        {
            OrientedGraph g;
            g.AddVertex(42);
            auto result = TopologicalSort(g);
            CheckEqual(result.size(), 1, "SingleVertex size");
            CheckEqual(result[0], 42, "SingleVertex value");
            std::cout << "PASS: SingleVertexTest" << std::endl;
        }

        // Тест 5: Пустой граф
        {
            OrientedGraph g;
            auto result = TopologicalSort(g);
            CheckEqual(result.size(), 0, "EmptyGraph size");
            std::cout << "PASS: EmptyGraphTest" << std::endl;
        }

        // Тест 6: Обнаружение цикла
        {
            OrientedGraph g;
            g.AddVertex(1);
            g.AddVertex(2);
            g.AddVertex(3);
            g.AddEdge(1, 2);
            g.AddEdge(2, 3);
            g.AddEdge(3, 1);  // Цикл

            bool caught = false;
            try {
                TopologicalSort(g);
            }
            catch (const std::runtime_error&) {
                caught = true;
            }
            CheckTrue(caught, "CycleDetection must throw runtime_error");
            std::cout << "PASS: CycleDetectionTest" << std::endl;
        }

        // Тест 7: Случайный DAG
        {
            std::mt19937 gen(42);
            std::uniform_int_distribution<size_t> size_dist(5, 30);
            size_t n = size_dist(gen);

            OrientedGraph g;
            for (size_t i = 0; i < n; ++i) {
                g.AddVertex(i);
            }

            std::uniform_int_distribution<size_t> vertex_dist(0, n - 1);
            for (size_t attempt = 0; attempt < n * 3; ++attempt) {
                size_t u = vertex_dist(gen);
                size_t v = vertex_dist(gen);
                if (u < v && !g.HasEdge(u, v)) {
                    g.AddEdge(u, v);
                }
            }

            auto result = TopologicalSort(g);
            CheckEqual(result.size(), n, "RandomDAG size");

            std::unordered_map<size_t, size_t> pos;
            for (size_t i = 0; i < result.size(); ++i) {
                pos[result[i]] = i;
            }

            for (const auto& v : g.Vertices()) {
                for (const auto& to : g.Edges(v)) {
                    CheckTrue(pos[v] < pos[to], "RandomDAG order violation");
                }
            }
            std::cout << "PASS: RandomDAGTest" << std::endl;
        }

        std::cout << "ALL TOPOLOGICAL SORT TESTS PASSED!" << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "TEST FAILED: " << e.what() << std::endl;
        throw;
    }
}
