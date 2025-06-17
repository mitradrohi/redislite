#include "ThreadPoolServerHandler.h"
#include <fcntl.h>
#include <errno.h>

bool isFdValid(int fd) {
    return (fcntl(fd, F_GETFD)!=-1);
}

void ThreadPoolServerHandler::run() {
    sockaddr_in server_addr;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) throw std::runtime_error("Socket creation failed");

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
        throw std::runtime_error("Bind failed");

    if (listen(server_fd, 10) < 0)
        throw std::runtime_error("Listen failed");

    std::cout << "Server listening on port " << port << std::endl;
    running = true;

    std::call_once(once, [&] {start_promise.set_value(true); });
    while (running) {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

        if (client_fd < 0) {
            if (running) {
                std::cerr << "Accept failed: " << strerror(errno) << "\n";
            }
            continue;
        }
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
        std::cout << "Connection from: " << client_ip << ":" << ntohs(client_addr.sin_port) << "\n";

        pool.enqueue([this, client_fd] { handleClient(client_fd); });
    }
}

void ThreadPoolServerHandler::stop() {
    running = false;

    if (isFdValid(server_fd)) {
        // Wake up the accept() call with a dummy connection
        int dummy_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (dummy_fd >= 0) {
            sockaddr_in dummy_addr{};
            dummy_addr.sin_family = AF_INET;
            dummy_addr.sin_port = htons(port);
            inet_pton(AF_INET, "127.0.0.1", &dummy_addr.sin_addr);
            connect(dummy_fd, (sockaddr*)&dummy_addr, sizeof(dummy_addr));
            close(dummy_fd);
        }

        std::cout << "Closing server fd\n";
        close(server_fd);
    }
    std::cout << "Server stopped cleanly.\n";
}

void ThreadPoolServerHandler::handleClient(int client_fd) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes = read(client_fd, buffer, sizeof(buffer) - 1);
        if (bytes <= 0) break;

        std::string command(buffer);
        std::string response = cmdHandler.handle(command);
        send(client_fd, response.c_str(), response.length(), 0);
    }
    close(client_fd);
}

