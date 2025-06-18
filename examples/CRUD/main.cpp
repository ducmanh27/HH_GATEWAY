#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <chrono>

#include <odb/database.hxx>
#include <odb/pgsql/database.hxx>
#include <odb/transaction.hxx>
#include <odb/session.hxx>

#include "dao/nodedaoimpl.h"
#include "entity/node.hxx"
#include "logger/logger.h"
using namespace std;
using namespace odb::core;

datetime now() {
    using namespace std::chrono;
    auto tp = system_clock::now();
    auto s = duration_cast<seconds>(tp.time_since_epoch()).count();
    std::time_t t = static_cast<std::time_t>(s);
    std::tm *tm_ptr = std::localtime(&t);

    return datetime(
               tm_ptr->tm_year + 1900,
               tm_ptr->tm_mon + 1,
               tm_ptr->tm_mday,
               tm_ptr->tm_hour,
               tm_ptr->tm_min,
               tm_ptr->tm_sec
           );
}

void print_node(const Node &node) {
    LOG_DEBUG("Node ID: {}, Status: {}, Room ID: {}, Location: ({}, {}), KeepAlive: {}",
              node.id(), node.status(), node.room_id(),
              node.latitude(), node.longitude(),
              node.is_keep_alive() ? "true" : "false");

    std::cout << "created " << node.created_time();
}

int main() {
    mpd::Logger::getInstance()->initLogger("test-crud");
    try {
        // Khởi tạo kết nối DB
        auto pgdb = std::make_shared<odb::pgsql::database>("admin", "secret", "gatewaydb", "localhost", 5432);
        std::unique_ptr<NodeDAOImpl> dao = std::make_unique<NodeDAOImpl>(pgdb);

        // // ======= SAVE ========
        // {
        //     datetime created = now();
        //     datetime updated = created;

        //     Node n1("ACTIVE", 101, 21.03, 105.85, true, created, updated);
        //     Node n2("INACTIVE", 102, 22.02, 106.02, false, created, updated);
        //     Node n3("ACTIVE", 103, 20.99, 105.70, true, created, updated);

        //     dao->save(n1);
        //     dao->save(n2);
        //     dao->save(n3);
        // }

        // // ======= FIND ALL ========
        // {
        //     LOG_INFO("All Nodes:");
        //     for (const auto &node : dao->findAll()) {
        //         print_node(node);
        //     }
        // }

        // // ======= FIND BY STATUS ========
        // {
        //     LOG_INFO("Nodes with status = ACTIVE:");
        //     for (const auto &node : dao->findByStatus("ACTIVE")) {
        //         print_node(node);
        //     }
        // }

        // ======= FIND BY ID ========
        // {
        //     LOG_INFO("Find Node by ID = 4:");
        //     auto n = dao->findById(4);
        //     if (n != nullptr) {
        //         print_node(*n);
        //     }
        // }

        // ======= UPDATE ========
        {
            auto n_ptr = dao->findById(4);
            if (n_ptr) {
                datetime new_updated = now();

                // Cập nhật trực tiếp vào object bên trong shared_ptr
                n_ptr->set_status("XXXR");
                n_ptr->set_updated_time(new_updated);

                dao->update(n_ptr);

                LOG_INFO("After update Node ID = 4:");
                print_node(*dao->findById(4));
            }
            else {
                LOG_ERROR("Cannot update. Node ID 1 not found.");
            }
        }

        // // ======= DELETE NODE ID = 2 ========
        // {
        //     dao->deleteNodeById(2);
        //     LOG_INFO("Remaining nodes:");
        //     for (const auto &node : dao->findAll()) {
        //         print_node(node);
        //     }
        // }

        // // ======= DELETE ALL ========
        // {
        //     int deleted = dao->deleteAll();
        //     LOG_INFO("Deleted {} node(s) from DB.", deleted);
        // }
    }
    catch (const odb::exception &e) {
        LOG_ERROR("[ODB Exception] {}", e.what());
        return 1;
    }

    return 0;
}
