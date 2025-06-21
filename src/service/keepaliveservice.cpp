#include "keepaliveservice.h"
#include "dao/nodedao.h"
#include "logger/logger.h"
#include "json.hpp"
#include <QDebug>

KeepAliveService::KeepAliveService(std::shared_ptr<NodeDAO> nodeDAO,
                                   std::chrono::milliseconds sendInterval,
                                   std::chrono::milliseconds checkInterval,
                                   std::chrono::seconds timeout,
                                   std::chrono::minutes syncInterval)
    : mNodeDAO(std::move(nodeDAO)),
      mSendInterval(sendInterval),
      mCheckInterval(checkInterval),
      mTimeout(timeout),
      mSyncInterval(syncInterval) {

    getAllNode();

    mTimerSendKeepAlive = std::make_unique<QTimer>();
    mTimerCheckNodeAlive = std::make_unique<QTimer>();
    mTimerSyncToDb = std::make_unique<QTimer>();

    connect(mTimerSendKeepAlive.get(), &QTimer::timeout,
    [this]() {
        for (const auto &node : mNodes) {
            nlohmann::json replyJson = {
                {"messageType", "keepAlive"},
                {"macAddress", node.mac_address()},
                {"nodeId", node.id()}
            };
            emit hasKeepAliveMessage("gateway/request", replyJson.dump());
        }
    });
    connect(mTimerCheckNodeAlive.get(), &QTimer::timeout,
            this, &KeepAliveService::checkExistingNodeActive);
    connect(mTimerSyncToDb.get(), &QTimer::timeout,
            this, &KeepAliveService::syncToDatabase);

    mTimerSyncToDb->start(std::chrono::duration_cast<std::chrono::milliseconds>(mSyncInterval).count());
    mTimerSendKeepAlive->start(mSendInterval.count());
    mTimerCheckNodeAlive->start(mCheckInterval.count());

}

void KeepAliveService::getAllNode() {
    if (!mNodeDAO) return;
    mNodes = mNodeDAO->findAll();
    for (const auto &node : mNodes) {
        LOG_INFO("[KeepAlive] [NODE_INFO] id: {} MAC: {}, status: {}, room_id: {}",
                 node.id(), node.mac_address(), node.status(), node.room_id());
    }
}

void KeepAliveService::handleReplyKeepAlive(int nodeId, std::string macAddress) {
    LOG_DEBUG("[KeepAlive] Reply from NodeId: {}, MAC: {}", nodeId, macAddress);
    for (auto &node : mNodes) {
        if (node.id() == nodeId && node.mac_address() == macAddress) {
            if (node.status() != 1) {
                node.set_status(1);
                node.set_updated_time(datetime::now());

                if (mNodeDAO) {
                    mNodeDAO->update(std::make_shared<Node>(node));
                    LOG_INFO("[KeepAlive] Node {} status updated to ACTIVE in DB.", nodeId);
                }
            }
            else {
                node.set_updated_time(datetime::now());
                LOG_DEBUG("[KeepAlive] Node {} already active. No update status.", nodeId);
            }
        }
    }
}

void KeepAliveService::checkExistingNodeActive() {
    using namespace std::chrono;

    auto now = system_clock::now();
    datetime now_dt = datetime::now();

    for (auto &node : mNodes) {
        if (node.status() == 1) {
            auto lastUpdateTime = node.updated_time().to_time_point();
            auto duration = duration_cast<seconds>(now - lastUpdateTime);

            if (duration > mTimeout) {
                LOG_INFO("[KeepAlive] Node {} timed out ({}s). Marking INACTIVE.",
                         node.id(), duration.count());

                node.set_status(0);
                node.set_updated_time(now_dt);

                if (mNodeDAO) {
                    mNodeDAO->update(std::make_shared<Node>(node));
                    LOG_INFO("[KeepAlive] Node {} updated to INACTIVE in DB.", node.id());
                }
            }
        }
    }
}

void KeepAliveService::syncToDatabase() {
    if (!mNodeDAO) return;

    LOG_INFO("[KeepAlive] Syncing all cached node data to database...");
    mNodeDAO->updateAll(mNodes);
    LOG_INFO("[KeepAlive] Sync complete. {} nodes updated.", mNodes.size());
}
