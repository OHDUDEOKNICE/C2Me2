#ifndef C2ME2_COMMON_CONFIG_HPP
#define C2ME2_COMMON_CONFIG_HPP

#include <string>
#include <mutex>

namespace c2me2 {
    namespace common {

        class Config {
        public:
            static Config& instance();

            void loadConfig(const std::string& configFile);

            int getPort() const;
            std::string getLoggingLevel() const;
            int getSessionTimeout() const;

        private:
            Config() = default;
            Config(const Config&) = delete;
            Config(Config&&) = delete;
            Config& operator=(const Config&) = delete;
            Config& operator=(Config&&) = delete;

            void setDefaults();

            mutable std::mutex m_mutex;

            int m_port = 4444;
            std::string m_loggingLevel = "Info";
            int m_sessionTimeout = 600;
        };

    } // namespace common
} // namespace c2me2

#endif // C2ME2_COMMON_CONFIG_HPP
