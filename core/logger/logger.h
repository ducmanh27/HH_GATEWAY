#ifndef LOGGER_H
#define LOGGER_H
#include <mutex>
#include <memory>
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/spdlog.h"
#include "spdlog/async.h"

#define APP_LOGGER_NAME "mpd_logger"
#define MAX_ROTATE_FILE_SIZE 1024*10000         //10 MB
#define MAX_ROTATE_FILES 100


#define LOG_DEBUG(...) SPDLOG_LOGGER_CALL(spdlog::get(APP_LOGGER_NAME), spdlog::level::debug, __VA_ARGS__)
#define LOG_INFO(...) SPDLOG_LOGGER_CALL(spdlog::get(APP_LOGGER_NAME), spdlog::level::info, __VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_LOGGER_CALL(spdlog::get(APP_LOGGER_NAME), spdlog::level::err, __VA_ARGS__)
#define LOG_TRACE(...) SPDLOG_LOGGER_CALL(spdlog::get(APP_LOGGER_NAME), spdlog::level::trace, __VA_ARGS__)



namespace  spdlog {
    class logger;
}
namespace mpd {
    class Logger {
        public:
            enum LevelEnum {
                TRACE = 0,
                DEBUG,
                INFO,
                WARN,
                ERROR,
                CRITICAL,
                OFF,
                N_LEVELS
            };
            static Logger *getInstance();
            void initLogger();
            void initLogger(const std::string &appName);
            void initLogger(const std::string &appName,
                            const std::string &prefixLogDir,
                            bool isAsync = false);
            std::shared_ptr<spdlog::logger> getLogger() const;

        private:
            static Logger *mInstance;
            static std::mutex mMutex;
            std::shared_ptr<spdlog::logger> mAppLogger {nullptr};
            std::string mCurrentLogDir {"."};
            Logger();
    };
}


#endif // LOGGER_H
