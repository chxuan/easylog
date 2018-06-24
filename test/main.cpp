#include "include/easylog.h"
#include "clock_timer.h"
#include <iostream>

int main()
{
    init_easylog("./log", log_level::all, 100 * 1024 * 1024);

    clock_timer timer;
    for (int i = 1; i <= 1000000; ++i)
    {
        log_file_debug("12345678901234567890123456789012345678901234567890123456789012345678901234567890name:{}--age:{}", "chxuan", 3.124);
    }

    std::cout << "耗时:" << timer.elapsed() << "ms" << std::endl;

    /* log_trace("name:{}--age:{}", name, age); */
    /* log_debug("name:{}--age:{}", name, age); */
    /* log_info("name:{}--age:{}", name, age); */
    /* log_warn("name:{}--age:{}", name, age); */
    /* log_error("name:{}--age:{}", name, age); */
    /* log_fatal("name:{}--age:{}", name, age); */

    return 0;
}

