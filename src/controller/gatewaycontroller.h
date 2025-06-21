#ifndef GATEWAYCONTROLLER_H
#define GATEWAYCONTROLLER_H

#include <memory>
#include <map>
#include <functional>
#include <string>
#include <QObject>
#include <QTimer>
class AsyncPublish;
class AsyncSubscribe;
class RegisterService;
class KeepAliveService;
class GatewayController  : public QObject {
        Q_OBJECT
    public:
        explicit GatewayController(std::shared_ptr<RegisterService> registerService,
                                   std::shared_ptr<KeepAliveService> keepAliveService);

        std::map<std::string, std::function<void(const std::string &)>> topicHandlers() const;

    private:
        void registerHandlers();
        void registerSensorDataHandler();
        void registerRequestHandler();
        void registerReplyHandler();
        std::shared_ptr<KeepAliveService> mKeepAliveService {nullptr};
        std::shared_ptr<RegisterService> mRegisterService {nullptr};
        std::map<std::string, std::function<void(const std::string &)>> mTopicHandlers;
    signals:
        void hasReplyMessage(const std::string &topic, const std::string &msg);
        void hasRequestMessage(const std::string &topic, const std::string &msg);

};

#endif // GATEWAYCONTROLLER_H
