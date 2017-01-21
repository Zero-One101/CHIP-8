#include "Cpu.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <iterator>

Cpu::Cpu()
{
    
}

Cpu::~Cpu()
{

}

void Cpu::Init()
{
    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;

    memset(memory, 0, memorySize);
    memset(gfx, 0, displaySize);
    memset(stack, 0, stackSize);
    memset(V, 0, registerCount);
    std::memcpy(memory, fontset, 80);

    delayTimer = 0;
    soundTimer = 0;
}

bool Cpu::LoadGame(std::string const &gameName)
{
    std::ifstream input(gameName, std::ios::binary);
    std::istream_iterator<char> start(input), end;
    std::vector<char> buffer(start, end);

    memcpy(memory + 0x200, buffer.data(), buffer.size());

    return true;
}

void Cpu::AdvanceCycle()
{
    FetchOpcode();
    DecodeOpcode();
}

void Cpu::FetchOpcode()
{
    /* Each opcode is 2 bytes, so load the first byte
    then shift left by 8, then load the second byte */
    opcode = memory[pc] << 8 | memory[pc + 1];
}

void Cpu::DecodeOpcode()
{
    // To be replaced with function pointers
    switch (opcode & 0xF000)
    {
        case 0x0000:
        {
            switch (opcode & 0x000F)
            {
                default:
                    printf("Unknown opcode [0x0000]: 0x%.4X\n", opcode);
            }
        }
        case 0x8000:
        {
            switch (opcode & 0x000F)
            {
                default:
                    printf("Unknown opcode [0x8000]: 0x%.4X\n", opcode);
            }
        }
        case 0xE000:
        {
            switch (opcode & 0x00FF)
            {
                default:
                    printf("Unknown opcode [0xE000]: 0x%.4X\n", opcode);
            }
        }
        case 0xF000:
        {
            switch (opcode & 0x00FF)
            {
                default:
                    printf("Unknown opcode [0xF000]: 0x%.4X\n", opcode);
            }
        }
        default:
            printf("Unknown opcode: 0x%.4X\n", opcode);
    }
}