#include "nodedaoimpl.h"
#include "node-odb.hxx"

#include <odb/transaction.hxx>
#include <odb/session.hxx>
#include <memory>
#include <vector>
#include <odb/database.hxx>
using namespace std;
using namespace odb::core;

// Với func() trả về KHÔNG PHẢI void
template <typename Func, typename Ret = std::invoke_result_t<Func>>
std::enable_if_t < !std::is_void_v<Ret>, Ret >
withTransaction(std::shared_ptr<odb::database> db, Func func) {
    odb::transaction t(db->begin());
    Ret result = func();
    t.commit();
    return result;
}

// Với func() trả về void
template <typename Func, typename Ret = std::invoke_result_t<Func>>
std::enable_if_t<std::is_void_v<Ret>, void>
withTransaction(std::shared_ptr<odb::database> db, Func func) {
    odb::transaction t(db->begin());
    func();
    t.commit();
}

NodeDAOImpl::NodeDAOImpl(std::unique_ptr<odb::database> db)
    : db_(std::move(db)) {}

void NodeDAOImpl::save(Node theNode) {
    transaction t(db_->begin());
    db_->persist(theNode);
    t.commit();
}

std::shared_ptr<Node> NodeDAOImpl::findById(int id) {
    transaction t(db_->begin());
    shared_ptr<Node> result = db_->find<Node>(id);
    t.commit();
    return result;
}

std::shared_ptr<Node> NodeDAOImpl::findByMacAddress(const std::string &mac) {
    transaction t(db_->begin());

    odb::result<Node> r = db_->query<Node>(odb::query<Node>::mac_address == mac);

    if (!r.empty()) {
        auto it = r.begin();
        std::shared_ptr<Node> result = std::make_shared<Node>(*it);
        t.commit();
        return result;
    }

    t.commit();
    return nullptr;
}

vector<Node> NodeDAOImpl::findAll() {
    transaction t(db_->begin());
    vector<Node> result;
    odb::result<Node> r = db_->query<Node>();

    for (const auto &n : r) {
        result.push_back(n);
    }

    t.commit();
    return result;
}

vector<Node> NodeDAOImpl::findByStatus(int status) {
    transaction t(db_->begin());
    vector<Node> result;
    odb::result<Node> r(db_->query<Node>(odb::query<Node>::status == status));

    for (const auto &n : r) {
        result.push_back(n);
    }

    t.commit();
    return result;
}
// TODO: test this after
// withTransaction(db_, [this, theNode]() {
//     db_->update(theNode);
// });
void NodeDAOImpl::update(std::shared_ptr<Node> node) {
    transaction t(db_->begin());
    db_->update(*node);
    t.commit();
}

void NodeDAOImpl::updateAll(const std::vector<Node> &nodes) {
    odb::transaction t(db_->begin());

    for (const auto &node : nodes) {
        db_->update(node);
    }

    t.commit();
}


void NodeDAOImpl::deleteNodeById(int id) {
    transaction t(db_->begin());
    db_->erase<Node>(id);
    t.commit();
}

int NodeDAOImpl::deleteAll() {
    transaction t(db_->begin());
    int deleted = db_->erase_query<Node>();
    t.commit();
    return deleted;
}
