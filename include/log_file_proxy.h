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
    log_file_proxy(const std::string& dir, const std::string& level) : dir_(dir), level_(level) 
    {
        exe_name_ = get_current_exe_name();
        link_file_ = dir_ + "/" + exe_name_ + "." + level_;
    }

    ~log_file_proxy()
    {
        if (fd_ != -1)
        {
            close(fd_);
        }
    }

    // 写文件
    inline void write_log(const std::string& log);

private:
    // 打开文件
    inline bool open_file();
    // 写文件
    inline void write_to_file(const std::string& log);

private:
    std::string dir_;
    std::string level_;
    std::string exe_name_;
    std::string link_file_;
    std::atomic<bool> is_opened_ {false};
    int fd_ = -1;
};

void log_file_proxy::write_log(const std::string& log)
{
    printf("%s", log.c_str());

    if (is_opened_)
    {
        write_to_file(log);
    }
    else
    {
        if (open_file())
        {
            write_to_file(log);
        }
    }
}

bool log_file_proxy::open_file()
{
    std::string base_name = exe_name_ + "." + level_ + "_" + get_current_time_ms();
    std::string file_name = dir_ + "/" + base_name;

    fd_ = open(file_name.c_str(), O_CREAT | O_RDWR | O_APPEND | O_SYNC, 0664);
    if (fd_ != -1)
    {
        is_opened_ = true;

        ::remove(link_file_.c_str());
        if (symlink(base_name.c_str(), link_file_.c_str()) == -1)
        {
            printf("写日志时，创建链接文件失败:%s\n", link_file_.c_str());
        }

        return true;
    }

    printf("写日志时，打开文件失败:%s\n", file_name.c_str());

    return false;
}

void log_file_proxy::write_to_file(const std::string& log)
{
    if (::write(fd_, log.c_str(), log.length()) == -1)
    {
        printf("写日志到文件失败:%s\n", log.c_str());
    }
}

