#include "actionlistener.h"


void ActionListener::on_failure(const mqtt::token &tok) {
    LOG_ERROR("{} failure", name_);
    if (tok.get_message_id() != 0) {
        LOG_DEBUG("Token ID: [{}]", tok.get_message_id());
    }
}

void ActionListener::on_success(const mqtt::token &tok) {
    LOG_INFO("{} success", name_);
    if (tok.get_message_id() != 0) {
        LOG_DEBUG("Token ID: [{}]", tok.get_message_id());
    }
    auto top = tok.get_topics();
    if (top && !top->empty()) {
        LOG_DEBUG("Token topic: '{}', ...", (*top)[0]);
    }
}

ActionListener::ActionListener(const std::string &name) : name_(name) {}
