
#ifndef CPU6502_MEMORY_H
#define CPU6502_MEMORY_H

#include "../types.h"

class Memory {
    static constexpr dword MAX_MEM = 1024 * 64;
    byte data[MAX_MEM];
public:
    /// Default constructor initializes data to all Zeros
    Memory();

    /// Read byte
    byte operator[](word address) const;

    /// Write byte
    byte& operator[](word address);

    /// Read word (little endian)
    word readWord(word address) const;

    /// Write word (little endian)
    void writeWord(word value, word address);

    /// Zeros all data
    void clear();

    friend class Computer;
    friend class CPU;
};


#endif //CPU6502_MEMORY_H
