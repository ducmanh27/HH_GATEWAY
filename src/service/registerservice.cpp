#include "registerservice.h"
#include "logger/logger.h"
#include "json.hpp"

RegisterService::RegisterService() {
}

Reply RegisterService::processRegisterMessage(const std::string &message) {
    try {
        nlohmann::json j = nlohmann::json::parse(message);
        std::string mac = j["macAddress"];
        int nodeId = 0;

        auto it = mMacToNodeId.find(mac);
        if (it != mMacToNodeId.end()) {
            nodeId = it->second;
            LOG_INFO("MAC address {} already exists with nodeId {}", mac, nodeId);
        }
        else {
            nodeId = mNextNodeId++;
            mMacToNodeId[mac] = nodeId;
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

std::map<std::string, int> RegisterService::macToNodeId() const
{
    return mMacToNodeId;
}
