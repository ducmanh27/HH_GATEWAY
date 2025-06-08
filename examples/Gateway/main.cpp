#include "logger/logger.h"
#include <QCoreApplication>
#include <QTimer>
#include "configmanager.h"
int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    mpd::Logger::getInstance()->initLogger();
    ConfigManager::getInstance().initSystem();
    return a.exec();
}
