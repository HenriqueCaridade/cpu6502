
#include "gtest/gtest.h"
#include "../src/Computer.h"

class BitInstructionTests : public ::testing::Test {
public:
    static const byte UNCHANGED_FLAGS = 0b00001101;
    Computer computer;

    void SetUp() override { computer.reset(); }
    void TearDown() override {}

    void VerifyUnchangedFlags(const CPU& copy) const {
        EXPECT_EQ(computer.cpu.status & UNCHANGED_FLAGS, copy.status & UNCHANGED_FLAGS);
    }
};

// ================== //
//       bitZpg       //
// ================== //

TEST_F(BitInstructionTests, bitZpg_CanCheckIfAnyBitInMemoryIsSet) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.V = true;
    computer.cpu.flag.Z = true;
    computer.cpu.A = 0xFF;
    computer.memory[0x1000] = CPU::bitZpg;
    computer.memory[0x1001] = 0x80;
    computer.memory[0x0080] = 0x35;
    const int EXPECTED_CYCLES = 3;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0xFF);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.V);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(BitInstructionTests, bitZpg_CanCheckIfAnyBitInMemoryIsSet_SetNegativeFlag) {
    // Given:
    computer.cpu.flag.N = false;
    computer.cpu.flag.V = true;
    computer.cpu.flag.Z = true;
    computer.cpu.A = 0xFF;
    computer.memory[0x1000] = CPU::bitZpg;
    computer.memory[0x1001] = 0x80;
    computer.memory[0x0080] = 0x95;
    const int EXPECTED_CYCLES = 3;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0xFF);
    EXPECT_TRUE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.V);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(BitInstructionTests, bitZpg_CanCheckIfAnyBitInMemoryIsSet_SetOverflowFlag) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.V = false;
    computer.cpu.flag.Z = true;
    computer.cpu.A = 0xFF;
    computer.memory[0x1000] = CPU::bitZpg;
    computer.memory[0x1001] = 0x80;
    computer.memory[0x0080] = 0x55;
    const int EXPECTED_CYCLES = 3;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0xFF);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_TRUE(computer.cpu.flag.V);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(BitInstructionTests, bitZpg_CanCheckIfAnyBitInMemoryIsSet_SetNegativeAndOverflowFlags) {
    // Given:
    computer.cpu.flag.N = false;
    computer.cpu.flag.V = false;
    computer.cpu.flag.Z = true;
    computer.cpu.A = 0xFF;
    computer.memory[0x1000] = CPU::bitZpg;
    computer.memory[0x1001] = 0x80;
    computer.memory[0x0080] = 0xD5;
    const int EXPECTED_CYCLES = 3;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0xFF);
    EXPECT_TRUE(computer.cpu.flag.N);
    EXPECT_TRUE(computer.cpu.flag.V);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(BitInstructionTests, bitZpg_CanCheckIfAnyBitInMemoryIsSet_WithMask) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.V = true;
    computer.cpu.flag.Z = false;
    computer.cpu.A = 0x08;
    computer.memory[0x1000] = CPU::bitZpg;
    computer.memory[0x1001] = 0x80;
    computer.memory[0x0080] = 0x35;
    const int EXPECTED_CYCLES = 3;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x08);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.V);
    EXPECT_TRUE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(BitInstructionTests, bitZpg_CanCheckIfAnyBitInMemoryIsSet_WithMask_SetNegativeFlag) {
    // Given:
    computer.cpu.flag.N = false;
    computer.cpu.flag.V = true;
    computer.cpu.flag.Z = false;
    computer.cpu.A = 0x08;
    computer.memory[0x1000] = CPU::bitZpg;
    computer.memory[0x1001] = 0x80;
    computer.memory[0x0080] = 0x95;
    const int EXPECTED_CYCLES = 3;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x08);
    EXPECT_TRUE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.V);
    EXPECT_TRUE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(BitInstructionTests, bitZpg_CanCheckIfAnyBitInMemoryIsSet_WithMask_SetOverflowFlag) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.V = false;
    computer.cpu.flag.Z = false;
    computer.cpu.A = 0x08;
    computer.memory[0x1000] = CPU::bitZpg;
    computer.memory[0x1001] = 0x80;
    computer.memory[0x0080] = 0x55;
    const int EXPECTED_CYCLES = 3;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x08);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_TRUE(computer.cpu.flag.V);
    EXPECT_TRUE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(BitInstructionTests, bitZpg_CanCheckIfAnyBitInMemoryIsSet_WithMask_SetNegativeAndOverflowFlags) {
    // Given:
    computer.cpu.flag.N = false;
    computer.cpu.flag.V = false;
    computer.cpu.flag.Z = false;
    computer.cpu.A = 0x08;
    computer.memory[0x1000] = CPU::bitZpg;
    computer.memory[0x1001] = 0x80;
    computer.memory[0x0080] = 0xD5;
    const int EXPECTED_CYCLES = 3;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x08);
    EXPECT_TRUE(computer.cpu.flag.N);
    EXPECT_TRUE(computer.cpu.flag.V);
    EXPECT_TRUE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

// ================== //
//       bitAbs       //
// ================== //

