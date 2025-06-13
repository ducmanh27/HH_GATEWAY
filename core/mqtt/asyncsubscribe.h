#ifndef ASYNCSUBSCRIBE_H
#define ASYNCSUBSCRIBE_H
#include <cctype>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <functional>
#include <memory>

#include "logger/logger.h"
#include "mqtt/async_client.h"
#include "json.hpp"
#include "actionlistener.h"
#include "callbackimpl.h"


class AsyncSubscribe {
    public:
        // Nested Builder class
        class AsyncSubscribeBuilder {
            public:
                AsyncSubscribeBuilder();

                AsyncSubscribeBuilder &setHost(const std::string &host);
                AsyncSubscribeBuilder &setPort(int port);
                AsyncSubscribeBuilder &setTopicHandlerMap(std::map<std::string, std::function<void(const std::string &)>> handlerMap);
                AsyncSubscribeBuilder &setClientId(const std::string &client_id);
                AsyncSubscribeBuilder &setTopic(const std::string &topic);
                AsyncSubscribeBuilder &setQos(int qos);
                AsyncSubscribeBuilder &setRetryAttempts(int attempts);

                std::shared_ptr<AsyncSubscribe> build();

            private:
                std::string host_;
                int port_;
                std::string client_id_ = "mpd-777";
                std::string topic_ = "#";
                int qos_ = 0;
                int n_retry_attempts_ = 5;
                std::map<std::string, std::function<void(const std::string &)>> topicHandlerMap_;

        };

        ~AsyncSubscribe() = default;

        void connect();

        void setTopicHandlerMap(const std::map<std::string, std::function<void (const std::string &)> > &newTopicHandlerMap);

    private:
        AsyncSubscribe(const std::string &server_uri,
                       std::map<std::string, std::function<void(const std::string &)>> topicHandlerMap,
                       const std::string &client_id = "mpd-777",
                       const std::string &topic = "#",
                       int qos = 0,
                       int n_retry_attempts = 5
                      );

        std::unique_ptr<mqtt::async_client> cli {nullptr};
        mqtt::connect_options connOpts;
        std::unique_ptr<CallbackImpl> cb {nullptr};

        std::string server_uri_ {"tcp://broker.hivemq.com:1883"};
        std::string client_id_ {"mpd-777"};
        std::string topic_ {"#"};
        int qos_ {0};
        int n_retry_attempts_ {5};
        std::map<std::string, std::function<void(const std::string &)>> topicHandlerMap_;

};


#endif // ASYNCSUBSCRIBE_H
