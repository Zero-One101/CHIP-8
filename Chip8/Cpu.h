#pragma once
#include <string>

class Cpu
{
public:
    Cpu();
    ~Cpu();

    // Initialise registers and memory to zero
    void Init();

    // Load the program in to memory
    bool LoadGame(std::string const &gameName);

    // Fetch, decode and execute opcode
    void AdvanceCycle();

private:
    void DecodeOpcode();

    static const int memorySize = 0x1000;
    static const int displaySize = 64 * 32;
    static const int stackSize = 16;
    static const int registerCount = 16;

    // The current opcode being read by the CPU
    unsigned short opcode;

    /* The RAM the CPU has access to
    0x000-0x1FF - CHIP-8 interpreter
    0x050-0x0A0 - Used for the built in 4x5 font (0-F)
    0x200-0xFFF - Program ROM and work RAM */
    unsigned char memory[memorySize];

    /* 16 8-bit registers.
    V0-VE are general purpose
    VF is a carry flag*/
    unsigned char V[16];

    // Index register
    unsigned short I;

    // Program counter
    unsigned short pc;

    /* The graphics of the CHIP-8
    Can be presented as either 1 or 0 per pixel*/
    unsigned char gfx[displaySize];

    /* Timers that count at 60Hz. Will count down to zero when set above zero
    When the sound timer is not zero, a buzzing sound will be made*/
    unsigned char delayTimer;
    unsigned char soundTimer;

    /* Stack and stack pointer for keeping track of jumps*/
    unsigned short stack[stackSize];
    unsigned short sp;

    // Hex-based keypad
    unsigned char key[16];

    /* A 4x5 pixel font with characters
    0-9 and A-F*/
    unsigned char fontset[80] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80, // F
    };
};