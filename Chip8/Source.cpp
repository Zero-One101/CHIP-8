#include "Cpu.h"

#include <iostream>
#include <direct.h>
#include <Windows.h>

Cpu cpu;

int main(int argc, char* argv[])
{
    char* gameFile;

    if (argc <= 1)
    {
#ifndef _DEBUG
        std::cout << "Please pass a game file as an argument." << std::endl;
        system("pause");
        return 1;
#endif

        gameFile = "./ROMs/invaders.c8";
    }
    else
    {
        gameFile = argv[1];
    }

    cpu.Init();
    cpu.LoadGame(gameFile);

    while (true)
    {
        cpu.AdvanceCycle();
        Sleep(100);
    }

    return 0;
}