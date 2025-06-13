#ifndef ACTIONLISTENER_H
#define ACTIONLISTENER_H
#include "logger/logger.h"
#include "mqtt/async_client.h"
class ActionListener : public virtual mqtt::iaction_listener {
        std::string name_;

        void on_failure(const mqtt::token &tok) override;

        void on_success(const mqtt::token &tok) override;

    public:
        explicit ActionListener(const std::string &name);
};

#endif // ACTIONLISTENER_H
