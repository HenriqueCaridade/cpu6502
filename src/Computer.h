
#ifndef CPU6502_COMPUTER_H
#define CPU6502_COMPUTER_H

#include "memory/Memory.h"
#include "cpu/CPU.h"

class Computer {
public:
    Memory memory;
    CPU cpu;

    /// @brief Constructor.
    explicit Computer(word resetVector = 0x1000);

    /// @brief Resets the cpu and the memory to the initial state.
    void reset();

    /// @brief Resets the Program Counter(PC) to the reset vector.
    void resetPC();

    /// @brief Loads Program Given Onto Memory.
    void loadProgram(const byte* bytes, dword noBytes);

    /// @brief Runs Program.
    int run(int cpuCycles);
};


#endif //CPU6502_COMPUTER_H
