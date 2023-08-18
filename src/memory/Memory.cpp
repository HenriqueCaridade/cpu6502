
#include <cstring>
#include "Memory.h"

Memory::Memory() {
    clear();
}

byte Memory::operator[](word address) const { return data[address]; }

byte &Memory::operator[](word address) { return data[address]; }

word Memory::readWord(word address) const {
    // assert(address < MAX_MEM - 1);
    word loByte = data[address];
    word hiByte = data[address + 1];
    return loByte | (hiByte << 8);
}

void Memory::writeWord(word value, word address) {
    // assert(address < MAX_MEM - 1);
    data[address] = (byte) value; // Low byte
    data[address + 1] = value >> 8; // High byte
}

void Memory::clear() {
    memset(data, 0, MAX_MEM);
}



