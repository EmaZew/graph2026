/*
 * @file methods/main.cpp
 * @author Mikhail Lozhnikov
 *
 * Файл с функцией main() для серверной части программы.
 */
#include <httplib.h>

#include <cstdio>
#include <iostream>
#include <string>

#include <nlohmann/json.hpp>
#include "methods.hpp"



int main(int argc, char* argv[]) {
    // Порт по-умолчанию.
    int port = 8080;

    if (argc >= 2) {
        // Меняем порт по умолчанию, если предоставлен соответствующий
        // аргумент командной строки.
        if (std::sscanf(argv[1], "%d", &port) != 1) {
            return -1;
        }
    }

    std::cerr << "Listening on port " << port << "..." << std::endl;

    httplib::Server svr;

    // Обработчик для GET запроса по адресу /stop. Этот обработчик
    // останавливает сервер.
    svr.Get("/stop", [&](const httplib::Request& req,
        httplib::Response& res) {
            svr.stop();
            res.set_content("OK", "text/plain");
        });

    /* Сюда нужно вставить обработчик post запроса для алгоритма. */

    // Обработчик POST запроса для топологической сортировки.
    svr.Post("/TopologicalSort", [](const httplib::Request& req,
        httplib::Response& res) {
            nlohmann::json input;
            try {
                input = nlohmann::json::parse(req.body);
            }
            catch (const std::exception& e) {
                nlohmann::json error_output;
                error_output["status"] = "error";
                error_output["message"] = std::string("Invalid JSON: ") +
                    e.what();
                res.status = 400;
                res.set_content(error_output.dump(), "application/json");
                return;
            }

            nlohmann::json output;
            int status = graph::TopologicalSortMethod(input, &output);

            if (status == 0) {
                res.status = 200;
            } else {
                res.status = 400;
            }
            res.set_content(output.dump(), "application/json");
        });

    /* Конец вставки. */

    // Эта функция запускает сервер на указанном порту. Программа не
    // завершится до тех пор, пока сервер не будет остановлен.
    svr.listen("0.0.0.0", port);

    return 0;
}
