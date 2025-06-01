#include "logger/logger.h"
#include <QCoreApplication>
#include <QTimer>
#include "mqtt/asyncsubscribe.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    mpd::Logger::getInstance()->initLogger();
    std::map<std::string, std::function<void(const std::string &)>> topicHandlers;
    topicHandlers["node/sensor_data"] = [](const std::string & payload) {
        LOG_INFO("Payload xxx: {}", payload);
    };
    AsyncSubscribe subscriber = AsyncSubscribe::AsyncSubscribeBuilder()
                                .setHost("broker.hivemq.com")
                                .setPort(1883)
                                .setClientId("XXX123")
                                .setTopic("#")
                                .setQos(0)
                                .setRetryAttempts(5)
                                .setTopicHandlerMap(topicHandlers)
                                .build();

    subscriber.connect();

    return a.exec();
}
