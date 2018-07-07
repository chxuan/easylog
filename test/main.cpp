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
        log_file_debug("hello world");
        log_file_warn("hello world");
    }

    std::cout << "线程:" << pthread_self() << ", 耗时:" << timer.elapsed() << "ms" << std::endl;
}

int main()
{
    init_easylog("./log", log_level::all, 100 * 1024 * 1024);

    /* log_test(); */
    std::thread t1([]{ log_test(); });
    std::thread t2([]{ log_test(); });

    t1.join();
    t2.join();

    /* clock_timer timer; */
    /* for (int i = 1; i <= 1000000; ++i) */
    /* { */
    /*     log_file_debug("12345678901234567890123456789012345678901234567890123456789012345678901234567890name:{}--age:{}", "chxuan", 3.124); */
    /*     /1* log_file_debug("hello world"); *1/ */
    /* } */

    /* std::cout << "线程:" << std::this_thread::get_id() << ", 耗时:" << timer.elapsed() << "ms" << std::endl; */

    /* std::cout << "线程:" << std::this_thread::get_id() << ", 耗时:" << timer.elapsed() << "ms" << std::endl; */
    /* std::string name = "Jack"; */
    /* int age = 20; */
    /* log_file_debug("name:{}--age:{}", name, age); */
    /* char* p = NULL; */
    /* *p = 1; */
    /* log_file_debug("name:{}--age:{}", name, age); */
    /* log_file_debug("name:--age:"); */
    /* log_debug("name:{}--age:{}", name, age); */
    /* log_info("name:{}--age:{}", name, age); */
    /* log_warn("name:{}--age:{}", name, age); */
    /* log_error("name:{}--age:{}", name, age); */
    /* log_fatal("name:{}--age:{}", name, age); */

    return 0;
}

