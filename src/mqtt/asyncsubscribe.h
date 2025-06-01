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
#include "logger/logger.h"
#include "mqtt/async_client.h"
#include "json.hpp"
/////////////////////////////////////////////////////////////////////////////

// Callbacks for the success or failures of requested actions.
// This could be used to initiate further action, but here we just log the
// results to the console.

class action_listener : public virtual mqtt::iaction_listener {
        std::string name_;

        void on_failure(const mqtt::token &tok) override {
            LOG_ERROR("{} failure", name_);
            if (tok.get_message_id() != 0) {
                LOG_DEBUG("Token ID: [{}]", tok.get_message_id());
            }
        }

        void on_success(const mqtt::token &tok) override {
            LOG_INFO("{} success", name_);
            if (tok.get_message_id() != 0) {
                LOG_DEBUG("Token ID: [{}]", tok.get_message_id());
            }
            auto top = tok.get_topics();
            if (top && !top->empty()) {
                LOG_DEBUG("Token topic: '{}', ...", (*top)[0]);
            }
        }

    public:
        explicit action_listener(const std::string &name) : name_(name) {}
};

class callback : public virtual mqtt::callback, public virtual mqtt::iaction_listener {
    public:
        using TopicHandlerMap = std::map<std::string, std::function<void(const std::string &)>>;
        callback(mqtt::async_client &cli,
                 mqtt::connect_options &connOpts,
                 const std::string &topic,
                 const std::string &client_id,
                 int qos,
                 int n_retry_attempts,
                 TopicHandlerMap topicHandlers)
            : nretry_(0),
              cli_(cli),
              connOpts_(connOpts),
              subListener_("Subscription"),
              topic_(topic),
              client_id_(client_id),
              qos_(qos),
              N_RETRY_ATTEMPTS(n_retry_attempts),
              topicHandlerMap_(std::move(topicHandlers))
        {}
    private:
        int nretry_;
        mqtt::async_client &cli_;
        mqtt::connect_options &connOpts_;
        action_listener subListener_;
        const int N_RETRY_ATTEMPTS = 5;
        std::string topic_;
        std::string client_id_;
        int qos_;
        TopicHandlerMap topicHandlerMap_;

        void reconnect() {
            std::this_thread::sleep_for(std::chrono::milliseconds(2500));
            try {
                cli_.connect(connOpts_, nullptr, *this);
            }
            catch (const mqtt::exception &exc) {
                LOG_ERROR("Error: {}", exc.what());
                exit(1);
            }
        }

        void on_failure(const mqtt::token &tok) override {
            LOG_ERROR("Connection attempt failed");
            if (++nretry_ > N_RETRY_ATTEMPTS)
                exit(1);
            reconnect();
        }

        void on_success(const mqtt::token &tok) override {}

        void connected(const std::string &cause) override {
            LOG_INFO("Connection success");
            LOG_INFO("Subscribing to topic '{}' for client {} using QoS {}", topic_, client_id_, qos_);
            cli_.subscribe(topic_, qos_, nullptr, subListener_);
        }

        void connection_lost(const std::string &cause) override {
            LOG_INFO("Connection lost");
            if (!cause.empty())
                LOG_DEBUG("Cause: {}", cause);
            LOG_INFO("Reconnecting...");
            nretry_ = 0;
            reconnect();
        }

        void message_arrived(mqtt::const_message_ptr msg) override {
            const std::string &topic = msg->get_topic();
            const std::string &payload = msg->to_string();

            auto it = topicHandlerMap_.find(topic);
            if (it != topicHandlerMap_.end()) {
                it->second(payload);
            }
            else {
                LOG_ERROR("No handler found for topic: {}", topic);
            }
        }

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

        void delivery_complete(mqtt::delivery_token_ptr token) override {}




};


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

                AsyncSubscribe build();

            private:
                std::string host_;
                int port_;
                std::string client_id_ = "mpd-777";
                std::string topic_ = "#";
                int qos_ = 0;
                int n_retry_attempts_ = 5;
                std::map<std::string, std::function<void(const std::string &)>> topicHandlerMap_;

        };

        ~AsyncSubscribe();

        void connect();

    private:
        AsyncSubscribe(const std::string &server_uri,
                       std::map<std::string, std::function<void(const std::string &)>> topicHandlerMap,
                       const std::string &client_id = "mpd-777",
                       const std::string &topic = "#",
                       int qos = 0,
                       int n_retry_attempts = 5
                      );

        mqtt::async_client *cli {nullptr};
        mqtt::connect_options connOpts;
        callback *cb {nullptr};

        std::string server_uri_ {"tcp://broker.hivemq.com:1883"};
        std::string client_id_ {"mpd-777"};
        std::string topic_ {"#"};
        int qos_ {0};
        int n_retry_attempts_ {5};
        std::map<std::string, std::function<void(const std::string &)>> topicHandlerMap_;

};


#endif // ASYNCSUBSCRIBE_H
