#include "pty_session.hpp"
#include "../common/logging.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <pty.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <fstream>
#include <mutex>
#include <atomic>

namespace {
    std::atomic<int> global_session_counter{1};
}

namespace c2me2 {
    namespace server {

        PTYSession::PTYSession(int client_fd, const std::string& client_ip)
            : m_clientFd(client_fd), m_masterFd(-1), m_childPid(-1),
              m_clientIp(client_ip), m_sessionId(generateSessionId()), m_running(false) {
            m_logFile.open("logs/session_" + std::to_string(m_sessionId) + ".log", std::ios::app);
            if (!m_logFile.is_open()) {
                c2me2::common::logError("Failed to open log file for session " + std::to_string(m_sessionId));
            }
        }

        PTYSession::~PTYSession() {
            m_running = false;
            if (m_thread.joinable()) {
                m_thread.join();
            }
            if (m_clientFd >= 0) {
                close(m_clientFd);
            }
            if (m_masterFd >= 0) {
                close(m_masterFd);
            }
            if (m_childPid > 0) {
                kill(m_childPid, SIGKILL);
                waitpid(m_childPid, nullptr, 0);
            }
        }

        int PTYSession::generateSessionId() {
            return global_session_counter.fetch_add(1, std::memory_order_relaxed);
        }

        void PTYSession::logCommand(const std::string& command, const std::string& output) {
            if (m_logFile.is_open()) {
                m_logFile << "[COMMAND] " << command << "\n";
                m_logFile << "[OUTPUT]\n" << output << "\n";
                m_logFile.flush();
            }
        }

        std::string PTYSession::executeCommand(const std::string& cmd) {
            writeToShell(cmd + "\n");
            usleep(100000); // 0.1s delay for demonstration
            return readFromShell();
        }

        bool PTYSession::uploadThroughShell(const std::string& localFile, const std::string& remoteFile) {
            std::ifstream in(localFile, std::ios::binary);
            if (!in.is_open()) {
                c2me2::common::logError("Failed to open local file: " + localFile);
                return false;
            }

            std::string cmd = "cat > " + remoteFile + "\n";
            write(m_clientFd, cmd.c_str(), cmd.size());

            char buf[1024];
            while (in.read(buf, sizeof(buf)) || in.gcount() > 0) {
                write(m_clientFd, buf, in.gcount());
            }

            write(m_clientFd, "\x04", 1); // EOF
            in.close();
            return true;
        }

        bool PTYSession::downloadThroughShell(const std::string& remoteFile, const std::string& localPath) {
            std::string cmd = "cat " + remoteFile + "\n";
            write(m_clientFd, cmd.c_str(), cmd.size());

            std::ofstream out(localPath, std::ios::binary);
            if (!out.is_open()) {
                c2me2::common::logError("Failed to open local file: " + localPath);
                return false;
            }

            char buf[1024];
            while (true) {
                ssize_t n = read(m_clientFd, buf, sizeof(buf));
                if (n <= 0) break;
                out.write(buf, n);
            }
            out.close();
            return true;
        }

        int PTYSession::writeToShell(const std::string& data) {
            return write(m_clientFd, data.c_str(), data.size());
        }

        std::string PTYSession::readFromShell() {
            char buf[1024];
            ssize_t n = read(m_clientFd, buf, sizeof(buf)-1);
            if (n > 0) {
                buf[n] = '\0';
                return std::string(buf);
            }
            return "";
        }

    } // namespace server
} // namespace c2me2
