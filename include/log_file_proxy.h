#pragma once
/*
功能:日志文件代理
日期:2018-06-24
作者:chxuan <787280310@qq.com>
*/
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <atomic>
#include <mutex>
#include "utility.h"

class log_file_proxy
{
public:
    ~log_file_proxy() { close_file(fd_); }

    // 初始化日志文件
    inline void init_log_file(const std::string& output_dir, const std::string& level, unsigned long long max_file_size);
    // 写日志
    inline void write_log(const std::string& log);

private:
    // 创建文件
    inline void create_file();
    // 创建软链接
    inline void create_link(const std::string& time_ms);
    // 写文件
    inline void write_file(const std::string& log);
    // 判断文件是否打开
    bool is_opened() { return fd_ > 0; }
    // 关闭文件
    void close_file(int fd) { if (fd > 0) { close(fd); printf("关闭文件成功, fd:%d, 线程id:%lu\n", fd, pthread_self()); } }

private:
    std::string file_name_;
    std::string file_name_with_dir_;
    unsigned long long max_file_size_;
    std::atomic<unsigned long long> curr_file_size_ {0};
    std::atomic<int> fd_ {-1};
    std::mutex mt_;
};

void log_file_proxy::init_log_file(const std::string& output_dir, const std::string& level, unsigned long long max_file_size)
{
    max_file_size_ = max_file_size;
    file_name_ = get_current_exe_name() + "." + level;
    file_name_with_dir_ = output_dir + "/" + file_name_;
}

void log_file_proxy::write_log(const std::string& log)
{
    if (!is_opened())
    {
        std::lock_guard<std::mutex> lock(mt_);
        if (!is_opened())
        {
            create_file();
        }
    }

    if (curr_file_size_ >= max_file_size_)
    {
        std::unique_lock<std::mutex> lock(mt_, std::defer_lock);
        if (lock.try_lock())
        {
            create_file();
        }
    }

    write_file(log);
}

void log_file_proxy::create_file()
{
    std::string time_ms = get_current_time_ms();
    std::string file_name = file_name_with_dir_ + "_" + time_ms;

    int last_fd = fd_;
    int fd = open(file_name.c_str(), O_CREAT | O_EXCL | O_RDWR | O_APPEND, 0664);
    if (fd > 0)
    {
        fd_ = fd;
        curr_file_size_ = 0;
        printf("打开日志文件成功, fd:%d, 线程id:%lu, 文件名:%s\n", fd_.load(), pthread_self(), time_ms.c_str());

        close_file(last_fd);
        create_link(time_ms);
    }
    else
    {
        printf("打开日志文件失败, fd:%d, 线程id:%lu, 错误原因:%s, 文件名:%s\n", fd, pthread_self(), strerror(errno), file_name.c_str());
    }
}

void log_file_proxy::create_link(const std::string& time_ms)
{
    std::string log_file = file_name_ + "_" + time_ms;

    remove(file_name_with_dir_.c_str());
    if (symlink(log_file.c_str(), file_name_with_dir_.c_str()) == -1)
    {
        printf("创建日志链接文件失败, 线程id:%lu, 错误原因:%s, 日志文件:%s, 链接文件:%s\n", pthread_self(), strerror(errno), log_file.c_str(), file_name_with_dir_.c_str());
    }
    else
    {
        printf("创建日志链接文件成功, 线程id:%lu, 日志文件:%s, 链接文件:%s\n", pthread_self(), log_file.c_str(), file_name_with_dir_.c_str());
    }
}

void log_file_proxy::write_file(const std::string& log)
{
    int size = write(fd_, log.c_str(), log.length());
    if (size > 0)
    {
        curr_file_size_ += size;
    }
    else
    {
        printf("写日志到文件失败, fd:%d, 线程id:%lu, 错误原因:%s, 日志内容:%s\n", fd_.load(), pthread_self(), strerror(errno), log.c_str());
    }
}

