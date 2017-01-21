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
    input >> std::noskipws; // Don't skip whitespace, or else the ROM will be read incorrectly
    std::istream_iterator<char> start(input), end;
    std::vector<char> buffer(start, end);

    memcpy(memory + 0x200, buffer.data(), buffer.size());

    return true;
}

bool Cpu::ShouldDraw()
{
    return shouldDraw;
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
            switch (opcode & 0x00FF)
            {
                default:
                    printf("Unknown opcode [0x0000]: 0x%.4X\n", opcode);
                    break;
            }
        }
        break;

        case 0x1000:
        {
            JumpToAddress();
            break;
        }
        break;

        case 0x2000:
        {
            JumpToSubroutine();
        }
        break;

        case 0x3000:
        {
            SkipIfRegisterEqual();
        }
        break;

        case 0x6000:
        {
            SetRegister();
        }
        break;

        case 0x7000:
        {
            AddToRegister();
        }
        break;

        case 0x8000:
        {
            switch (opcode & 0x000F)
            {
                case 0x0000:
                {
                    AssignRegisterToRegister();
                }
                break;

                default:
                    printf("Unknown opcode [0x8000]: 0x%.4X\n", opcode);
                    break;
            }
        }
        break;

        case 0xA000:
        {
            SetIndex();
        }
        break;

        case 0xD000:
        {
            Draw();
        }
        break;

        case 0xE000:
        {
            switch (opcode & 0x00FF)
            {
                case 0x00A1:
                {
                    SkipIfKeyUp();
                }
                break;

                default:
                    printf("Unknown opcode [0xE000]: 0x%.4X\n", opcode);
                    break;
            }
        }
        break;

        case 0xF000:
        {
            switch (opcode & 0x00FF)
            {
                case 0x001E:
                {
                    AddRegisterToIndex();
                }
                break;

                default:
                    printf("Unknown opcode [0xF000]: 0x%.4X\n", opcode);
                    break;
            }
        }
        break;

        default:
            printf("Unknown opcode: 0x%.4X\n", opcode);
            break;
    }
}

void Cpu::JumpToAddress()
{
    pc = opcode & 0x0FFF;
    printf("0x%.4X: Jumped to 0x%.4X\n", opcode, opcode & 0x0FFF);
}

void Cpu::JumpToSubroutine()
{
    stack[sp] = pc;
    sp++;
    pc = opcode & 0x0FFF;
    printf("0x%.4X: Jumped to subroutine at 0x%.4X\n", opcode, opcode & 0x0FFF);
}

void Cpu::SkipIfRegisterEqual()
{
    if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
    {
        pc += 4;
        printf("0x%.4X: Register %i equals 0x%.4X, skipped instruction\n", opcode, (opcode & 0x0F00) >> 8, opcode & 0x00FF);
    }
    else
    {
        pc += 2;
        printf("0x%.4X: Register %i did not equal 0x%.4X, did not skip instruction\n", opcode, (opcode & 0x0F00) >> 8, opcode & 0x00FF);
    }
}

void Cpu::SetRegister()
{
    V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
    pc += 2;
    printf("0x%.4X: Set register %i to 0x%.4X\n", opcode, (opcode & 0x0F00) >> 8, opcode & 0x00FF);
}

void Cpu::AddToRegister()
{
    V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
    pc += 2;
    printf("0x%.4X: Added 0x%.4X to register %i\n", opcode, opcode & 0x00FF, (opcode & 0x0F00) >> 8);
}

void Cpu::AssignRegisterToRegister()
{
    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
    pc += 2;
    printf("0x%.4X: Set register %i to register %i", opcode, (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
}

void Cpu::SetIndex()
{
    I = opcode & 0x0FFF;
    pc += 2;
    printf("0x%.4X: Set I to 0x%.4X\n", opcode, opcode & 0x0FFF);
}

void Cpu::Draw()
{
    unsigned short x = V[(opcode & 0x0F00) >> 8];
    unsigned short y = V[(opcode & 0x00F0) >> 4];
    unsigned short height = opcode & 0x000F;
    unsigned short pixel;

    V[0xF] = 0;
    for (int yline = 0; yline < height; yline++)
    {
        pixel = memory[I + yline];

        for (int xline = 0; xline < 8; xline++)
        {
            if ((pixel & (0x80 >> xline)) != 0)
            {
                if (gfx[(x + xline + ((y + yline) * 64))] == 1)
                {
                    V[0xF] = 1;
                    gfx[x + xline + ((y + yline) * 64)] ^= 1;
                }
            }
        }
    }

    shouldDraw = true;
    pc += 2;
    printf("0x%.4X: Set draw flag\n", opcode);
}

void Cpu::SkipIfKeyUp()
{
    if (!key[V[(opcode & 0x0F00) >> 8]])
    {
        pc += 4;
        printf("0x%.4X: Key %i was up, skipped instruction\n", opcode, (opcode & 0x0F00) >> 8);
    }
    else
    {
        pc += 2;
        printf("0x%.4X: Key %i was down, did not skip instruction\n", opcode, (opcode & 0x0F00) >> 8);
    }
}

void Cpu::AddRegisterToIndex()
{
    I += V[(opcode & 0x0F00) >> 8];
    pc += 2;
    printf("0x%.4X: Added register %i to Index\n", opcode, (opcode & 0x0F00) >> 8);
}