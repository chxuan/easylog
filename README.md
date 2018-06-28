# An easy to use C++ log library

> easylog是采用C++开发的，使用方便的日志库。

![License][1] 

## Tutorial

* **Simple code**

    ```cpp
    #include "easylog.h"
    
    int main()
    {
        // 初始化easylog
        init_easylog("./log", log_level::debug, 1024 * 1024 * 1024);

        // 输出日志到控制台和文件
        log_info("name:{}, age:{}", "Jack", 20);

        // 输出日志到控制台
        log_console_debug("Current thread id:{}", std::this_thread::get_id());

        // 输出日志到文件
        log_file_fatal("System error");

        return 0;
    }
    ```

## License

This software is licensed under the [MIT license][2]. © 2018 chxuan

  [1]: http://img.shields.io/badge/license-MIT-blue.svg?style=flat-square
  [2]: https://github.com/chxuan/easylog/blob/master/LICENSE


