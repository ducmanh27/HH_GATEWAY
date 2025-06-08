#include "configmanager.h"
#include "controller/gatewaycontroller.h"
#include "service/registerservice.h"
#include "mqtt/asyncsubscribe.h"
#include "mqtt/asyncpublish.h"
#include "service/registerservice.h"
#include "mqtt/asyncpublish.h"
#include <chrono>
using namespace std::chrono_literals;
ConfigManager &ConfigManager::getInstance() {
    static ConfigManager instance; // Initial once, thread-safe from C++11
    return instance;
}

void ConfigManager::initSystem() {
    // Service
    mRegisterService = std::make_shared<RegisterService>();

    // Controller
    mGatewayController = std::make_shared<GatewayController>(mRegisterService);

    // Transport
    mSub = AsyncSubscribe::AsyncSubscribeBuilder()
           .setHost("localhost")
           .setPort(1883)
           .setClientId("XXX123")
           .setTopic("#")
           .setQos(0)
           .setRetryAttempts(5)
           .setTopicHandlerMap(mGatewayController->topicHandlers())
           .build();
    mPub = AsyncPublish::AsyncPublishBuilder()
           .setHost("localhost")
           .setPort(1883)
           .setClientId("test-pub")
           .setQos(0)
           .setRetain(false)
           .setTimeout(3s)
           .build();
    connect(mGatewayController.get(), &GatewayController::hasReplyMessage, mPub.get(), &AsyncPublish::publish);
    mSub->connect();
    mPub->connect();
}

ConfigManager::ConfigManager() {

}
