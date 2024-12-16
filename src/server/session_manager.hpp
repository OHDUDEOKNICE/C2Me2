#ifndef C2ME2_SERVER_SESSION_MANAGER_HPP
#define C2ME2_SERVER_SESSION_MANAGER_HPP

#include <vector>
#include <mutex>

namespace c2me2 {
    namespace server {

        class PTYSession; // Forward declaration

        class SessionManager {
        public:
            static SessionManager& instance(); // Singleton instance

            void addSession(PTYSession* session);
            void removeSession(int sessionId);
            std::vector<PTYSession*> listSessions();
            PTYSession* getSession(int sessionId);

        private:
            SessionManager() = default; // Private constructor
            SessionManager(const SessionManager&) = delete;
            SessionManager(SessionManager&&) = delete;
            SessionManager& operator=(const SessionManager&) = delete;
            SessionManager& operator=(SessionManager&&) = delete;

            std::mutex m_mutex; // Mutex for thread safety
            std::vector<PTYSession*> m_sessions; // List of active sessions
        };

    } // namespace server
} // namespace c2me2

#endif // C2ME2_SERVER_SESSION_MANAGER_HPP
