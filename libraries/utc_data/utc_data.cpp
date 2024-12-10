#include "utc_data.h"
int main() { std::cout << get_utc_time() << std::endl;; return 0; }
// TODO: Update UTC time using sys clock
std::string get_utc_time() {
    const auto now = std::chrono::system_clock::now();
    const std::time_t t_c = std::chrono::system_clock::to_time_t(now);
    tm utc_time_struct;
    tm* utc_time = gmtime_r(&t_c, &utc_time_struct);
    std::ostringstream oss;
    oss << "UTC time: " << 1900 + utc_time->tm_year << "-"
            << 1 + utc_time->tm_mon << "-"
            << utc_time->tm_mday << "T"
            << utc_time->tm_hour << ":"
            << utc_time->tm_min << ":"
            << utc_time->tm_sec << std::endl;
    return oss.str();
}