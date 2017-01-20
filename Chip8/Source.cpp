#include "Cpu.h"

#include <iostream>

int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        std::cout << "Please pass a game file as an argument." << std::endl;
        system("pause");
        return 1;
    }

    return 0;
}