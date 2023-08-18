
#ifndef CPU6502_COMPUTER_H
#define CPU6502_COMPUTER_H

#include <iostream>
#include "memory/Memory.h"
#include "cpu/CPU.h"

class Computer {
public:
    Memory memory;
    CPU cpu;

    Computer();
    void reset();
    void reset(word resetVector);
    int run(int cpuCycles);
};


#endif //CPU6502_COMPUTER_H
