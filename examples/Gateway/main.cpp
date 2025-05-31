#include "logger/logger.h"
#include <QCoreApplication>
#include <QTimer>
int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);
  mpd::Logger::getInstance()->initLogger();
  auto logger = mpd::Logger::getInstance()->getLogger();
  QTimer::singleShot(std::chrono::seconds(5), []() {
    LOG_INFO("Info log");
    LOG_ERROR("Error log !");
    LOG_TRACE("Hello world {}", 3);
    LOG_DEBUG("exit app");
    QCoreApplication::exit();
  });
  return a.exec();
}
