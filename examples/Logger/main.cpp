#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <chrono>

// #include <odb/database.hxx>
// #include <odb/pgsql/database.hxx>
// #include <odb/transaction.hxx>
// #include <odb/session.hxx>

// #include "dao/nodedaoimpl.h"
// #include "entity/node.hxx"
#include "logger/logger.h"
// using namespace std;
// using namespace odb::core;

// datetime now() {
//     using namespace std::chrono;
//     auto tp = system_clock::now();
//     auto s = duration_cast<seconds>(tp.time_since_epoch()).count();
//     std::time_t t = static_cast<std::time_t>(s);
//     std::tm *tm_ptr = std::localtime(&t);

//     return datetime(
//                tm_ptr->tm_year + 1900,
//                tm_ptr->tm_mon + 1,
//                tm_ptr->tm_mday,
//                tm_ptr->tm_hour,
//                tm_ptr->tm_min,
//                tm_ptr->tm_sec
//            );
// }

// void print_node(const Node &node) {
//     LOG_DEBUG("Node ID: {}, Status: {}, Room ID: {}, Location: ({}, {}), KeepAlive: {}",
//               node.id(), node.status(), node.room_id(),
//               node.latitude(), node.longitude(),
//               node.is_keep_alive() ? "true" : "false");

//     std::cout << "created " << node.created_time();
// }

int main() {
    mpd::Logger::getInstance()->initLogger("test-crud");
    // try {
    //     // Khởi tạo kết nối DB
    //     std::unique_ptr<odb::database> pgdb = std::make_unique<odb::pgsql::database>("admin", "secret", "gatewaydb", "localhost", 5432);
    //     auto dao = std::make_unique<NodeDAOImpl>(std::move(pgdb));

    // }
    // catch (const odb::exception &e) {
    //     LOG_ERROR("[ODB Exception] {}", e.what());
    //     return 1;
    // }

    return 0;
}
