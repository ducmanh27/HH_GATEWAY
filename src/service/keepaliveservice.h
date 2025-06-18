#ifndef KEEPALIVESERVICE_H
#define KEEPALIVESERVICE_H

#include <memory>
#include <QTimer>
#include <QObject>
#include <entity/node.hxx>
#include <chrono>

class NodeDAO;

using namespace std::chrono_literals;

class KeepAliveService : public QObject {
        Q_OBJECT
    public:
        explicit KeepAliveService(std::shared_ptr<NodeDAO> nodeDAO,
                                  std::chrono::milliseconds sendInterval = 2000ms,
                                  std::chrono::milliseconds checkInterval = 5000ms,
                                  std::chrono::seconds timeout = 5s,
                                  std::chrono::minutes syncInterval = 5min);

        void getAllNode();
        void handleReplyKeepAlive(int nodeId, std::string macAddress);
        void checkExistingNodeActive();
        void syncToDatabase();
    signals:
        void hasKeepAliveMessage(const std::string &topic, const std::string &msg);

    private:
        std::vector<Node> mNodes;
        std::shared_ptr<NodeDAO> mNodeDAO {nullptr};

        std::unique_ptr<QTimer> mTimerSendKeepAlive {nullptr};
        std::unique_ptr<QTimer> mTimerCheckNodeAlive {nullptr};
        std::unique_ptr<QTimer> mTimerSyncToDb {nullptr};

        std::chrono::milliseconds mSendInterval {2000ms};
        std::chrono::milliseconds mCheckInterval {5000ms};
        std::chrono::seconds mTimeout {5s};
        std::chrono::minutes mSyncInterval {5min};
};

#endif // KEEPALIVESERVICE_H
