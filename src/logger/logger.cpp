#include "logger.h"
#include <QCoreApplication>
#include <iostream>
#include <sys/stat.h>
namespace mpd {
Logger *Logger::mInstance{nullptr};
std::mutex Logger::mMutex;
Logger *Logger::getInstance() {
  std::lock_guard<std::mutex> lock(mMutex);
  if (mInstance == nullptr) {
    mInstance = new Logger();
  }
  return mInstance;
}

void Logger::initLogger() {
  initLogger(QCoreApplication::applicationName().toStdString(),
             "../log/applog_", false);
}

void Logger::initLogger(std::string appName) {
  initLogger(appName, "../log/applog_", false);
}

void Logger::initLogger(std::string appName, std::string prefixLogDir,
                        bool isAsync) {
  time_t rawtime;
  struct tm *timeinfo;
  char buffer[80];
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", timeinfo);
  std::string dateTime(buffer);
  mCurrentLogDir = prefixLogDir + dateTime;
  struct stat st;

  //    S_ISDIR — directory
  if ((stat("../log", &st) == 0) && S_ISDIR(st.st_mode)) {
    printf(" /log folder is present");
  } else {
    printf(" /log folder is not present");
#ifdef __linux__
    int result = mkdir("../log", 0777);
#elif _WIN32
    int result = mkdir("../log");
#endif
    printf("mkdir = {%s}", (result == 0 ? "Success!" : "Fail"));
  }

  if ((stat(mCurrentLogDir.c_str(), &st) == 0) && S_ISDIR(st.st_mode)) {
    printf(" /log_dir is present");
  } else {
    printf(" /log_dir is not present");
#ifdef __linux__
    int result = mkdir(mCurrentLogDir.c_str(), 0777);
#elif _WIN32
    int result = mkdir(mCurrentLogDir.c_str());
#endif
    printf("mkdir = {%s}", (result == 0 ? "Success!" : "Fail"));
  }

  std::string logApplicationFileName = "/" + appName;
  logApplicationFileName = mCurrentLogDir + logApplicationFileName;
  // spdlog declare:
  std::vector<spdlog::sink_ptr> sinks;
  spdlog::sink_ptr fileSink;
  spdlog::sink_ptr stdoutSink;
  // create sinks

  stdoutSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
      logApplicationFileName, MAX_ROTATE_FILE_SIZE, MAX_ROTATE_FILES);
  sinks.push_back(stdoutSink);
  sinks.push_back(fileSink);

  if (isAsync) {
    // queue with 8192 items and 1 backing thread
    spdlog::init_thread_pool(spdlog::details::default_async_q_size, 1U);
    mAppLogger = std::make_shared<spdlog::async_logger>(
        APP_LOGGER_NAME, begin(sinks), end(sinks), spdlog::thread_pool());
  } else {
    mAppLogger = std::make_shared<spdlog::logger>(APP_LOGGER_NAME, begin(sinks),
                                                  end(sinks));
  }

  mAppLogger->set_level(spdlog::level::trace);
  mAppLogger->flush_on(spdlog::level::debug);
  mAppLogger->set_level(spdlog::level::trace);
  mAppLogger->flush_on(spdlog::level::debug);
  //[%Y-%m-%d %H:%M:%S.%e]: Day / Month / Year: Hour / Minute / Second :
  // Miliseconds [000-999] can use %f: Microseconds [000000-999999] can use %F:
  // Nanoseconds [000000000-999999999]
  //[%L]: Short log level D (debug), I(info), etc ...
  //[t_%t]: Thread ID
  //[%n]: Logger name
  //[%g]: Full or relative path of source file
  //[%#]: Line in source file
  //[%v]: Actual text log
  mAppLogger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%L] [t_%t] [%n] [%g:%#] %v");
  mAppLogger->info("Init application logger successful");
  mAppLogger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%L] [t_%t] [%n] [%g:%#] %v");
  mAppLogger->info("Init platform logger successful");
  spdlog::register_logger(mAppLogger);
}

std::shared_ptr<spdlog::logger> Logger::getLogger() const { return mAppLogger; }

Logger::Logger() {}
} // namespace mpd
