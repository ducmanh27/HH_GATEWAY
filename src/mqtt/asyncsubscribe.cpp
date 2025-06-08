#include "asyncsubscribe.h"
#include "logger/logger.h"

AsyncSubscribe::AsyncSubscribeBuilder::AsyncSubscribeBuilder()
    : host_(""), port_(0),
      client_id_("paho_cpp_async_subscribe"),
      topic_("#"),
      qos_(0),
      n_retry_attempts_(5) {}

AsyncSubscribe::AsyncSubscribeBuilder &AsyncSubscribe::AsyncSubscribeBuilder::setTopicHandlerMap(std::map<std::string, std::function<void (const std::string &)> > handlerMap) {
    topicHandlerMap_ = std::move(handlerMap);
    return *this;
}

AsyncSubscribe::AsyncSubscribeBuilder &AsyncSubscribe::AsyncSubscribeBuilder::setHost(const std::string &host) {
    host_ = host;
    return *this;
}

AsyncSubscribe::AsyncSubscribeBuilder &AsyncSubscribe::AsyncSubscribeBuilder::setPort(int port) {
    port_ = port;
    return *this;
}

AsyncSubscribe::AsyncSubscribeBuilder &AsyncSubscribe::AsyncSubscribeBuilder::setClientId(const std::string &client_id) {
    client_id_ = client_id;
    return *this;
}

AsyncSubscribe::AsyncSubscribeBuilder &AsyncSubscribe::AsyncSubscribeBuilder::setTopic(const std::string &topic) {
    topic_ = topic;
    return *this;
}

AsyncSubscribe::AsyncSubscribeBuilder &AsyncSubscribe::AsyncSubscribeBuilder::setQos(int qos) {
    qos_ = qos;
    return *this;
}

AsyncSubscribe::AsyncSubscribeBuilder &AsyncSubscribe::AsyncSubscribeBuilder::setRetryAttempts(int attempts) {
    n_retry_attempts_ = attempts;
    return *this;
}

std::shared_ptr<AsyncSubscribe> AsyncSubscribe::AsyncSubscribeBuilder::build() {
    if (host_.empty()) {
        throw std::runtime_error("Host must be set via setHost() before build().");
    }
    if (port_ == 0) {
        throw std::runtime_error("Port must be set via setPort() before build().");
    }

    std::string server_uri = "tcp://" + host_ + ":" + std::to_string(port_);

    return std::shared_ptr<AsyncSubscribe>(
               new AsyncSubscribe(
                   server_uri,
                   topicHandlerMap_,
                   client_id_,
                   topic_,
                   qos_,
                   n_retry_attempts_
               )
           );
}


AsyncSubscribe::AsyncSubscribe(const std::string &server_uri,
                               std::map<std::string, std::function<void (const std::string &)> > topicHandlerMap,
                               const std::string &client_id,
                               const std::string &topic,
                               int qos,
                               int n_retry_attempts
                              )
    : server_uri_(server_uri),
      client_id_(client_id),
      topic_(topic),
      qos_(qos),
      n_retry_attempts_(n_retry_attempts),
      topicHandlerMap_(std::move(topicHandlerMap)) {
    cli = std::make_unique<mqtt::async_client>(server_uri_, client_id_);
    connOpts.set_clean_session(false);
    cb = std::make_unique<CallbackImpl>(*cli, connOpts, topic_, client_id_, qos_, n_retry_attempts_, topicHandlerMap_);
    cli->set_callback(*cb);
}

void AsyncSubscribe::setTopicHandlerMap(const std::map<std::string, std::function<void (const std::string &)> > &newTopicHandlerMap)
{
    topicHandlerMap_ = newTopicHandlerMap;
}


void AsyncSubscribe::connect() {
    try {
        LOG_DEBUG("Connecting to the MQTT server {}", server_uri_);
        cli->connect(connOpts, nullptr, *cb);
    }
    catch (const mqtt::exception &e) {
        LOG_ERROR("MQTT error: {}", e.what());
    }
}
