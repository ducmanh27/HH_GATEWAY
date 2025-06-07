#include "logger/logger.h"
#include <QCoreApplication>
#include <QTimer>
#include "mqtt/asyncsubscribe.h"
#include "mqtt/asyncpublish.h"
#include <chrono>
using namespace std::chrono_literals;
int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    mpd::Logger::getInstance()->initLogger();
    std::map<std::string, std::function<void(const std::string &)>> topicHandlers;
    topicHandlers["node/sensor_data"] = [](const std::string & payload) {
        LOG_INFO("Payload xxx: {}", payload);
    };
    AsyncSubscribe subscriber = AsyncSubscribe::AsyncSubscribeBuilder()
                                .setHost("localhost")
                                .setPort(1883)
                                .setClientId("XXX123")
                                .setTopic("#")
                                .setQos(0)
                                .setRetryAttempts(5)
                                .setTopicHandlerMap(topicHandlers)
                                .build();

    subscriber.connect();


    // try {
    //     auto j = nlohmann::json::parse(msg->to_string());

    //     // Lambda để in ra tất cả key-value trong json object (phẳng)
    //     auto print_json_key_values = [](const nlohmann::json & obj) {
    //         if (!obj.is_object()) {
    //             LOG_ERROR("JSON payload is not an object");
    //             return;
    //         }
    //         for (auto it = obj.begin(); it != obj.end(); ++it) {
    //             LOG_INFO("{}: {}", it.key(), it.value().dump());
    //         }
    //     };
    //     print_json_key_values(j);

    // }
    // catch (const nlohmann::json::parse_error &e) {
    //     LOG_ERROR("JSON parse error: {}", e.what());
    // }
    // catch (const std::exception &e) {
    //     LOG_ERROR("Exception: {}", e.what());
    // }

    AsyncPublish publisher = AsyncPublish::AsyncPublishBuilder()
                             .setHost("localhost")
                             .setPort(1883)
                             .setClientId("test-pub")
                             .setQos(1)
                             .setRetain(false)
                             .setTimeout(3s)
                             .build();
    publisher.connect();
    try {
        // publisher.connect();
        std::string topic = "test/topic";
        std::string payload = "Hello from AsyncPublish!";
        publisher.publish(topic, payload);
        LOG_DEBUG("Message published successfully.");
    }
    catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return a.exec();
}
