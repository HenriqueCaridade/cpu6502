
#include "Computer.h"

Computer::Computer() = default;

void Computer::reset() {
    memory.clear();
    cpu.reset();
}

void Computer::reset(word resetVector) {
    memory.clear();
    cpu.reset(resetVector);
}

int Computer::run(int cpuCycles) {
    return cpu.execute(cpuCycles, memory);
}
