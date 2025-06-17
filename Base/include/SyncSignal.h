#pragma once

#include<condition_variable>
#include <mutex>
#include <atomic>

extern std::condition_variable global_cv;
extern std::mutex global_mtx;
extern std::atomic<bool>stop_flag;
