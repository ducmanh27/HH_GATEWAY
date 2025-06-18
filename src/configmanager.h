#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H
#include <QObject>
#include <memory>
namespace odb {
    namespace pgsql {
        class database;
    }
}
class AsyncPublish;
class AsyncSubscribe;
class GatewayController;
class RegisterService;
class NodeDAO;
class ConfigManager : public QObject {
        Q_OBJECT
    public:

        static ConfigManager &getInstance();
        ConfigManager(const ConfigManager &) = delete;
        ConfigManager &operator=(const ConfigManager &) = delete;
        ConfigManager(ConfigManager &&) = delete;
        ConfigManager &operator=(ConfigManager &&) = delete;

        void initSystem();

    private:
        ConfigManager();
        std::shared_ptr<odb::pgsql::database> mDb {nullptr};
        std::shared_ptr<NodeDAO> mNodeDAO {nullptr};
        std::shared_ptr<AsyncPublish> mPub {nullptr};
        std::shared_ptr<AsyncSubscribe> mSub{nullptr};
        std::shared_ptr<RegisterService> mRegisterService {nullptr};
        std::shared_ptr<GatewayController> mGatewayController {nullptr};
};

#endif // CONFIGMANAGER_H
