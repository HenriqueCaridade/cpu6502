
#include "gtest/gtest.h"
#include "../src/Computer.h"

class TransferTests : public ::testing::Test {
public:
    static const byte UNCHANGED_FLAGS = 0b01001101;
    Computer computer;

    void SetUp() override { computer.reset(); }
    void TearDown() override {}

    void VerifyUnchangedFlags(const CPU& copy) const {
        EXPECT_EQ(computer.cpu.status & UNCHANGED_FLAGS, copy.status & UNCHANGED_FLAGS);
    }

    void CanTransferFromAToB(CPU::Instruction instruction, byte CPU::* a, byte CPU::* b) {
        // Given:
        computer.cpu.flag.N = true;
        computer.cpu.flag.Z = true;
        computer.cpu.*a = 0x35;
        computer.memory[0xFFFC] = instruction;
        const int EXPECTED_CYCLES = 2;
        const CPU cpuCopy = computer.cpu;

        // When:
        int cyclesExecuted = computer.run(EXPECTED_CYCLES);

        // Then:
        EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
        EXPECT_EQ(computer.cpu.*b, 0x35);
        EXPECT_FALSE(computer.cpu.flag.N);
        EXPECT_FALSE(computer.cpu.flag.Z);
        VerifyUnchangedFlags(cpuCopy);
    }

    void CanTransferFromAToB_SetNegativeFlag(CPU::Instruction instruction, byte CPU::* a, byte CPU::* b) {
        // Given:
        computer.cpu.flag.N = false;
        computer.cpu.flag.Z = true;
        computer.cpu.*a = 0x95;
        computer.memory[0xFFFC] = instruction;
        const int EXPECTED_CYCLES = 2;
        const CPU cpuCopy = computer.cpu;

        // When:
        int cyclesExecuted = computer.run(EXPECTED_CYCLES);

        // Then:
        EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
        EXPECT_EQ(computer.cpu.*b, 0x95);
        EXPECT_TRUE(computer.cpu.flag.N);
        EXPECT_FALSE(computer.cpu.flag.Z);
        VerifyUnchangedFlags(cpuCopy);
    }

    void CanTransferFromAToB_SetZeroFlag(CPU::Instruction instruction, byte CPU::* a, byte CPU::* b) {
        // Given:
        computer.cpu.flag.N = true;
        computer.cpu.flag.Z = false;
        computer.cpu.*a = 0x00;
        computer.memory[0xFFFC] = instruction;
        const int EXPECTED_CYCLES = 2;
        const CPU cpuCopy = computer.cpu;

        // When:
        int cyclesExecuted = computer.run(EXPECTED_CYCLES);

        // Then:
        EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
        EXPECT_EQ(computer.cpu.*b, 0x00);
        EXPECT_FALSE(computer.cpu.flag.N);
        EXPECT_TRUE(computer.cpu.flag.Z);
        VerifyUnchangedFlags(cpuCopy);
    }
};

// ================== //
//       taxImp       //
// ================== //

TEST_F(TransferTests, taxImp_CanTransferARegisterToXRegister) {
    CanTransferFromAToB(CPU::taxImp, &CPU::A, &CPU::X);
}

TEST_F(TransferTests, taxImp_CanTransferARegisterToXRegister_SetNegativeFlag) {
    CanTransferFromAToB_SetNegativeFlag(CPU::taxImp, &CPU::A, &CPU::X);
}

TEST_F(TransferTests, taxImp_CanTransferARegisterToXRegister_SetZeroFlag) {
    CanTransferFromAToB_SetZeroFlag(CPU::taxImp, &CPU::A, &CPU::X);
}

// ================== //
//       txaImp       //
// ================== //

TEST_F(TransferTests, txaImp_CanTransferXRegisterToARegister) {
    CanTransferFromAToB(CPU::txaImp, &CPU::X, &CPU::A);
}

TEST_F(TransferTests, txaImp_CanTransferXRegisterToARegister_SetNegativeFlag) {
    CanTransferFromAToB_SetNegativeFlag(CPU::txaImp, &CPU::X, &CPU::A);
}

TEST_F(TransferTests, txaImp_CanTransferXRegisterToARegister_SetZeroFlag) {
    CanTransferFromAToB_SetZeroFlag(CPU::txaImp, &CPU::X, &CPU::A);
}

// ================== //
//       tayImp       //
// ================== //

TEST_F(TransferTests, tayImp_CanTransferARegisterToYRegister) {
    CanTransferFromAToB(CPU::tayImp, &CPU::A, &CPU::Y);
}

TEST_F(TransferTests, tayImp_CanTransferARegisterToYRegister_SetNegativeFlag) {
    CanTransferFromAToB_SetNegativeFlag(CPU::tayImp, &CPU::A, &CPU::Y);
}

TEST_F(TransferTests, tayImp_CanTransferARegisterToYRegister_SetZeroFlag) {
    CanTransferFromAToB_SetZeroFlag(CPU::tayImp, &CPU::A, &CPU::Y);
}

// ================== //
//       tyaImp       //
// ================== //

TEST_F(TransferTests, tyaImp_CanTransferYRegisterToARegister) {
    CanTransferFromAToB(CPU::tyaImp, &CPU::Y, &CPU::A);
}

TEST_F(TransferTests, tyaImp_CanTransferYRegisterToARegister_SetNegativeFlag) {
    CanTransferFromAToB_SetNegativeFlag(CPU::tyaImp, &CPU::Y, &CPU::A);
}

TEST_F(TransferTests, tyaImp_CanTransferYRegisterToARegister_SetZeroFlag) {
    CanTransferFromAToB_SetZeroFlag(CPU::tyaImp, &CPU::Y, &CPU::A);
}

// ================== //
//       tsxImp       //
// ================== //

TEST_F(TransferTests, tsxImp_CanTransferStackPointerToXRegister) {
    CanTransferFromAToB(CPU::tsxImp, &CPU::SP, &CPU::X);
}

TEST_F(TransferTests, tsxImp_CanTransferStackPointerToXRegister_SetNegativeFlag) {
    CanTransferFromAToB_SetNegativeFlag(CPU::tsxImp, &CPU::SP, &CPU::X);
}

TEST_F(TransferTests, tsxImp_CanTransferStackPointerToXRegister_SetZeroFlag) {
    CanTransferFromAToB_SetZeroFlag(CPU::tsxImp, &CPU::SP, &CPU::X);
}

// ================== //
//       txsImp       //
// ================== //

TEST_F(TransferTests, txsImp_CanTransferXRegisterToStackPointer) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.cpu.X = 0x35;
    computer.memory[0xFFFC] = CPU::txsImp;
    const int EXPECTED_CYCLES = 2;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.SP, 0x35);
    EXPECT_EQ(computer.cpu.status, cpuCopy.status);
}
