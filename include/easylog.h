#pragma once

#include <iostream>

class easylog
{
public:
    easylog() {}
    ~easylog() {}

    inline void print();

private:

};

void easylog::print()
{
    std::cout << "hello" << std::endl;
}

