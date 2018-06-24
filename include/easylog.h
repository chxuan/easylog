#pragma once
/*
功能:日志库
日期:2018-06-22
作者:chxuan <787280310@qq.com>
*/
#include "log_file_proxy.h"
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

class easylog
{
public:
    // 获取单例对象
    static easylog& get() { static easylog inst; return inst; }
    // 设置日志输出目录
    void set_log_dir(const std::string& dir) { log_dir_ = dir; mkdir(dir); }
    // 设置日志输出等级
    void set_log_level(log_level level) { level_ = level; }
    // 设置日志文件大小
    void set_log_file_size(unsigned long long file_size) { file_size_ = file_size; }
    // 输出日志
    template<typename... Args>
    inline void log(log_level level, const char* file_name, unsigned long line, const char* fmt, Args&&... args);

private:
    easylog() : all_proxy_(log_dir_, "ALL"), warn_proxy_(log_dir_, "WARN"), 
                error_proxy_(log_dir_, "ERROR"), fatal_proxy_(log_dir_, "FATAL") {}
    ~easylog() {}

    // 判断是否输出日志
    bool is_logged(log_level level) { return level >= level_; }
    // 创建日志
    inline std::string create_log(log_level level, const char* file_name, unsigned long line, const std::string& content);
    // 将日志等级转换成字符串
    inline const char* to_string(log_level level);
    // 写到日志文件
    inline void write_log(log_level level, const std::string& log);

private:
    std::string log_dir_ = "./log";
    log_level level_ = log_level::all;
    unsigned long long file_size_ = 100 * 1024 * 1024; // 100MB

    log_file_proxy all_proxy_;
    log_file_proxy warn_proxy_;
    log_file_proxy error_proxy_;
    log_file_proxy fatal_proxy_;
};

template<typename... Args>
void easylog::log(log_level level, const char* file_name, unsigned long line, const char* fmt, Args&&... args)
{
    if (is_logged(level))
    {
        std::string log = create_log(level, file_name, line, format(fmt, std::forward<Args>(args)...));
        write_log(level, log);
    }
}

std::string easylog::create_log(log_level level, const char* file_name, unsigned long line, const std::string& content)
{
    std::string log;
    log += to_string(level);
    log += get_current_time_ms();
    log += " ";
    log += file_name;
    log += ":";
    log += std::to_string(line);
    log += "] ";
    log += content;
    log += "\n";

    return log;
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

void easylog::write_log(log_level level, const std::string& log)
{
    if (level == log_level::warn)
    {
        warn_proxy_.write_log(log);
    }
    else if (level == log_level::error)
    {
        error_proxy_.write_log(log);
    }
    else if (level == log_level::fatal)
    {
        fatal_proxy_.write_log(log);
    }

    all_proxy_.write_log(log);
}

#define FILE_LINE basename(const_cast<char*>(__FILE__)), __LINE__

#define set_log_dir(dir) easylog::get().set_log_dir(dir)
#define set_log_level(level) easylog::get().set_log_level(level)
#define set_log_file_size(file_size) easylog::get().set_log_file_size(file_size)

#define log_all(...) easylog::get().log(log_level::all, FILE_LINE, __VA_ARGS__)
#define log_trace(...) easylog::get().log(log_level::trace, FILE_LINE, __VA_ARGS__)
#define log_debug(...) easylog::get().log(log_level::debug, FILE_LINE, __VA_ARGS__)
#define log_info(...) easylog::get().log(log_level::info, FILE_LINE, __VA_ARGS__)
#define log_warn(...) easylog::get().log(log_level::warn, FILE_LINE, __VA_ARGS__)
#define log_error(...) easylog::get().log(log_level::error, FILE_LINE, __VA_ARGS__)
#define log_fatal(...) easylog::get().log(log_level::fatal, FILE_LINE, __VA_ARGS__)
