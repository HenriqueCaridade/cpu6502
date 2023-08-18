
#include "Computer.h"

Computer::Computer(word resetVector) : memory() {
    memory.writeWord(resetVector, CPU::RESET_ADRESS);
    this->cpu = CPU(memory);
}

void Computer::reset() {
    memory.clear();
    cpu.reset(memory);
}

void Computer::resetPC() {
    cpu.resetPC(memory);
}

int Computer::run(int cpuCycles) {
    return cpu.execute(cpuCycles, memory);
}

void Computer::loadProgram(const byte *bytes, dword noBytes) {
    if (bytes == nullptr && noBytes < 2) return;
    const word resetVector = bytes[0] + ((word) bytes[1] << 8);
    memory[0xFFFC] = bytes[0]; memory[0xFFFD] = bytes[1];
    for (int i = 2; i < noBytes; i++) {
        memory[resetVector + (i - 2)] = bytes[i];
    }
}
