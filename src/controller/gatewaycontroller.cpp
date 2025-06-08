#include "gatewaycontroller.h"
#include "service/registerservice.h"
#include "mqtt/asyncsubscribe.h"
#include "mqtt/asyncpublish.h"
#include "logger/logger.h"
#include "json.hpp"

GatewayController::GatewayController(
    std::shared_ptr<RegisterService> registerService)
    : mRegisterService(registerService) {
    registerHandlers();
}

void GatewayController::registerHandlers() {
    registerSensorDataHandler();
    registerRequestHandler();
}

void GatewayController::registerSensorDataHandler() {
    mTopicHandlers["node/sensor_data"] = [](const std::string & payload) {
        LOG_INFO("Payload xxx: {}", payload);

    };
}

void GatewayController::registerRequestHandler() {
    mTopicHandlers["node/request"] = [this](const std::string & payload) {
        nlohmann::json j;
        try {
            j = nlohmann::json::parse(payload);
            for (auto it = j.begin(); it != j.end(); ++it) {
                LOG_INFO("{}: {}", it.key(), it.value().dump());
            }
        }
        catch (const nlohmann::json::parse_error &e) {
            LOG_ERROR("JSON parse error: {}", e.what());
            return ;
        }
        catch (const std::exception &e) {
            LOG_ERROR("Exception: {}", e.what());
            return ;
        }
        std::string messageType = j["messageType"];
        if (messageType == "register") {
            auto reply = mRegisterService->processRegisterMessage(payload);
            emit hasReplyMessage(reply.first, reply.second);
        }

    };
}

std::map<std::string, std::function<void(const std::string &)>> GatewayController::topicHandlers() const {
    return mTopicHandlers;
}
