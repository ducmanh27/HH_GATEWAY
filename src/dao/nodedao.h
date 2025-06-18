#ifndef NODEDAO_H
#define NODEDAO_H
#include "entity/node.hxx"
#include <vector>
#include <memory>
///
/// \brief Interface for NodeDAO
///
class NodeDAO {
    public:
        virtual ~NodeDAO() = default;
        virtual void save(Node theNode) = 0;
        virtual std::shared_ptr<Node> findById(int id) = 0;
        virtual std::vector<Node> findAll() = 0;
        virtual std::vector<Node> findByStatus(std::string status) = 0;
        virtual std::shared_ptr<Node> findByMacAddress(const std::string &mac) = 0;
        virtual void update(std::shared_ptr<Node> node) = 0;
        virtual void deleteNodeById(int id) = 0;
        virtual int deleteAll() = 0;
};
#endif // NODEDAO_H
