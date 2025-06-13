#ifndef CALLBACKIMPL_H
#define CALLBACKIMPL_H
#include "logger/logger.h"
#include "mqtt/async_client.h"
#include "actionlistener.h"
class CallbackImpl : public virtual mqtt::callback, public virtual mqtt::iaction_listener {
    public:
        using TopicHandlerMap = std::map<std::string, std::function<void(const std::string &)>>;
        CallbackImpl(mqtt::async_client &cli,
                     mqtt::connect_options &connOpts,
                     const std::string &topic,
                     const std::string &client_id,
                     int qos,
                     int n_retry_attempts,
                     TopicHandlerMap topicHandlers);
    private:
        int nretry_;
        mqtt::async_client &cli_;
        mqtt::connect_options &connOpts_;
        ActionListener subListener_;
        const int N_RETRY_ATTEMPTS = 5;
        std::string topic_;
        std::string client_id_;
        int qos_;
        TopicHandlerMap topicHandlerMap_;

        void reconnect();

        void on_failure(const mqtt::token &tok) override;

        void on_success(const mqtt::token &tok) override;

        void connected(const std::string &cause) override;

        void connection_lost(const std::string &cause) override;

        void message_arrived(mqtt::const_message_ptr msg) override;

        void delivery_complete(mqtt::delivery_token_ptr token) override;
};

#endif // CALLBACKIMPL_H
