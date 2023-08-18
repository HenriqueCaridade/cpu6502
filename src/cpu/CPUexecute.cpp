
#include <iostream>
#include "CPU.h"

int CPU::execute(int cycles, Memory &memory) {
    int cyclesExpected = cycles;
    while (cycles > 0) {
        Instruction instruction = fetchInstruction(cycles, memory);
        switch (instruction) {
            // LOAD INSTRUCTIONS
            case ldaImm: {
                A = fetchByte(cycles, memory);
                setAssignmentFlags(A);
            } break;
            case ldxImm: {
                X = fetchByte(cycles, memory);
                setAssignmentFlags(X);
            } break;
            case ldyImm: {
                Y = fetchByte(cycles, memory);
                setAssignmentFlags(Y);
            } break;
            case ldaZpg: {
                word address = zeroPageAddress(cycles, memory);
                A = readByte(cycles, memory, address);
                setAssignmentFlags(A);
            } break;
            case ldxZpg: {
                word address = zeroPageAddress(cycles, memory);
                X = readByte(cycles, memory, address);
                setAssignmentFlags(X);
            } break;
            case ldyZpg: {
                word address = zeroPageAddress(cycles, memory);
                Y = readByte(cycles, memory, address);
                setAssignmentFlags(Y);
            } break;
            case ldaZpX: {
                word address = zeroPageAddress(cycles, memory, X);
                A = readByte(cycles, memory, address);
                setAssignmentFlags(A);
            } break;
            case ldxZpY: {
                word address = zeroPageAddress(cycles, memory, Y);
                X = readByte(cycles, memory, address);
                setAssignmentFlags(X);
            } break;
            case ldyZpX: {
                word address = zeroPageAddress(cycles, memory, X);
                Y = readByte(cycles, memory, address);
                setAssignmentFlags(Y);
            } break;
            case ldaAbs: {
                word address = absoluteAddress(cycles, memory);
                A = readByte(cycles, memory, address);
                setAssignmentFlags(A);
            } break;
            case ldxAbs: {
                word address = absoluteAddress(cycles, memory);
                X = readByte(cycles, memory, address);
                setAssignmentFlags(X);
            } break;
            case ldyAbs: {
                word address = absoluteAddress(cycles, memory);
                Y = readByte(cycles, memory, address);
                setAssignmentFlags(Y);
            } break;
            case ldaAbX: {
                word address = absoluteAddress(cycles, memory, X);
                A = readByte(cycles, memory, address);
                setAssignmentFlags(A);
            } break;
            case ldaAbY: {
                word address = absoluteAddress(cycles, memory, Y);
                A = readByte(cycles, memory, address);
                setAssignmentFlags(A);
            } break;
            case ldxAbY: {
                word address = absoluteAddress(cycles, memory, Y);
                X = readByte(cycles, memory, address);
                setAssignmentFlags(X);
            } break;
            case ldyAbX: {
                word address = absoluteAddress(cycles, memory, X);
                Y = readByte(cycles, memory, address);
                setAssignmentFlags(Y);
            } break;
            case ldaIdX: {
                word address = indirectPreAddress(cycles, memory, X);
                A = readByte(cycles, memory, address);
                setAssignmentFlags(A);
            } break;
            case ldaIdY: {
                word address = indirectPostAddress(cycles, memory, Y);
                A = readByte(cycles, memory, address);
                setAssignmentFlags(A);
            } break;
            // STORE INSTRUCTIONS
            case staZpg: {
                word address = zeroPageAddress(cycles, memory);
                writeByte(A, cycles, memory, address);
            } break;
            case stxZpg: {
                word address = zeroPageAddress(cycles, memory);
                writeByte(X, cycles, memory, address);
            } break;
            case styZpg: {
                word address = zeroPageAddress(cycles, memory);
                writeByte(Y, cycles, memory, address);
            } break;
            case staZpX: {
                word address = zeroPageAddress(cycles, memory, X);
                writeByte(A, cycles, memory, address);
            } break;
            case stxZpY: {
                word address = zeroPageAddress(cycles, memory, Y);
                writeByte(X, cycles, memory, address);
            } break;
            case styZpX: {
                word address = zeroPageAddress(cycles, memory, X);
                writeByte(Y, cycles, memory, address);
            } break;
            case staAbs: {
                word address = absoluteAddress(cycles, memory);
                writeByte(A, cycles, memory, address);
            } break;
            case stxAbs: {
                word address = absoluteAddress(cycles, memory);
                writeByte(X, cycles, memory, address);
            } break;
            case styAbs: {
                word address = absoluteAddress(cycles, memory);
                writeByte(Y, cycles, memory, address);
            } break;
            case staAbX: {
                word address = absoluteAddressFixed(cycles, memory, X);
                writeByte(A, cycles, memory, address);
            } break;
            case staAbY: {
                word address = absoluteAddressFixed(cycles, memory, Y);
                writeByte(A, cycles, memory, address);
            } break;
            case staIdX: {
                word address = indirectPreAddress(cycles, memory, X);
                writeByte(A, cycles, memory, address);
            } break;
            case staIdY: {
                word address = indirectPostAddressFixed(cycles, memory, Y);
                writeByte(A, cycles, memory, address);
            } break;
            // AND
            case andImm: {
                A &= fetchByte(cycles, memory);
                setAssignmentFlags(A);
            } break;
            case andZpg: {
                word address = zeroPageAddress(cycles, memory);
                A &= readByte(cycles, memory, address);
                setAssignmentFlags(A);
            } break;
            case andZpX: {
                word address = zeroPageAddress(cycles, memory, X);
                A &= readByte(cycles, memory, address);
                setAssignmentFlags(A);
            } break;
            case andAbs: {
                word address = absoluteAddress(cycles, memory);
                A &= readByte(cycles, memory, address);
                setAssignmentFlags(A);
            } break;
            case andAbX: {
                word address = absoluteAddress(cycles, memory, X);
                A &= readByte(cycles, memory, address);
                setAssignmentFlags(A);
            } break;
            case andAbY: {
                word address = absoluteAddress(cycles, memory, Y);
                A &= readByte(cycles, memory, address);
                setAssignmentFlags(A);
            } break;
            case andIdX: {
                word address = indirectPreAddress(cycles, memory, X);
                A &= readByte(cycles, memory, address);
                setAssignmentFlags(A);
            } break;
            case andIdY: {
                word address = indirectPostAddress(cycles, memory, Y);
                A &= readByte(cycles, memory, address);
                setAssignmentFlags(A);
            } break;
            // EOR
            case eorImm: {
                A ^= fetchByte(cycles, memory);
                setAssignmentFlags(A);
            } break;
            case eorZpg: {
                word address = zeroPageAddress(cycles, memory);
                A ^= readByte(cycles, memory, address);
                setAssignmentFlags(A);
            } break;
            case eorZpX: {
                word address = zeroPageAddress(cycles, memory, X);
                A ^= readByte(cycles, memory, address);
                setAssignmentFlags(A);
            } break;
            case eorAbs: {
                word address = absoluteAddress(cycles, memory);
                A ^= readByte(cycles, memory, address);
                setAssignmentFlags(A);
            } break;
            case eorAbX: {
                word address = absoluteAddress(cycles, memory, X);
                A ^= readByte(cycles, memory, address);
                setAssignmentFlags(A);
            } break;
            case eorAbY: {
                word address = absoluteAddress(cycles, memory, Y);
                A ^= readByte(cycles, memory, address);
                setAssignmentFlags(A);
            } break;
            case eorIdX: {
                word address = indirectPreAddress(cycles, memory, X);
                A ^= readByte(cycles, memory, address);
                setAssignmentFlags(A);
            } break;
            case eorIdY: {
                word address = indirectPostAddress(cycles, memory, Y);
                A ^= readByte(cycles, memory, address);
                setAssignmentFlags(A);
            } break;
            // ORA
            case oraImm: {
                A |= fetchByte(cycles, memory);
                setAssignmentFlags(A);
            } break;
            case oraZpg: {
                word address = zeroPageAddress(cycles, memory);
                A |= readByte(cycles, memory, address);
                setAssignmentFlags(A);
            } break;
            case oraZpX: {
                word address = zeroPageAddress(cycles, memory, X);
                A |= readByte(cycles, memory, address);
                setAssignmentFlags(A);
            } break;
            case oraAbs: {
                word address = absoluteAddress(cycles, memory);
                A |= readByte(cycles, memory, address);
                setAssignmentFlags(A);
            } break;
            case oraAbX: {
                word address = absoluteAddress(cycles, memory, X);
                A |= readByte(cycles, memory, address);
                setAssignmentFlags(A);
            } break;
            case oraAbY: {
                word address = absoluteAddress(cycles, memory, Y);
                A |= readByte(cycles, memory, address);
                setAssignmentFlags(A);
            } break;
            case oraIdX: {
                word address = indirectPreAddress(cycles, memory, X);
                A |= readByte(cycles, memory, address);
                setAssignmentFlags(A);
            } break;
            case oraIdY: {
                word address = indirectPostAddress(cycles, memory, Y);
                A |= readByte(cycles, memory, address);
                setAssignmentFlags(A);
            } break;
            // BIT
            case bitZpg: {
                word address = zeroPageAddress(cycles, memory);
                byte value = readByte(cycles, memory, address);
                flag.Z = isZero(A & value);
                flag.N = isNeg(value);
                flag.V = (value & 0x40) == 0x40; // 6th bit
            } break;
            case bitAbs: {
                word address = absoluteAddress(cycles, memory);
                byte value = readByte(cycles, memory, address);
                flag.Z = isZero(A & value);
                flag.N = isNeg(value);
                flag.V = (value & 0x40) == 0x40; // 6th bit
            } break;
            // TRANSFER INSTRUCTIONS
            case taxImp: {
                X = A; cycles--;
                setAssignmentFlags(X);
            } break;
            case txaImp: {
                A = X; cycles--;
                setAssignmentFlags(A);
            } break;
            case tayImp: {
                Y = A; cycles--;
                setAssignmentFlags(Y);
            } break;
            case tyaImp: {
                A = Y; cycles--;
                setAssignmentFlags(A);
            } break;
            case tsxImp: {
                X = SP; cycles--;
                setAssignmentFlags(X);
            } break;
            case txsImp: {
                SP = X; cycles--;
            } break;
            // STACK INSTRUCTIONS
            case phaImp: {
                stackPushByte(A, cycles, memory);
                cycles--;
            } break;
            case phpImp: {
                stackPushByte(status | 0b00110000, cycles, memory);
                cycles--;
            } break;
            case plaImp: {
                A = stackPullByte(cycles, memory);
                cycles -= 2;
            } break;
            case plpImp: {
                status = (status & 0b00110000) | (stackPullByte(cycles, memory) & 0b11001111);
                cycles -= 2;
            } break;
            // INCREMENT INSTRUCTIONS
            case incZpg: {
                word address = zeroPageAddress(cycles, memory);
                byte value = readByte(cycles, memory, address);
                value++; cycles--;
                writeByte(value, cycles, memory, address);
                setAssignmentFlags(value);
            } break;
            case incZpX: {
                word address = zeroPageAddress(cycles, memory, X);
                byte value = readByte(cycles, memory, address);
                value++; cycles--;
                writeByte(value, cycles, memory, address);
                setAssignmentFlags(value);
            } break;
            case incAbs: {
                word address = absoluteAddress(cycles, memory);
                byte value = readByte(cycles, memory, address);
                value++; cycles--;
                writeByte(value, cycles, memory, address);
                setAssignmentFlags(value);
            } break;
            case incAbX: {
                word address = absoluteAddressFixed(cycles, memory, X);
                byte value = readByte(cycles, memory, address);
                value++; cycles--;
                writeByte(value, cycles, memory, address);
                setAssignmentFlags(value);
            } break;
            case inxImp: {
                X++; cycles--;
                setAssignmentFlags(X);
            } break;
            case inyImp: {
                Y++; cycles--;
                setAssignmentFlags(Y);
            } break;
            // DECREMENT INSTRUCTIONS
            case decZpg: {
                word address = zeroPageAddress(cycles, memory);
                byte value = readByte(cycles, memory, address);
                value--; cycles--;
                writeByte(value, cycles, memory, address);
                setAssignmentFlags(value);
            } break;
            case decZpX: {
                word address = zeroPageAddress(cycles, memory, X);
                byte value = readByte(cycles, memory, address);
                value--; cycles--;
                writeByte(value, cycles, memory, address);
                setAssignmentFlags(value);
            } break;
            case decAbs: {
                word address = absoluteAddress(cycles, memory);
                byte value = readByte(cycles, memory, address);
                value--; cycles--;
                writeByte(value, cycles, memory, address);
                setAssignmentFlags(value);
            } break;
            case decAbX: {
                word address = absoluteAddressFixed(cycles, memory, X);
                byte value = readByte(cycles, memory, address);
                value--; cycles--;
                writeByte(value, cycles, memory, address);
                setAssignmentFlags(value);
            } break;
            case dexImp: {
                X--; cycles--;
                setAssignmentFlags(X);
            } break;
            case deyImp: {
                Y--; cycles--;
                setAssignmentFlags(Y);
            } break;
            // JUMP AND CALLS INSTRUCTIONS
            case jmpAbs: {
                word address = absoluteAddress(cycles, memory);
                jumpTo(address);
            } break;
            case jmpInd: {
                word address = indirectAddress(cycles, memory);
                jumpTo(address);
            } break;
            case jsrAbs: {
                word address = absoluteAddress(cycles, memory);
                stackPushWord(PC - 1, cycles, memory);
                jumpTo(address); cycles--;
            } break;
            case rtsImp: {
                word address = stackPullWord(cycles, memory);
                jumpTo(address + 1); cycles -= 3;
            } break;
            case nop: cycles--; break;
            default :
                std::cout << "Unhandled instruction opcode: "
                    << std::hex << instruction << std::endl;
                throw -1;
        }
    }
    return cyclesExpected - cycles;
}
