#include "callbackimpl.h"


CallbackImpl::CallbackImpl(mqtt::async_client &cli, mqtt::connect_options &connOpts, const std::string &topic, const std::string &client_id, int qos, int n_retry_attempts, TopicHandlerMap topicHandlers)
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

void CallbackImpl::reconnect() {
    std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    try {
        cli_.connect(connOpts_, nullptr, *this);
    }
    catch (const mqtt::exception &exc) {
        LOG_ERROR("Error: {}", exc.what());
        exit(1);
    }
}

void CallbackImpl::on_failure(const mqtt::token &tok) {
    LOG_ERROR("Connection attempt failed");
    if (++nretry_ > N_RETRY_ATTEMPTS)
        exit(1);
    reconnect();
}

void CallbackImpl::on_success(const mqtt::token &tok) {

}

void CallbackImpl::connected(const std::string &cause) {
    LOG_INFO("Connection success");
    LOG_INFO("Subscribing to topic '{}' for client {} using QoS {}", topic_, client_id_, qos_);
    cli_.subscribe(topic_, qos_, nullptr, subListener_);
}

void CallbackImpl::connection_lost(const std::string &cause) {
    LOG_INFO("Connection lost");
    if (!cause.empty())
        LOG_DEBUG("Cause: {}", cause);
    LOG_INFO("Reconnecting...");
    nretry_ = 0;
    reconnect();
}

void CallbackImpl::message_arrived(mqtt::const_message_ptr msg) {
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

void CallbackImpl::delivery_complete(mqtt::delivery_token_ptr token) {
    LOG_INFO("Delivery complete for token: {}", (token ? token->get_message_id() : -1));
}
