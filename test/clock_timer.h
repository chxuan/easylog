#pragma once
/*
功能:计时器
日期:2018-06-24
作者:chxuan <787280310@qq.com>
*/
#include <chrono>

class clock_timer
{
public:
    clock_timer() : begin_(std::chrono::high_resolution_clock::now()) {}

    // 重置计时器
    void reset() { begin_ = std::chrono::high_resolution_clock::now(); }
    // 微秒
    long long elapsed_microseconds() const { return elapsed<std::chrono::microseconds>(); }
    // 纳秒
    long long elapsed_nanoseconds() const { return elapsed<std::chrono::nanoseconds>(); }
    // 秒
    long long elapsed_seconds() const { return elapsed<std::chrono::seconds>(); }
    // 分
    long long elapsed_minutes() const { return elapsed<std::chrono::minutes>(); }
    // 时
    long long elapsed_hours() const { return elapsed<std::chrono::hours>(); }
    // 默认输出毫秒
    template<typename Duration = std::chrono::milliseconds>
    long long elapsed() const { return std::chrono::duration_cast<Duration>(std::chrono::high_resolution_clock::now() - begin_).count(); }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> begin_;
};
