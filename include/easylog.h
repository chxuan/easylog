#pragma once
/*
功能:日志库
日期:2018-06-22
作者:chxuan <787280310@qq.com>
*/
#include <iostream>
#include <atomic>
#include "utility.h"
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

class file_proxy
{
public:
    file_proxy(const std::string& dir, const std::string& level) : dir_(dir), level_(level) {}
    ~file_proxy()
    {
        if (fd_ != -1)
        {
            close(fd_);
        }
    }

    // 写文件
    inline void write(const std::string& text);

private:
    std::string dir_;
    std::string level_;
    std::atomic<bool> is_opened_ {false};
    int fd_ = -1;
};

void file_proxy::write(const std::string& text)
{
    if (is_opened_)
    {

    }
    else
    {
        std::string exe_name = get_current_exe_name();
        std::string link_file = dir_ + "/" + exe_name + "." + level_;

        std::string time_ms = get_current_time_ms();
        std::string base_name = exe_name + "." + level_ + "_" + time_ms;
        std::string file_name = dir_ + "/" + base_name;

        fd_ = open(file_name.c_str(), O_CREAT | O_RDWR | O_APPEND | O_SYNC, 0664);
        if (fd_ != -1)
        {
            if (symlink(base_name.c_str(), link_file.c_str()) == -1)
            {
                std::cout << "Create link file failed, link file:" << link_file << std::endl;
            }
        }
        else
        {
            std::cout << "Open file faield, file name:" << file_name << std::endl;
        }
    }
}

class easylog
{
public:
    // 获取单例对象
    static easylog& get() { static easylog inst; return inst; }
    // 设置日志输出目录
    void set_log_dir(const std::string& dir) { log_dir_ = dir; mkdir(dir); }
    // 设置日志输出等级
    void set_log_level(log_level level) { level_ = level; }
    // 输出日志
    template<typename... Args>
        inline void log(log_level level, const char* file_name, unsigned long line, const char* fmt, Args&&... args);

private:
    easylog() : all_proxy_(log_dir_, "ALL"), warn_proxy_(log_dir_, "WARN"), 
    error_proxy_(log_dir_, "ERROR"), fatal_proxy_(log_dir_, "FATAL") {}
    ~easylog() {}

    // 判断是否输出日志
    bool is_logged(log_level level) { return level >= level_; }
    // 创建日志文本
    inline std::string create_log_text(log_level level, const char* file_name, unsigned long line, const std::string& content);
    // 将日志等级转换成字符串
    inline const char* to_string(log_level level);
    // 写到日志文件
    inline void write_log_file(log_level level, const std::string& text);

private:
    std::string log_dir_ = "./log";
    log_level level_ = log_level::all;

    file_proxy all_proxy_;
    file_proxy warn_proxy_;
    file_proxy error_proxy_;
    file_proxy fatal_proxy_;
};

    template<typename... Args>
void easylog::log(log_level level, const char* file_name, unsigned long line, const char* fmt, Args&&... args)
{
    if (is_logged(level))
    {
        std::string text = create_log_text(level, file_name, line, format(fmt, std::forward<Args>(args)...));
        write_log_file(level, text);
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

void easylog::write_log_file(log_level level, const std::string& text)
{
    if (level == log_level::warn)
    {
        warn_proxy_.write(text);
    }
    else if (level == log_level::error)
    {
        error_proxy_.write(text);
    }
    else if (level == log_level::fatal)
    {
        fatal_proxy_.write(text);
    }

    all_proxy_.write(text);
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
