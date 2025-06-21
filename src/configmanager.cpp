#include "configmanager.h"
#include <odb/pgsql/database.hxx>
#include <chrono>

#include "dao/nodedaoimpl.h"
#include "controller/gatewaycontroller.h"
#include "service/registerservice.h"
#include "mqtt/asyncsubscribe.h"
#include "mqtt/asyncpublish.h"
#include "service/registerservice.h"
#include "service/keepaliveservice.h"
#include "mqtt/asyncpublish.h"

using namespace std::chrono_literals;
ConfigManager &ConfigManager::getInstance() {
    static ConfigManager instance; // Initial once, thread-safe from C++11
    return instance;
}

void ConfigManager::initSystem() {
    try {
        mDb = create_database("admin", "secret", "gatewaydb", "localhost", 5432);
    }
    catch (const odb::exception &e) {
        LOG_ERROR("[ODB Exception] {}", e.what());
        exit(0);
    }
    // DAO
    mNodeDAO = std::make_shared<NodeDAOImpl>(std::move(mDb));
    // Service
    mRegisterService = std::make_shared<RegisterService>(mNodeDAO);
    mKeepAliveService = std::make_shared<KeepAliveService>(mNodeDAO);
    // Controller
    mGatewayController = std::make_shared<GatewayController>(mRegisterService,
                                                             mKeepAliveService);

    // Transport
    mSub = AsyncSubscribe::AsyncSubscribeBuilder()
           .setHost("localhost")
           .setPort(1883)
           .setClientId("XXX123")
           .setTopic("node/#")
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
    connect(mGatewayController.get(), &GatewayController::hasRequestMessage, mPub.get(), &AsyncPublish::publish);
    mSub->connect();
    mPub->connect();
}

ConfigManager::ConfigManager() {

}
