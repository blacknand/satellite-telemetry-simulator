#include <chrono>

#include "utc_data.h"

std::string update_utc_time() {
    const auto now = std::chrono::system_clock::now();
    const std::time_t t_c = std::chrono::system_clock::to_time_t(now); 
}