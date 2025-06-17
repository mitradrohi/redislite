// ThreadPoolServerHandler.h
#pragma once
#include "ThreadPool.h"
#include "CommandHandler.h"
#include <netinet/in.h>
#include <atomic>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

extern std::promise<bool> start_promise;
extern std::future<bool> start_future;
extern std::once_flag once;

class ThreadPoolServerHandler {
public:
    ThreadPoolServerHandler(int port, KeyValueStore& kv, size_t threadCount = 4)
        : port(port), pool(threadCount), cmdHandler(kv), running(false) {}

    void run();

    void stop() ;
       
    

private:
    void handleClient(int client_fd);

    int port;
    int server_fd;
    ThreadPool pool;
    CommandHandler cmdHandler;
    std::atomic<bool> running;
};
