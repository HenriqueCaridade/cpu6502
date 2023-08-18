
#include "gtest/gtest.h"
#include "../src/Computer.h"

class JumpsAndSubroutinesTests : public ::testing::Test {
public:
    static const byte UNCHANGED_FLAGS = 0b11001111;
    Computer computer;

    void SetUp() override { computer.reset(); }
    void TearDown() override {}

    void VerifyUnchangedFlags(const CPU& copy) const {
        EXPECT_EQ(computer.cpu.status & UNCHANGED_FLAGS, copy.status & UNCHANGED_FLAGS);
    }
};

// ================== //
//       jmpAbs       //
// ================== //

TEST_F(JumpsAndSubroutinesTests, jmpAbs_CanJumpToNewLocation) {
    // Given:
    computer.memory[0x1000] = CPU::jmpAbs;
    computer.memory[0x1001] = 0x10;
    computer.memory[0x1002] = 0x30;
    const int EXPECTED_CYCLES = 3;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.PC, 0x3010);
    EXPECT_EQ(computer.cpu.SP, cpuCopy.SP);
    VerifyUnchangedFlags(cpuCopy);
}

// ================== //
//       jmpInd       //
// ================== //

TEST_F(JumpsAndSubroutinesTests, jmpInd_CanJumpToNewLocation) {
    // Given:
    computer.memory[0x1000] = CPU::jmpInd;
    computer.memory[0x1001] = 0x10;
    computer.memory[0x1002] = 0x30;
    computer.memory[0x3010] = 0x20;
    computer.memory[0x3011] = 0x60;
    const int EXPECTED_CYCLES = 5;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.PC, 0x6020);
    EXPECT_EQ(computer.cpu.SP, cpuCopy.SP);
    VerifyUnchangedFlags(cpuCopy);
}

// ================== //
//       jsrAbs       //
// ================== //

TEST_F(JumpsAndSubroutinesTests, jsrAbs_CanJumpToNewLocationSavingReturnAddress) {
    // Given:
    computer.memory[0x1000] = CPU::jsrAbs;
    computer.memory[0x1001] = 0x10;
    computer.memory[0x1002] = 0x30;
    const int EXPECTED_CYCLES = 6;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.PC, 0x3010);
    EXPECT_EQ(computer.cpu.SP, 0xFD);
    EXPECT_EQ(computer.memory[0x01FE], 0x02);
    EXPECT_EQ(computer.memory[0x01FF], 0x10);
    VerifyUnchangedFlags(cpuCopy);
}

// ================== //
//       rtsImp       //
// ================== //

TEST_F(JumpsAndSubroutinesTests, rtsImp_CanJumpToNewLocationSavingReturnAddress) {
    // Given:
    computer.cpu.SP = 0xFD;
    computer.memory[0x01FE] = 0x10;
    computer.memory[0x01FF] = 0x30;
    computer.memory[0x1000] = CPU::rtsImp;
    const int EXPECTED_CYCLES = 6;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.PC, 0x3011);
    EXPECT_EQ(computer.cpu.SP, 0xFF);
    VerifyUnchangedFlags(cpuCopy);
}

// ================== //
//  jsrAbs && rtsImp  //
// ================== //

TEST_F(JumpsAndSubroutinesTests, jsrAbs_rtsImp_CanJumpToNewLocationAndJumpBack) {
    // Given:
    computer.memory[0x1000] = CPU::jsrAbs;
    computer.memory[0x1001] = 0x10;
    computer.memory[0x1002] = 0x30;
    computer.memory[0x3010] = CPU::rtsImp;
    const int EXPECTED_CYCLES = 6 + 6;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.PC, 0x1003);
    EXPECT_EQ(computer.cpu.SP, 0xFF);
    VerifyUnchangedFlags(cpuCopy);
}
