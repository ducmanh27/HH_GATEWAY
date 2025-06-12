#include "gatewaycontroller.h"
#include "service/registerservice.h"
#include "mqtt/asyncsubscribe.h"
#include "mqtt/asyncpublish.h"
#include "logger/logger.h"
#include "json.hpp"
#include <chrono>
using namespace std::chrono_literals;
GatewayController::GatewayController(
    std::shared_ptr<RegisterService> registerService)
    : mRegisterService(registerService) {
    registerHandlers();
    mTimerKeepAlive = std::make_unique<QTimer>(this);
    connect(mTimerKeepAlive.get(), &QTimer::timeout, this, [this]() {
        auto nodes = mRegisterService->macToNodeId();
        for (auto node : nodes) {
            // check if status == active && chưa được bản tin keep alive
            // nếu không thì set nó về inactive
            // phía lúc nhận bản tin keepALiveReply thì cần reset lại biến này
            nlohmann::json msg;
            msg["macAddress"] = node.first;
            msg["nodeId"] = node.second;
            msg["messageType"] = "keepAlive";
            emit hasReplyMessage("gateway/request", msg.dump());
        }
    });
    mTimerKeepAlive->setInterval(15s);
    mTimerKeepAlive->start();
}

void GatewayController::registerHandlers() {
    registerSensorDataHandler();
    registerRequestHandler();
    registerReplyHandler();
}

void GatewayController::registerSensorDataHandler() {
    mTopicHandlers["node/sensor_data"] = [](const std::string & payload) {
        LOG_INFO("Payload xxx: {}", payload);

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
        std::string messageType;
        try {
            messageType = nlohmann::json::parse(payload)["messageType"];
        }
        catch (const nlohmann::json::parse_error &e) {
            LOG_ERROR("JSON parse error: {}", e.what());
            return ;
        }
        if (messageType == "keepAliveReply") {
            LOG_DEBUG("[KeepAlive] Message: {}", nlohmann::json::parse(payload).dump());
        }

    };
}

std::map<std::string, std::function<void(const std::string &)>> GatewayController::topicHandlers() const {
    return mTopicHandlers;
}
