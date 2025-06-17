#include<iostream>
#include"ServerHandler.h"
#include"KeyValueStore.h"
#include"SyncSignal.h"
#include<thread>
#include<pthread.h>
#include<atomic>
#include<csignal>
#include"thread"
#include"ThreadPoolServerHandler.h"
#include <future>
std::promise<bool> start_promise;
std::future<bool> start_future=start_promise.get_future();
std::once_flag once;

void wait_for_signal() {
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);
    sigaddset(&sigset, SIGTERM);
    
    int sig;
    sigwait(&sigset, &sig);  // blocks here

    std::cout << "Received signal: " << sig << "\n";
    {
        std::lock_guard<std::mutex> lock(global_mtx);
        stop_flag = true;
    }
    global_cv.notify_all();
}


int main() {
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);
    sigaddset(&sigset, SIGTERM);
    pthread_sigmask(SIG_BLOCK, &sigset, nullptr);

    std::thread signal_thread(wait_for_signal);

    KeyValueStore kv;
    ThreadPoolServerHandler srhd(1080, kv, 5);

 
    
    

    std::thread serverThread([&]() {
        try {
            srhd.run(); // If bind() fails, it will throw here
           std::call_once(once, [&] { start_promise.set_value(true); });
        } catch (const std::exception& ex) {
            std::call_once(once, [&] { start_promise.set_value(false); });
            std::cerr << "Server error: " << ex.what() << std::endl;
        }
    });

    {
        std::unique_lock<std::mutex> lock(global_mtx);
        global_cv.wait(lock, [] { return stop_flag.load(); });
    }

    std::cout << "Shutting down server...\n";

    if (start_future.get()) {
        std::cout<<"inside serverstareted delteding\n";
        srhd.stop(); 
    }

    if (serverThread.joinable())
        serverThread.join();

    signal_thread.join();
    return 0;
}
