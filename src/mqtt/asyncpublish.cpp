#include "asyncpublish.h"
#include <chrono>
#include "logger/logger.h"
#include "callbackimpl.h"
#include "actionlistener.h"

const std::string LWT_PAYLOAD {"Last will and testament."};
const std::string TOPIC_LWT{"xxx"};
using namespace std::chrono_literals;

// ========== Builder ==========

AsyncPublish::AsyncPublishBuilder::AsyncPublishBuilder()
    : port_(1883), qos_(1), retain_(false), timeout_(5s) {}


auto AsyncPublish::AsyncPublishBuilder::setHost(const std::string &host) -> AsyncPublishBuilder & {
    host_ = host;
    return *this;
}

auto AsyncPublish::AsyncPublishBuilder::setPort(int port) -> AsyncPublishBuilder & {
    port_ = port;
    return *this;
}

auto AsyncPublish::AsyncPublishBuilder::setClientId(const std::string &client_id) -> AsyncPublishBuilder & {
    client_id_ = client_id;
    return *this;
}

auto AsyncPublish::AsyncPublishBuilder::setQos(int qos) -> AsyncPublishBuilder & {
    qos_ = qos;
    return *this;
}

auto AsyncPublish::AsyncPublishBuilder::setRetain(bool retain) -> AsyncPublishBuilder & {
    retain_ = retain;
    return *this;
}

auto AsyncPublish::AsyncPublishBuilder::setTimeout(std::chrono::seconds timeout) -> AsyncPublishBuilder & {
    timeout_ = timeout;
    return *this;
}

AsyncPublish AsyncPublish::AsyncPublishBuilder::build() {
    if (host_.empty()) {
        throw std::runtime_error("Host must be set before calling build()");
    }
    std::string server_uri = "tcp://" + host_ + ":" + std::to_string(port_);
    return AsyncPublish(server_uri, client_id_, qos_, retain_, timeout_);
}

// ========== AsyncPublish ==========

AsyncPublish::AsyncPublish(const std::string &server_uri,
                           const std::string &client_id,
                           int qos,
                           bool retain,
                           std::chrono::seconds timeout)
    : server_uri_(server_uri),
      client_id_(client_id),
      qos_(qos),
      retain_(retain) {
    cli_ = std::make_unique<mqtt::async_client>(server_uri_, client_id_);
    connOpts_ = mqtt::connect_options_builder()
                .connect_timeout(timeout)
                .clean_session()
                .will(mqtt::message(TOPIC_LWT, LWT_PAYLOAD, qos_, false))
                .finalize();
}

AsyncPublish::~AsyncPublish() {
    try {
        if (cli_ && cli_->is_connected()) {
            cli_->disconnect()->wait();
        }
    }
    catch (const mqtt::exception &e) {
        LOG_ERROR("Error during disconnect: {}", e.what());
    }
}

void AsyncPublish::connect() {
    try {
        LOG_DEBUG("Connecting to MQTT broker at {}", server_uri_);
        cli_->connect(connOpts_)->wait();
        LOG_INFO("Connected to MQTT broker at {}", server_uri_);
    }
    catch (const mqtt::exception &e) {
        LOG_ERROR("Connection error: {}", e.what());
        throw;
    }
}

void AsyncPublish::publish(const std::string &topic, const std::string &payload) {
    if (!cli_->is_connected()) {
        LOG_DEBUG("Client not connected. Attempting to connect...");
        connect();
    }

    mqtt::message_ptr msg = mqtt::make_message(topic, payload);
    msg->set_qos(qos_);
    msg->set_retained(retain_);

    try {
        cli_->publish(msg, nullptr, *listener_);
        LOG_DEBUG("Publishing message to topic '{}': {}", topic, payload);
    }
    catch (const mqtt::exception &e) {
        LOG_ERROR("Publish error: {}", e.what());
    }
}
