#include "registerservice.h"
#include "logger/logger.h"
#include "json.hpp"
#include "dao/nodedao.h"


RegisterService::RegisterService(std::shared_ptr<NodeDAO> nodeDAO) :
    mNodeDAO(nodeDAO) {
}

Reply RegisterService::processRegisterMessage(const std::string &message) {
    try {
        nlohmann::json j = nlohmann::json::parse(message);
        std::string mac = j["macAddress"];
        int nodeId = 0;

        std::shared_ptr<Node> existingNode = mNodeDAO->findByMacAddress(mac);

        if (existingNode) {
            nodeId = static_cast<int>(existingNode->id());
            LOG_INFO("MAC address {} already exists with nodeId {}", mac, nodeId);
        }
        else {
            Node newNode;
            newNode.set_mac_address(mac);
            newNode.set_status("new");
            newNode.set_room_id(0);
            newNode.set_latitude(0.0);
            newNode.set_longitude(0.0);
            newNode.set_is_keep_alive(false);
            datetime now = datetime::now();
            newNode.set_created_time(now);
            newNode.set_updated_time(now);

            // Save and get ID
            mNodeDAO->save(newNode);

            // Refetch to get auto-generated ID
            std::shared_ptr<Node> savedNode = mNodeDAO->findByMacAddress(mac);
            nodeId = static_cast<int>(savedNode->id());

            LOG_INFO("MAC address {} registered with new nodeId {}", mac, nodeId);
        }

        nlohmann::json replyJson = {
            {"messageType", "registerReply"},
            {"macAddress", mac},
            {"nodeId", nodeId}
        };

        return std::make_pair("gateway/reply", replyJson.dump());
    }
    catch (const std::exception &e) {
        LOG_ERROR("Failed to process register message: {}", e.what());
        return std::make_pair("error", "invalid register message");
    }
}

std::map<std::string, int> RegisterService::macToNodeId() const {
    return mMacToNodeId;
}
