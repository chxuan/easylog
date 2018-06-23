#include "include/easylog.h"

int main()
{
    std::string name = "jack";
    int age = 20;

    set_log_dir("./log");
    set_log_level(log_level::warn);

    log_all("name:{}--age:{}", "chxuan", 3.124);
    log_trace("name:{}--age:{}", name, age);
    log_debug("name:{}--age:{}", name, age);
    log_info("name:{}--age:{}", name, age);
    log_warn("name:{}--age:{}", name, age);
    log_error("name:{}--age:{}", name, age);
    log_fatal("name:{}--age:{}", name, age);

    log_info("hello world");

    return 0;
}

