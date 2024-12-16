#ifndef C2ME2_COMMON_LOGGING_HPP
#define C2ME2_COMMON_LOGGING_HPP

#include <string>
#include <mutex>
#include <fstream>

namespace c2me2 {
    namespace common {

        enum class LogLevel {
            Debug,
            Info,
            Warning,
            Error
        };

        class Logger {
        public:
            static Logger& instance();

            void setLogLevel(LogLevel level);
            void enableFileLogging(const std::string& filePath, bool rotateLogs = false, size_t maxSize = 1048576);
            void log(LogLevel level, const std::string& message);

        private:
            Logger() = default;
            Logger(const Logger&) = delete;
            Logger(Logger&&) = delete;
            Logger& operator=(const Logger&) = delete;
            Logger& operator=(Logger&&) = delete;

            std::string getTimestamp();
            void rotateLogFile();

            std::mutex m_mutex;
            LogLevel m_currentLevel = LogLevel::Info;
            std::ofstream m_logFile;
            bool m_rotateLogs = false;
            size_t m_maxFileSize = 0;
            std::string m_logFilePath;
        };

        inline void logDebug(const std::string& msg) {
            Logger::instance().log(LogLevel::Debug, msg);
        }

        inline void logInfo(const std::string& msg) {
            Logger::instance().log(LogLevel::Info, msg);
        }

        inline void logWarning(const std::string& msg) {
            Logger::instance().log(LogLevel::Warning, msg);
        }

        inline void logError(const std::string& msg) {
            Logger::instance().log(LogLevel::Error, msg);
        }

    } // namespace common
} // namespace c2me2

#endif // C2ME2_COMMON_LOGGING_HPP
