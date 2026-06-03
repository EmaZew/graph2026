/**
 * @file tests/main.cpp
 * @author Mikhail Lozhnikov
 *
 * Файл с функией main() для клиентской части программы (набора тестов).
 */

#include <httplib.h>
#include <string>
#include <cstdio>
#include "test.hpp"
#include "test_core.hpp"

int main(int argc, char* argv[]) {
    // Порт по-умолчанию.
    int port = 8080;

    // Адрес сервера по-умолчанию.
    std::string host = "127.0.0.1";

    // Обычные тесты на сборку для базовых шаблонов.
    // Сюда ничего добавлять не нужно.
    TestGraph();
    TestOrientedGraph();
    TestWeightedGraph();
    TestWeightedOrientedGraph();

    if (argc >= 2) {
        // Меняем хост, если предоставлен соответствующий аргумент командной строки.
        host = std::string(argv[1]);
    }

    if (argc >= 3) {
        // Меняем порт, если предоставлен соответствующий аргумент командной строки.
        if (std::sscanf(argv[2], "%d", &port) != 1)
            return -1;
    }

    // Адрес подключения в формате хост:порт.
    std::string url = host + ":" + std::to_string(port);

    httplib::Client cli(url.c_str());

    /* Сюда нужно вставить вызов набора тестов для алгоритма. */
    TestTopologicalSort();
    /* Конец вставки. */

      // Отправляем GET запрос для остановки сервера.
    httplib::Result res = cli.Get("/stop");

    // ВАЖНО: Проверяем, удалось ли подключиться к серверу
    if (!res) {
        std::cerr << "ОШИБКА: Не удалось подключиться к серверу по адресу "
            << url << std::endl;
        std::cerr << "Убедитесь, что graph_server запущен в другом окне/терминале!" << std::endl;
        return -1;
    }

    if (res->status != 200) {
        std::cerr << "ОШИБКА: Сервер вернул неожиданный статус: " << res->status << std::endl;
        return -1;
    }

    return TestSuite::Status();
}