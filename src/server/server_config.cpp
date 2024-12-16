#include "server_config.hpp"
#include "../common/config.hpp"

namespace c2me2 {
    namespace server {

        ServerConfig& ServerConfig::instance() {
            static ServerConfig instance;
            return instance;
        }

        int ServerConfig::getPort() const {
            return c2me2::common::Config::instance().getPort();
        }

        std::string ServerConfig::getLoggingLevel() const {
            return c2me2::common::Config::instance().getLoggingLevel();
        }

        int ServerConfig::getSessionTimeout() const {
            return c2me2::common::Config::instance().getSessionTimeout();
        }

    } // namespace server
} // namespace c2me2
