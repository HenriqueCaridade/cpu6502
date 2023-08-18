#include "gtest/gtest.h"
#include "../src/Computer.h"

class StackInstructionsTests : public ::testing::Test {
public:
    static const byte UNCHANGED_FLAGS = 0b11001111;
    static const byte UNCHANGED_ON_PULL_STATUS = 0b00110000;
    Computer computer;

    void SetUp() override { computer.reset(); }
    void TearDown() override {}

    void VerifyUnchangedFlags(const CPU& copy) const {
        EXPECT_EQ(computer.cpu.status & UNCHANGED_FLAGS, copy.status & UNCHANGED_FLAGS);
    }

    void VerifyChangedFlags(byte rightFlags) const {
        EXPECT_EQ(computer.cpu.status & UNCHANGED_FLAGS, rightFlags & UNCHANGED_FLAGS);
    }

    void VerifyUnchangedOnPullStatusFlags(const CPU& copy) const {
        EXPECT_EQ(computer.cpu.status & UNCHANGED_ON_PULL_STATUS, copy.status & UNCHANGED_ON_PULL_STATUS);
    }
};

// ================== //
//       phaImp       //
// ================== //

TEST_F(StackInstructionsTests, phaImp_CanPushARegisterToStack) {
    // Given:
    computer.cpu.A = 0x35;
    computer.memory[0xFFFC] = CPU::phaImp;
    const int EXPECTED_CYCLES = 3;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.memory[0x1FF], 0x35);
    EXPECT_EQ(computer.cpu.SP, 0xFE);
    VerifyUnchangedFlags(cpuCopy);
}

// ================== //
//       phpImp       //
// ================== //

TEST_F(StackInstructionsTests, phpImp_CanPushProcessorStatusToStack) {
    // Given:
    computer.cpu.status = 0b11111011;
    computer.memory[0xFFFC] = CPU::phpImp;
    const int EXPECTED_CYCLES = 3;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.memory[0x1FF], 0b11111011);
    EXPECT_EQ(computer.cpu.SP, 0xFE);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(StackInstructionsTests, phpImp_CanPushProcessorStatusToStack_Set5thBit) {
    // Given:
    computer.cpu.status = 0b11011011;
    computer.memory[0xFFFC] = CPU::phpImp;
    const int EXPECTED_CYCLES = 3;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.memory[0x1FF], 0b11111011);
    EXPECT_EQ(computer.cpu.SP, 0xFE);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(StackInstructionsTests, phpImp_CanPushProcessorStatusToStack_SetBreakBit) {
    // Given:
    computer.cpu.status = 0b11101011;
    computer.memory[0xFFFC] = CPU::phpImp;
    const int EXPECTED_CYCLES = 3;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.memory[0x1FF], 0b11111011);
    EXPECT_EQ(computer.cpu.SP, 0xFE);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(StackInstructionsTests, phpImp_CanPushProcessorStatusToStack_Set5thAndBreakBit) {
    // Given:
    computer.cpu.status = 0b11001011;
    computer.memory[0xFFFC] = CPU::phpImp;
    const int EXPECTED_CYCLES = 3;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.memory[0x1FF], 0b11111011);
    EXPECT_EQ(computer.cpu.SP, 0xFE);
    VerifyUnchangedFlags(cpuCopy);
}

// ================== //
//       plaImp       //
// ================== //

TEST_F(StackInstructionsTests, plaImp_CanPullARegisterFromStack) {
    // Given:
    computer.cpu.SP = 0xFE;
    computer.memory[0x01FF] = 0x35;
    computer.memory[0xFFFC] = CPU::plaImp;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.SP, 0xFF);
    EXPECT_EQ(computer.cpu.A, 0x35);
    VerifyUnchangedFlags(cpuCopy);
}

// ================== //
//       plpImp       //
// ================== //

TEST_F(StackInstructionsTests, plpImp_CanPullProcessorStatusFromStack) {
    // Given:
    computer.cpu.status = 0b00110000;
    computer.cpu.SP = 0xFE;
    computer.memory[0x01FF] = 0b11111111;
    computer.memory[0xFFFC] = CPU::plpImp;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.SP, 0xFF);
    VerifyChangedFlags(0b11001111);
    VerifyUnchangedOnPullStatusFlags(cpuCopy);
}

TEST_F(StackInstructionsTests, plpImp_CanPullProcessorStatusFromStack_Maintains5thBit) {
    // Given:
    computer.cpu.status = 0b00100000;
    computer.cpu.SP = 0xFE;
    computer.memory[0x01FF] = 0b11111111;
    computer.memory[0xFFFC] = CPU::plpImp;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.SP, 0xFF);
    VerifyChangedFlags(0b11001111);
    VerifyUnchangedOnPullStatusFlags(cpuCopy);
}

TEST_F(StackInstructionsTests, plpImp_CanPullProcessorStatusFromStack_MaintainsBreakBit) {
    // Given:
    computer.cpu.status = 0b00010000;
    computer.cpu.SP = 0xFE;
    computer.memory[0x01FF] = 0b11111111;
    computer.memory[0xFFFC] = CPU::plpImp;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.SP, 0xFF);
    VerifyChangedFlags(0b11001111);
    VerifyUnchangedOnPullStatusFlags(cpuCopy);
}

TEST_F(StackInstructionsTests, plpImp_CanPullProcessorStatusFromStack_Maintains5thAndBreakBit) {
    // Given:
    computer.cpu.status = 0b00110000;
    computer.cpu.SP = 0xFE;
    computer.memory[0x01FF] = 0b11111111;
    computer.memory[0xFFFC] = CPU::plpImp;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.SP, 0xFF);
    VerifyChangedFlags(0b11001111);
    VerifyUnchangedOnPullStatusFlags(cpuCopy);
}
