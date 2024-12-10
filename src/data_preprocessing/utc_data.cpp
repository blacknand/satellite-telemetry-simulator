#include "utc_data.h"

// TODO: Update UTC time using sys clock
std::string update_utc_time() {
    const auto now = std::chrono::system_clock::now();
    const std::time_t t_c = std::chrono::system_clock::to_time_t(now);
    return "0";
}