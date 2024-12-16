#ifndef C2ME2_SERVER_SERVER_CONFIG_HPP
#define C2ME2_SERVER_SERVER_CONFIG_HPP

#include <string>

namespace c2me2 {
    namespace server {

        class ServerConfig {
        public:
            static ServerConfig& instance();

            int getPort() const;
            std::string getLoggingLevel() const;
            int getSessionTimeout() const;

        private:
            ServerConfig() = default;
            ServerConfig(const ServerConfig&) = delete;
            ServerConfig(ServerConfig&&) = delete;
            ServerConfig& operator=(const ServerConfig&) = delete;
            ServerConfig& operator=(ServerConfig&&) = delete;
        };

    } // namespace server
} // namespace c2me2

#endif // C2ME2_SERVER_SERVER_CONFIG_HPP
