#ifndef ASYNCPUBLISH_H
#define ASYNCPUBLISH_H
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <stdint.h>
#include <string.h>
#include <memory>
#include "mqtt/async_client.h"

class ActionListener;
class CallbackImpl;
class AsyncPublish {
    public:
        class AsyncPublishBuilder {
            public:
                AsyncPublishBuilder();

                AsyncPublishBuilder &setHost(const std::string &host);
                AsyncPublishBuilder &setPort(int port);
                AsyncPublishBuilder &setClientId(const std::string &client_id);
                AsyncPublishBuilder &setQos(int qos);
                AsyncPublishBuilder &setRetain(bool retain);
                AsyncPublishBuilder &setTimeout(std::chrono::seconds timeout);
                AsyncPublish build();

            private:
                std::string host_ {"localhost"};
                int port_ {1883};
                std::string client_id_ {"mpd-publisher"};
                int qos_ {0};
                bool retain_ {false};
                std::chrono::seconds timeout_{std::chrono::seconds(5)};
        };

        ~AsyncPublish();
        void connect();
        void publish(const std::string &topic, const std::string &payload);
    private:
        AsyncPublish(const std::string &server_uri,
                     const std::string &client_id,
                     int qos,
                     bool retain,
                     std::chrono::seconds timeout);

        std::unique_ptr<mqtt::async_client> cli_{nullptr};
        mqtt::connect_options connOpts_;
        int qos_ {0};
        bool retain_ {false};
        std::string server_uri_;
        std::string client_id_;
        std::unique_ptr<ActionListener> listener_ {std::make_unique<ActionListener>("pub")};
};

#endif // ASYNCPUBLISH_H