TEST_F(BitInstructionTests, bitAbs_CanCheckIfAnyBitInMemoryIsSet) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.V = true;
    computer.cpu.flag.Z = true;
    computer.cpu.A = 0xFF;
    computer.memory[0x1000] = CPU::bitAbs;
    computer.memory[0x1001] = 0x10;
    computer.memory[0x1002] = 0x30;
    computer.memory[0x3010] = 0x35;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0xFF);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.V);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(BitInstructionTests, bitAbs_CanCheckIfAnyBitInMemoryIsSet_SetNegativeFlag) {
    // Given:
    computer.cpu.flag.N = false;
    computer.cpu.flag.V = true;
    computer.cpu.flag.Z = true;
    computer.cpu.A = 0xFF;
    computer.memory[0x1000] = CPU::bitAbs;
    computer.memory[0x1001] = 0x10;
    computer.memory[0x1002] = 0x30;
    computer.memory[0x3010] = 0x95;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0xFF);
    EXPECT_TRUE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.V);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(BitInstructionTests, bitAbs_CanCheckIfAnyBitInMemoryIsSet_SetOverflowFlag) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.V = false;
    computer.cpu.flag.Z = true;
    computer.cpu.A = 0xFF;
    computer.memory[0x1000] = CPU::bitAbs;
    computer.memory[0x1001] = 0x10;
    computer.memory[0x1002] = 0x30;
    computer.memory[0x3010] = 0x55;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0xFF);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_TRUE(computer.cpu.flag.V);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(BitInstructionTests, bitAbs_CanCheckIfAnyBitInMemoryIsSet_SetNegativeAndOverflowFlags) {
    // Given:
    computer.cpu.flag.N = false;
    computer.cpu.flag.V = false;
    computer.cpu.flag.Z = true;
    computer.cpu.A = 0xFF;
    computer.memory[0x1000] = CPU::bitAbs;
    computer.memory[0x1001] = 0x10;
    computer.memory[0x1002] = 0x30;
    computer.memory[0x3010] = 0xD5;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0xFF);
    EXPECT_TRUE(computer.cpu.flag.N);
    EXPECT_TRUE(computer.cpu.flag.V);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(BitInstructionTests, bitAbs_CanCheckIfAnyBitInMemoryIsSet_WithMask) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.V = true;
    computer.cpu.flag.Z = false;
    computer.cpu.A = 0x08;
    computer.memory[0x1000] = CPU::bitAbs;
    computer.memory[0x1001] = 0x10;
    computer.memory[0x1002] = 0x30;
    computer.memory[0x3010] = 0x35;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x08);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.V);
    EXPECT_TRUE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(BitInstructionTests, bitAbs_CanCheckIfAnyBitInMemoryIsSet_WithMask_SetNegativeFlag) {
    // Given:
    computer.cpu.flag.N = false;
    computer.cpu.flag.V = true;
    computer.cpu.flag.Z = false;
    computer.cpu.A = 0x08;
    computer.memory[0x1000] = CPU::bitAbs;
    computer.memory[0x1001] = 0x10;
    computer.memory[0x1002] = 0x30;
    computer.memory[0x3010] = 0x95;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x08);
    EXPECT_TRUE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.V);
    EXPECT_TRUE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(BitInstructionTests, bitAbs_CanCheckIfAnyBitInMemoryIsSet_WithMask_SetOverflowFlag) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.V = false;
    computer.cpu.flag.Z = false;
    computer.cpu.A = 0x08;
    computer.memory[0x1000] = CPU::bitAbs;
    computer.memory[0x1001] = 0x10;
    computer.memory[0x1002] = 0x30;
    computer.memory[0x3010] = 0x55;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x08);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_TRUE(computer.cpu.flag.V);
    EXPECT_TRUE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(BitInstructionTests, bitAbs_CanCheckIfAnyBitInMemoryIsSet_WithMask_SetNegativeAndOverflowFlags) {
    // Given:
    computer.cpu.flag.N = false;
    computer.cpu.flag.V = false;
    computer.cpu.flag.Z = false;
    computer.cpu.A = 0x08;
    computer.memory[0x1000] = CPU::bitAbs;
    computer.memory[0x1001] = 0x10;
    computer.memory[0x1002] = 0x30;
    computer.memory[0x3010] = 0xD5;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x08);
    EXPECT_TRUE(computer.cpu.flag.N);
    EXPECT_TRUE(computer.cpu.flag.V);
    EXPECT_TRUE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

// ==================================================== //

class NopInstructionTests : public ::testing::Test {
public:
    Computer computer;

    void SetUp() override { computer.reset(); }
    void TearDown() override {}
};

// ================= //
//        nop        //
// ================= //

TEST_F(NopInstructionTests, nop_DoesntChangeAnythingButDoesConsumeCycles) {
    // Given:
    computer.memory[0x1000] = CPU::nop;
    const int EXPECTED_CYCLES = 2;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, cpuCopy.A);
    EXPECT_EQ(computer.cpu.X, cpuCopy.X);
    EXPECT_EQ(computer.cpu.Y, cpuCopy.Y);
    EXPECT_EQ(computer.cpu.SP, cpuCopy.SP);
    EXPECT_EQ(computer.cpu.status, cpuCopy.status);
}

TEST_F(NopInstructionTests, nop_RepeatedNopInstructionsYieldNoChanges) {
    // Given:
    const int nopAmount = 16;
    for (int i = 0; i < nopAmount; i++) {
        computer.memory[0x1000 + i] = CPU::nop;
    }
    const int EXPECTED_CYCLES = 2 * nopAmount;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, cpuCopy.A);
    EXPECT_EQ(computer.cpu.X, cpuCopy.X);
    EXPECT_EQ(computer.cpu.Y, cpuCopy.Y);
    EXPECT_EQ(computer.cpu.SP, cpuCopy.SP);
    EXPECT_EQ(computer.cpu.status, cpuCopy.status);
}
