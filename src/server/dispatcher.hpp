#ifndef C2ME2_SERVER_DISPATCHER_HPP
#define C2ME2_SERVER_DISPATCHER_HPP

#include <string>

namespace c2me2 {
    namespace server {

        class Dispatcher {
        public:
            static void handleCommand(const std::string& cmd, bool& interactiveMode, int& interactiveSession);
        };

    } // namespace server
} // namespace c2me2

#endif // C2ME2_SERVER_DISPATCHER_HPP
