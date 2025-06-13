#ifndef NODEDAOIMPL_H
#define NODEDAOIMPL_H

#include <memory>
#include <vector>
#include <string>
#include "entity/node.hxx"
#include "nodedao.h"

namespace odb {
    class database;
}

class NodeDAOImpl : public NodeDAO {
    public:
        explicit NodeDAOImpl(std::shared_ptr<odb::database> db);

        void save(Node theNode) override;
        std::shared_ptr<Node> findById(int id) override;
        std::vector<Node> findAll() override;
        std::vector<Node> findByStatus(std::string status) override;
        void update(std::shared_ptr<Node> node) override;
        void deleteNodeById(int id) override;
        int deleteAll() override;

    private:
        std::shared_ptr<odb::database> db_;
};

#endif // NODEDAOIMPL_H
