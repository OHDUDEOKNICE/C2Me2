#ifndef C2ME2_SERVER_PTY_SESSION_HPP
#define C2ME2_SERVER_PTY_SESSION_HPP

#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <fstream>

namespace c2me2 {
    namespace server {

        class PTYSession {
        public:
            PTYSession(int client_fd, const std::string& client_ip);
            ~PTYSession();

            void start();
            void sendCommand(const std::string& cmd);
            std::string readOutput();

            std::string executeCommand(const std::string& cmd);
            bool uploadThroughShell(const std::string& localFile, const std::string& remoteFile);
            bool downloadThroughShell(const std::string& remoteFile, const std::string& localPath);

            void logCommand(const std::string& command, const std::string& output);

            int getSessionId() const { return m_sessionId; }
            std::string getClientIp() const { return m_clientIp; }

            int getFd() const { return m_clientFd; }

        private:
            void handleCommunication();
            static int generateSessionId();

            int writeToShell(const std::string& data);
            std::string readFromShell();

            int m_clientFd;
            int m_masterFd;
            pid_t m_childPid;
            std::string m_clientIp;
            int m_sessionId;
            std::thread m_thread;
            std::atomic<bool> m_running;

            std::mutex m_outputMutex;
            std::string m_outputBuffer;

            std::ofstream m_logFile; // Log file for this session
        };

    } // namespace server
} // namespace c2me2

#endif // C2ME2_SERVER_PTY_SESSION_HPP
