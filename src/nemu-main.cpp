#include <iostream>
#include "monitor.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <program> [args ...]\n";
    }
    
    Monitor monitor{argv[1]};

    monitor.cpu_exec(-1);

    return 0;
}