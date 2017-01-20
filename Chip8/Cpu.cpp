#include "Cpu.h"

Cpu::Cpu()
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

Cpu::~Cpu()
{

}

void Cpu::Init()
{

}

bool Cpu::LoadGame(std::string const &gameName)
{
    return true;
}

void Cpu::AdvanceCycle()
{

}

void Cpu::DecodeOpcode()
{

}