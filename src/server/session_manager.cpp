#include "session_manager.hpp"
#include "pty_session.hpp"
#include "../common/logging.hpp"
#include <algorithm>

namespace c2me2 {
    namespace server {

        SessionManager& SessionManager::instance() {
            static SessionManager inst;
            return inst;
        }

        void SessionManager::addSession(PTYSession* session) {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_sessions.push_back(session);
            c2me2::common::logInfo("Session added: " + std::to_string(session->getSessionId()) +
                                   " from IP: " + session->getClientIp());
        }

        void SessionManager::removeSession(int sessionId) {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_sessions.erase(std::remove_if(m_sessions.begin(), m_sessions.end(),
                [sessionId](PTYSession* s) {
                    if (s->getSessionId() == sessionId) {
                        delete s;
                        return true;
                    }
                    return false;
                }), m_sessions.end());
        }

        std::vector<PTYSession*> SessionManager::listSessions() {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_sessions; // returns a copy
        }

        PTYSession* SessionManager::getSession(int sessionId) {
            std::lock_guard<std::mutex> lock(m_mutex);
            for (auto s : m_sessions) {
                if (s->getSessionId() == sessionId) return s;
            }
            return nullptr;
        }

    } // namespace server
} // namespace c2me2
