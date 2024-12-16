#include "config.hpp"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp> // JSON library
#include "../common/logging.hpp"

using json = nlohmann::json;

namespace c2me2 {
    namespace common {

        Config& Config::instance() {
            static Config instance;
            return instance;
        }

        void Config::setDefaults() {
            m_port = 4444;
            m_loggingLevel = "Info";
            m_sessionTimeout = 600;
        }

        void Config::loadConfig(const std::string& configFile) {
            std::lock_guard<std::mutex> lock(m_mutex);
            setDefaults(); // Start with defaults

            std::ifstream inFile(configFile);
            if (!inFile.is_open()) {
                logWarning("Failed to open config file: " + configFile + ". Using defaults.");
                return;
            }

            try {
                json j;
                inFile >> j;

                if (j.contains("port")) {
                    m_port = j.at("port").get<int>();
                }
                if (j.contains("logging_level")) {
                    m_loggingLevel = j.at("logging_level").get<std::string>();
                }
                if (j.contains("session_timeout")) {
                    m_sessionTimeout = j.at("session_timeout").get<int>();
                }

                logInfo("Configuration loaded successfully from " + configFile);
            } catch (const std::exception& ex) {
                logError("Error parsing config file: " + std::string(ex.what()) + ". Using defaults.");
            }
        }

        int Config::getPort() const {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_port;
        }

        std::string Config::getLoggingLevel() const {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_loggingLevel;
        }

        int Config::getSessionTimeout() const {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_sessionTimeout;
        }

    } // namespace common
} // namespace c2me2
