#include "logging.hpp"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <filesystem>

namespace c2me2 {
    namespace common {

        Logger& Logger::instance() {
            static Logger instance;
            return instance;
        }

        void Logger::setLogLevel(LogLevel level) {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_currentLevel = level;
        }

        void Logger::enableFileLogging(const std::string& filePath, bool rotateLogs, size_t maxSize) {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_logFile.is_open()) {
                m_logFile.close();
            }
            m_logFilePath = filePath;
            m_rotateLogs = rotateLogs;
            m_maxFileSize = maxSize;
            m_logFile.open(filePath, std::ios::app);
            if (!m_logFile.is_open()) {
                std::cerr << "[ERROR] Failed to open log file: " << filePath << "\n";
            }
        }

        void Logger::log(LogLevel level, const std::string& message) {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (level < m_currentLevel) {
                return;
            }

            std::string timestamp = getTimestamp();
            std::string levelStr;
            switch (level) {
                case LogLevel::Debug:   levelStr = "[DEBUG]"; break;
                case LogLevel::Info:    levelStr = "[INFO] "; break;
                case LogLevel::Warning: levelStr = "[WARN] "; break;
                case LogLevel::Error:   levelStr = "[ERROR]"; break;
            }

            std::string logMessage = timestamp + " " + levelStr + " " + message + "\n";

            // Print to console
            std::cout << logMessage;

            // Print to file if enabled
            if (m_logFile.is_open()) {
                m_logFile << logMessage;
                m_logFile.flush();

                // Rotate log file if necessary
                if (m_rotateLogs && std::filesystem::file_size(m_logFilePath) > m_maxFileSize) {
                    rotateLogFile();
                }
            }
        }

        std::string Logger::getTimestamp() {
            auto now = std::chrono::system_clock::now();
            auto time = std::chrono::system_clock::to_time_t(now);
            std::tm localTime{};
            localtime_r(&time, &localTime);

            std::ostringstream oss;
            oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
            return oss.str();
        }

        void Logger::rotateLogFile() {
            m_logFile.close();
            std::string backupPath = m_logFilePath + ".old";
            std::filesystem::rename(m_logFilePath, backupPath);
            m_logFile.open(m_logFilePath, std::ios::trunc);
            if (!m_logFile.is_open()) {
                std::cerr << "[ERROR] Failed to rotate log file: " << m_logFilePath << "\n";
            }
        }

    } // namespace common
} // namespace c2me2
