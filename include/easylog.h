#pragma once
/*
功能:日志库
日期:2018-06-22
作者:chxuan <787280310@qq.com>
*/
#include <unistd.h>
#include <sys/time.h>
#include <libgen.h>
#include <iostream>
#include "format.h"

enum class log_level
{
    all = 0,
    trace,
    debug,
    info,
    warn,
    error,
    fatal
};

// 获得当前可执行文件名
inline std::string get_current_exe_name()
{
    char buf[1024] = {'\0'};

    if (readlink("/proc/self/exe", buf, sizeof(buf)) != -1)
    {
        return basename(buf);
    }

    return "";
}

// 获取当前时间,精确到毫秒 2018-06-23 16:39:50.131
inline std::string get_current_time_ms()
{
    struct timeval now_tv;
    gettimeofday(&now_tv, nullptr);

    struct tm t;
    localtime_r(&now_tv.tv_sec, &t);

    char str[24] = {"\0"};
    snprintf(str, sizeof(str), "%04d-%02d-%02d %02d:%02d:%02d.%03d", 
             t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, 
             t.tm_sec, static_cast<int>(now_tv.tv_usec / 1000));

    return str;
}

class file_io
{
public:

private:

};

class easylog
{
public:
    // 获取单例对象
    static easylog& get() { static easylog inst; return inst; }
    // 设置日志输出目录
    void set_log_dir(const std::string& dir) { log_dir_ = dir; }
    // 设置日志输出等级
    void set_log_level(log_level level) { level_ = level; }
    // 判断是否输出日志
    bool is_logged(log_level level) { return level >= level_; }
    // 输出日志
    template<typename... Args>
    inline void log(log_level level, const char* file_name, unsigned long line, const char* fmt, Args&&... args);
    // 将日志等级转换成字符串
    inline const char* to_string(log_level level);
    // 创建日志文本
    inline std::string create_log_text(log_level level, const char* file_name, unsigned long line, const std::string& content);

private:
    easylog() { exe_name_ = get_current_exe_name().c_str(); }
    ~easylog() {}

private:
    std::string log_dir_ = "./log";
    log_level level_ = log_level::debug;
    std::string exe_name_;
};

template<typename... Args>
void easylog::log(log_level level, const char* file_name, unsigned long line, const char* fmt, Args&&... args)
{
    if (is_logged(level))
    {
        std::string text = create_log_text(level, file_name, line, format(fmt, std::forward<Args>(args)...));
        std::cout << text << std::endl;
    }
}

std::string easylog::create_log_text(log_level level, const char* file_name, unsigned long line, const std::string& content)
{
    std::string text;
    text += "[";
    text += to_string(level);
    text += get_current_time_ms();
    text += " ";
    text += file_name;
    text += ":";
    text += std::to_string(line);
    text += "] ";
    text += content;

    return text;
}

const char* easylog::to_string(log_level level)
{
    switch (level)
    {
    case log_level::all: return "A";
    case log_level::trace: return "T";
    case log_level::debug: return "D";
    case log_level::info: return "I";
    case log_level::warn: return "W";
    case log_level::error: return "E";
    case log_level::fatal: return "F";
    default: return "U";
    }
}

#define FILE_LINE basename(const_cast<char*>(__FILE__)), __LINE__

#define set_log_dir(dir) easylog::get().set_log_dir(dir)
#define set_log_level(level) easylog::get().set_log_level(level)

#define log_all(...) easylog::get().log(log_level::all, FILE_LINE, __VA_ARGS__)
#define log_trace(...) easylog::get().log(log_level::trace, FILE_LINE, __VA_ARGS__)
#define log_debug(...) easylog::get().log(log_level::debug, FILE_LINE, __VA_ARGS__)
#define log_info(...) easylog::get().log(log_level::info, FILE_LINE, __VA_ARGS__)
#define log_warn(...) easylog::get().log(log_level::warn, FILE_LINE, __VA_ARGS__)
#define log_error(...) easylog::get().log(log_level::error, FILE_LINE, __VA_ARGS__)
#define log_fatal(...) easylog::get().log(log_level::fatal, FILE_LINE, __VA_ARGS__)
