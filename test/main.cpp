#include "../include/easylog.h"
#include "clock_timer.h"
#include <thread>
#include <iostream>

void log_test()
{
    clock_timer timer;
    for (int i = 1; i <= 10000000; ++i)
    {
        /* log_file_debug("12345678901234567890123456789012345678901234567890123456789012345678901234567890name:{}--age:{}", "chxuan", 3.124); */
        LOG_FILE_DEBUG("hello world");
        LOG_FILE_WARN("hello world");
    }

    std::cout << "线程:" << pthread_self() << ", 耗时:" << timer.elapsed() << "ms" << std::endl;
}

int main()
{
    INIT_EASYLOG("./log", log_level::all, 100 * 1024 * 1024);

    /* std::thread t1([]{ log_test(); }); */
    /* std::thread t2([]{ log_test(); }); */

    /* t1.join(); */
    /* t2.join(); */

    /* clock_timer timer; */
    /* for (int i = 1; i <= 1000000; ++i) */
    /* { */
    /*     LOG_FILE_DEBUG("12345678901234567890123456789012345678901234567890123456789012345678901234567890name:{}--age:{}", "chxuan", 3.124); */
    /* } */

    /* std::cout << "线程:" << std::this_thread::get_id() << ", 耗时:" << timer.elapsed() << "ms" << std::endl; */

    /* std::cout << "线程:" << std::this_thread::get_id() << ", 耗时:" << timer.elapsed() << "ms" << std::endl; */
    std::string name = "Jack";
    int age = 20;
    char age2 = '1';
    LOG_FILE_DEBUG("name:{}--age:{}", name, age);
    LOG_FILE_DEBUG("name:{}--age:{}", name, age);
    LOG_FILE_DEBUG("name:--age:");
    LOG_DEBUG("name:{}--age:{}", name, age);
    LOG_INFO("name:{}--age:{}", name, age);
    LOG_WARN("name:{}--age:{}", name, age);
    LOG_ERROR("name:{}--age:{}", name, age);
    LOG_FATAL("name:{}--age2:{}", name, age2);

    return 0;
}

