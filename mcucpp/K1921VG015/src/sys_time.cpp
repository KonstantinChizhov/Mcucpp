#include <chrono>
#include <csr.h>
#include <clock.h>
#include <mtimer.h>
#include <K1921VG015.h>
#include <sys_time.h>
#include <rtc.h>

Mcucpp::fast_clock::time_point Mcucpp::fast_clock::now() noexcept
{
    return Mcucpp::fast_clock::time_point{Mcucpp::fast_clock::duration{rdtime()}};
}

Mcucpp::rtc_clock::time_point Mcucpp::rtc_clock::now() noexcept
{
    return Mcucpp::rtc_clock::time_point{Mcucpp::rtc_clock::duration{PMURTC->RTC_TIME}};
}

void Mcucpp::rtc_clock::set_time(time_point time) noexcept
{
    PMURTC->RTC_TIME = time.time_since_epoch().count();
}

std::chrono::steady_clock::time_point std::chrono::steady_clock::now() noexcept
{
    return steady_clock::time_point{nanoseconds{Mcucpp::Clock::SysClock::CyclesToNs(mtimer_get_raw_time())}};
}

std::chrono::system_clock::time_point std::chrono::system_clock::now() noexcept
{
    return system_clock::time_point{seconds{PMURTC->RTC_TIME}};
}
