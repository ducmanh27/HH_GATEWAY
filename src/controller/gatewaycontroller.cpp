#include "gatewaycontroller.h"
#include "service/registerservice.h"
#include "mqtt/asyncsubscribe.h"
#include "mqtt/asyncpublish.h"
#include "logger/logger.h"
#include "json.hpp"
#include <chrono>
#include "service/keepaliveservice.h"
using namespace std::chrono_literals;
GatewayController::GatewayController(
    std::shared_ptr<RegisterService> registerService, std::shared_ptr<KeepAliveService> keepAliveService)
    : mRegisterService(registerService), mKeepAliveService(keepAliveService) {
    registerHandlers();

    connect(mKeepAliveService.get(), &KeepAliveService::hasKeepAliveMessage,
            this, &GatewayController::hasReplyMessage);

    connect(mRegisterService.get(), &RegisterService::hasNewNodeRegister,
            mKeepAliveService.get(), &KeepAliveService::getAllNode);
}

void GatewayController::registerHandlers() {
    registerSensorDataHandler();
    registerRequestHandler();
    registerReplyHandler();
}

void GatewayController::registerSensorDataHandler() {
    mTopicHandlers["node/sensor_data"] = [](const std::string & payload) {

    };
}

void GatewayController::registerRequestHandler() {
    mTopicHandlers["node/request"] = [this](const std::string & payload) {
        std::string messageType;
        try {
            messageType = nlohmann::json::parse(payload)["messageType"];
        }
        catch (const nlohmann::json::parse_error &e) {
            LOG_ERROR("JSON parse error: {}", e.what());
            return ;
        }
        if (messageType == "register") {
            auto reply = mRegisterService->processRegisterMessage(payload);
            emit hasReplyMessage(reply.first, reply.second);
        }

    };
}

void GatewayController::registerReplyHandler() {
    mTopicHandlers["node/reply"] = [this](const std::string & payload) {

        try {
            auto j = nlohmann::json::parse(payload);
            if (j.contains("messageType") && j["messageType"].is_string() && j["messageType"] == "keepAliveReply") {
                if (j.contains("macAddress") && j.contains("nodeId")) {
                    mKeepAliveService->handleReplyKeepAlive(j["nodeId"], j["macAddress"]);
                }
            }
        }
        catch (const nlohmann::json::parse_error &e) {
            LOG_ERROR("JSON parse error: {}", e.what());
            return ;
        }


    };
}

std::map<std::string, std::function<void(const std::string &)>> GatewayController::topicHandlers() const {
    return mTopicHandlers;
}
