
#include "CPU.h"

CPU::CPU() {
    reset();
}

void CPU::reset(word resetVector) {
    PC = resetVector;
    SP = 0xFF;
    A = X = Y = 0x00;
    status = 0x00;
}

void CPU::setLoadFlags(byte reg) {
    flag.Z = (reg == 0);
    flag.N = (reg & 0x80) == 0x80;
}

CPU::Instruction CPU::fetchInstruction(int& cycles, const Memory &memory) {
    cycles--;
    return (Instruction) memory[PC++];
}

byte CPU::fetchByte(int& cycles, const Memory &memory) {
    cycles--;
    return memory[PC++];
}

word CPU::fetchWord(int &cycles, const Memory &memory) {
    word data = memory.readWord(PC);
    cycles -= 2;
    PC += 2;
    return data;
}

byte CPU::readByte(int &cycles, const Memory &memory, word address) {
    cycles--;
    return memory[address];
}

word CPU::readWord(int &cycles, const Memory &memory, word address) {
    cycles -= 2;
    return memory.readWord(address);
}

void CPU::writeByte(byte value, int &cycles, Memory &memory, word address) {
    memory[address] = value;
    cycles--;
}

void CPU::writeWord(word value, int &cycles, Memory &memory, word address) {
    memory.writeWord(value, address);
    cycles -= 2;
}

void CPU::stackPushByte(byte value, int &cycles, Memory &memory) {
    memory[_SPaddress] = value;
    cycles--; SP--;
}

void CPU::stackPushWord(word value, int &cycles, Memory &memory) {
    memory.writeWord(value, _SPaddress - 1);
    cycles -= 2; SP -= 2;
}

byte CPU::stackPullByte(int &cycles, Memory &memory) {
    cycles--; SP++;
    return memory[_SPaddress];
}

word CPU::stackPullWord(int &cycles, Memory &memory) {
    cycles -= 2; SP += 2;
    return memory.readWord(_SPaddress - 1);
}

void CPU::jumpTo(word address) { PC = address; }

word CPU::zeroPageAddress(int &cycles, const Memory &memory) {
    return fetchByte(cycles, memory);
}

word CPU::zeroPageAddress(int &cycles, const Memory &memory, byte offset) {
    cycles--;
    return (byte) (fetchByte(cycles, memory) + offset);
}

word CPU::absoluteAddress(int &cycles, const Memory &memory) {
    return fetchWord(cycles, memory);
}

word CPU::absoluteAddress(int &cycles, const Memory &memory, byte offset) {
    word data = fetchWord(cycles, memory);
    word final = data + offset;
    // Page Boundary Crossing
    if ((final & 0x0100) != (data & 0x0100)) cycles--;
    return final;
}

word CPU::absoluteAddressS(int &cycles, const Memory &memory, byte offset) {
    cycles--;
    return fetchWord(cycles, memory) + offset;
}

word CPU::indirectAddress(int &cycles, const Memory &memory) {
    return readWord(cycles, memory, fetchWord(cycles, memory));
}

word CPU::indirectPreAddress(int &cycles, const Memory &memory, byte offset) {
    cycles--;
    return readWord(cycles, memory, (byte) (fetchByte(cycles, memory) + offset));
}

word CPU::indirectPostAddress(int &cycles, const Memory &memory, byte offset) {
    word data = readWord(cycles, memory, fetchByte(cycles,memory));
    word final = data + offset;
    // Page Boundary Crossing
    if ((final & 0x0100) != (data & 0x0100)) cycles--;
    return final;
}

word CPU::indirectPostAddressS(int &cycles, const Memory &memory, byte offset) {
    cycles--;
    return readWord(cycles, memory, fetchByte(cycles,memory)) + offset;
}
