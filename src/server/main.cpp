#include "../common/config.hpp"
#include "../common/logging.hpp"
#include "session_manager.hpp"  // Include SessionManager
#include "pty_session.hpp"
#include "dispatcher.hpp"

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <atomic>

static const char* ASCII_ART = 
"/  __ \\/ __  \\|  \\/  ||  ___/ __  \\                                        \n"
"| /  \\/`' / /'| .  . || |__ `' / /'                                        \n"
"| |      / /  | |\\/| ||  __|  / /                                          \n"
"| \\__/\\./ /___| |  | || |___./ /___                                        \n"
" \\____/\\_____/\\_|  |_/\\____/\\_____/                                        \n"
"                                                                             \n"
"                                                                             \n"
" _____ __  ______ _____ _   _     _____                       _       _       \n"
"/  ___/  | | ___ \\  ___| \\ | |   / /_  |                     | |     | |      \n"
"\\ `--.`| | | |_/ / |__ |  \\| |  / /  | | ___  _ __   __ _ ___| |_   _| | __   \n"
" `--. \\| | |    /|  __|| . ` | / /   | |/ _ \\| '_ \\ / _` / __| | | | | |/ /   \n"
"/\\__/ /| |_| |\\ \\| |___| |\\  |/ //\\__/ / (_) | | | | (_| \\__ \\ | |_| |   <    \n"
"\\____/ \\___/\\_| \\_\\____/\\_| \\_/_/ \\____/ \\___/|_| |_|\\__,_|___/_|\\__,_|_|\\_\\ \n"
"                                                                     \n"
"                                                                     \n";

static std::atomic<bool> g_running{true};

void acceptConnections(int server_fd) {
    while (g_running) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            if (!g_running) break;
            c2me2::common::logError("Failed to accept client connection.");
            continue;
        }

        char ip[INET_ADDRSTRLEN];
        if (!inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip))) {
            c2me2::common::logError("Failed to convert client IP.");
            close(client_fd);
            continue;
        }

        std::string client_ip = ip;
        c2me2::common::logInfo("New connection from: " + client_ip);

        auto session = new c2me2::server::PTYSession(client_fd, client_ip);
        c2me2::server::SessionManager::instance().addSession(session);  // Now recognized
    }
}

int main() {
    // Load configuration
    c2me2::common::Config::instance().loadConfig("config.json");

    // Set up logging
    std::string loggingLevel = c2me2::common::Config::instance().getLoggingLevel();
    if (loggingLevel == "Debug") {
        c2me2::common::Logger::instance().setLogLevel(c2me2::common::LogLevel::Debug);
    } else if (loggingLevel == "Info") {
        c2me2::common::Logger::instance().setLogLevel(c2me2::common::LogLevel::Info);
    } else if (loggingLevel == "Warning") {
        c2me2::common::Logger::instance().setLogLevel(c2me2::common::LogLevel::Warning);
    } else if (loggingLevel == "Error") {
        c2me2::common::Logger::instance().setLogLevel(c2me2::common::LogLevel::Error);
    }

    int port = c2me2::common::Config::instance().getPort();
    c2me2::common::logInfo("Starting C2Me2 TCP server on port " + std::to_string(port) + "...");

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        c2me2::common::logError("Failed to create socket.");
        return 1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    serv_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        c2me2::common::logError("Failed to bind to port " + std::to_string(port) + ".");
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 5) < 0) {
        c2me2::common::logError("Failed to listen on socket.");
        close(server_fd);
        return 1;
    }

    c2me2::common::logInfo("Server listening on port " + std::to_string(port) + ". Awaiting connections...");

    // Print ASCII art
    std::cout << ASCII_ART << "\n";

    std::thread accept_thread(acceptConnections, server_fd);

    while (true) {
        std::cout << "c2me2> ";
        std::fflush(stdout);
        std::string line;
        if (!std::getline(std::cin, line)) {
            break;
        }

        bool dummyInteractiveMode = false;
        int dummyInteractiveSession = -1;
        c2me2::server::Dispatcher::handleCommand(line, dummyInteractiveMode, dummyInteractiveSession);
    }

    g_running = false;
    shutdown(server_fd, SHUT_RDWR);
    close(server_fd);
    if (accept_thread.joinable()) {
        accept_thread.join();
    }
    return 0;
}
