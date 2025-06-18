#ifndef REGISTERSERVICE_H
#define REGISTERSERVICE_H
#include <map>
#include <string>
#include <QObject>
#include "json.hpp"

class NodeDAO;
using Reply = std::pair<std::string, std::string>;
class RegisterService : public QObject {
        Q_OBJECT
    public:
        explicit RegisterService(std::shared_ptr<NodeDAO> nodeDAO);
        Reply processRegisterMessage(const std::string &message);
        std::map<std::string, int> macToNodeId() const;

    private:
        // TODO: need to save in DB
        std::map<std::string, int> mMacToNodeId;
        int mNextNodeId = 1;
        std::shared_ptr<NodeDAO> mNodeDAO {nullptr};
    signals:
        void hasNewNodeRegister();
};

#endif // REGISTERSERVICE_H
