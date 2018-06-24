#pragma once
/*
功能:日志文件代理
日期:2018-06-24
作者:chxuan <787280310@qq.com>
*/
#include <stdio.h>
#include <atomic>
#include "utility.h"

class log_file_proxy
{
public:
    ~log_file_proxy() { if (fd_ != -1) { close(fd_); } }

    // 初始化日志文件
    inline void init_log_file(const std::string& output_dir, const std::string& level, unsigned long long max_file_size);
    // 写日志
    inline void write_log(const std::string& log);

private:
    // 打开文件
    inline bool open_file(const std::string& time_ms);
    // 创建软链接
    inline void create_link(const std::string& time_ms);
    // 写文件
    inline void write_file(const std::string& log);

private:
    std::string file_name_;
    std::string file_name_with_dir_;
    unsigned long long max_file_size_;
    std::atomic<unsigned long long> curr_file_size_ {0};
    std::atomic<bool> is_opened_ {false};
    int fd_ = -1;
};

void log_file_proxy::init_log_file(const std::string& output_dir, const std::string& level, unsigned long long max_file_size)
{
    max_file_size_ = max_file_size;
    file_name_ = get_current_exe_name() + "." + level;
    file_name_with_dir_ = output_dir + "/" + file_name_;
}

void log_file_proxy::write_log(const std::string& log)
{
    if (is_opened_)
    {
        write_file(log);
    }
    else
    {
        std::string time_ms = get_current_time_ms();
        if (open_file(time_ms))
        {
            create_link(time_ms);
            write_file(log);
        }
    }
}

bool log_file_proxy::open_file(const std::string& time_ms)
{
    std::string file_name = file_name_with_dir_ + "_" + time_ms;

    fd_ = open(file_name.c_str(), O_CREAT | O_RDWR | O_APPEND, 0664);
    if (fd_ != -1)
    {
        is_opened_ = true;
        return true;
    }

    printf("写日志时，打开文件失败:%s\n", file_name.c_str());

    return false;
}

void log_file_proxy::create_link(const std::string& time_ms)
{
    std::string log_file = file_name_ + "_" + time_ms;

    remove(file_name_with_dir_.c_str());
    if (symlink(log_file.c_str(), file_name_with_dir_.c_str()) == -1)
    {
        printf("写日志时，创建链接文件失败:%s\n", file_name_with_dir_.c_str());
    }
}

void log_file_proxy::write_file(const std::string& log)
{
    int size = write(fd_, log.c_str(), log.length());
    if (size != -1)
    {
        curr_file_size_ += size;
    }
    else
    {
        printf("写日志到文件失败:%s\n", log.c_str());
    }
}

