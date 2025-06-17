#include "SyncSignal.h"

std::condition_variable global_cv;
std::mutex global_mtx;
std::atomic<bool>stop_flag{false};
