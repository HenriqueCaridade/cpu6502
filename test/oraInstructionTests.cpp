
#include "gtest/gtest.h"
#include "../src/Computer.h"

class OraInstructionTests : public ::testing::Test {
public:
    static const byte UNCHANGED_FLAGS = 0b01001101;
    Computer computer;

    void SetUp() override { computer.reset(); }
    void TearDown() override {}

    void VerifyUnchangedFlags(const CPU& copy) const {
        EXPECT_EQ(computer.cpu.status & UNCHANGED_FLAGS, copy.status & UNCHANGED_FLAGS);
    }
};

// ================== //
//       oraImm       //
// ================== //

TEST_F(OraInstructionTests, oraImm_CanAndWithImmediate) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.cpu.A = 0x00;
    computer.memory[0x1000] = CPU::oraImm;
    computer.memory[0x1001] = 0x35;
    const int EXPECTED_CYCLES = 2;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x35);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(OraInstructionTests, oraImm_CanAndWithImmediate_SetNegativeFlag) {
    // Given:
    computer.cpu.flag.N = false;
    computer.cpu.flag.Z = true;
    computer.cpu.A = 0x00;
    computer.memory[0x1000] = CPU::oraImm;
    computer.memory[0x1001] = 0x95;
    const int EXPECTED_CYCLES = 2;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x95);
    EXPECT_TRUE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(OraInstructionTests, oraImm_CanAndWithImmediate_SetZeroFlag) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = false;
    computer.cpu.A = 0x00;
    computer.memory[0x1000] = CPU::oraImm;
    computer.memory[0x1001] = 0x00;
    const int EXPECTED_CYCLES = 2;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x00);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_TRUE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

// ================== //
//       oraZpg       //
// ================== //

TEST_F(OraInstructionTests, oraZpg_CanAndFromZeroPage) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.cpu.A = 0x00;
    computer.memory[0x1000] = CPU::oraZpg;
    computer.memory[0x1001] = 0x80;
    computer.memory[0x0080] = 0x35;
    const int EXPECTED_CYCLES = 3;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x35);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(OraInstructionTests, oraZpg_CanAndFromZeroPage_SetNegativeFlag) {
    // Given:
    computer.cpu.flag.N = false;
    computer.cpu.flag.Z = true;
    computer.cpu.A = 0x00;
    computer.memory[0x1000] = CPU::oraZpg;
    computer.memory[0x1001] = 0x80;
    computer.memory[0x0080] = 0x95;
    const int EXPECTED_CYCLES = 3;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x95);
    EXPECT_TRUE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(OraInstructionTests, oraZpg_CanAndFromZeroPage_SetZeroFlag) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = false;
    computer.cpu.A = 0x00;
    computer.memory[0x1000] = CPU::oraZpg;
    computer.memory[0x1001] = 0x80;
    computer.memory[0x0080] = 0x00;
    const int EXPECTED_CYCLES = 3;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x00);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_TRUE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

// ================== //
//       oraZpX       //
// ================== //

TEST_F(OraInstructionTests, oraZpX_CanAndFromZeroPageWithXOffset) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.cpu.A = 0x00;
    computer.cpu.X = 0x02;
    computer.memory[0x1000] = CPU::oraZpX;
    computer.memory[0x1001] = 0x80;
    computer.memory[0x0082] = 0x35;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x35);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(OraInstructionTests, oraZpX_CanAndFromZeroPageWithXOffset_SetNegativeFlag) {
    // Given:
    computer.cpu.flag.N = false;
    computer.cpu.flag.Z = true;
    computer.cpu.A = 0x00;
    computer.cpu.X = 0x02;
    computer.memory[0x1000] = CPU::oraZpX;
    computer.memory[0x1001] = 0x80;
    computer.memory[0x0082] = 0x95;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x95);
    EXPECT_TRUE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(OraInstructionTests, oraZpX_CanAndFromZeroPageWithXOffset_SetZeroFlag) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = false;
    computer.cpu.A = 0x00;
    computer.cpu.X = 0x02;
    computer.memory[0x1000] = CPU::oraZpX;
    computer.memory[0x1001] = 0x80;
    computer.memory[0x0082] = 0x00;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x00);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_TRUE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(OraInstructionTests, oraZpX_CanAndFromZeroPageWithXOffset_Wraps) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.cpu.A = 0x00;
    computer.cpu.X = 0x86;
    computer.memory[0x1000] = CPU::oraZpX;
    computer.memory[0x1001] = 0x84;
    // 0x84 + 0x86 = 0x10A  => 0x0A (with wrap)
    computer.memory[0x000A] = 0x35;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x35);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

// ================== //
//       oraAbs       //
// ================== //

TEST_F(OraInstructionTests, oraAbs_CanAndFromAbsolute) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.cpu.A = 0x00;
    computer.memory[0x1000] = CPU::oraAbs;
    computer.memory[0x1001] = 0x10;
    computer.memory[0x1002] = 0x30;
    computer.memory[0x3010] = 0x35;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x35);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(OraInstructionTests, oraAbs_CanAndFromAbsolute_SetNegativeFlag) {
    // Given:
    computer.cpu.flag.N = false;
    computer.cpu.flag.Z = true;
    computer.cpu.A = 0x00;
    computer.memory[0x1000] = CPU::oraAbs;
    computer.memory[0x1001] = 0x10;
    computer.memory[0x1002] = 0x30;
    computer.memory[0x3010] = 0x95;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x95);
    EXPECT_TRUE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(OraInstructionTests, oraAbs_CanAndFromAbsolute_SetZeroFlag) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = false;
    computer.cpu.A = 0x00;
    computer.memory[0x1000] = CPU::oraAbs;
    computer.memory[0x1001] = 0x10;
    computer.memory[0x1002] = 0x30;
    computer.memory[0x3010] = 0x00;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x00);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_TRUE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

// ================== //
//       oraAbX       //
// ================== //

TEST_F(OraInstructionTests, oraAbX_CanAndFromAbsoluteX) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.cpu.A = 0x00;
    computer.cpu.X = 0x02;
    computer.memory[0x1000] = CPU::oraAbX;
    computer.memory[0x1001] = 0x10;
    computer.memory[0x1002] = 0x30;
    computer.memory[0x3012] = 0x35;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x35);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(OraInstructionTests, oraAbX_CanAndFromAbsoluteX_SetNegativeFlag) {
    // Given:
    computer.cpu.flag.N = false;
    computer.cpu.flag.Z = true;
    computer.cpu.A = 0x00;
    computer.cpu.X = 0x02;
    computer.memory[0x1000] = CPU::oraAbX;
    computer.memory[0x1001] = 0x10;
    computer.memory[0x1002] = 0x30;
    computer.memory[0x3012] = 0x95;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x95);
    EXPECT_TRUE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(OraInstructionTests, oraAbX_CanAndFromAbsoluteX_SetZeroFlag) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = false;
    computer.cpu.A = 0x00;
    computer.cpu.X = 0x02;
    computer.memory[0x1000] = CPU::oraAbX;
    computer.memory[0x1001] = 0x10;
    computer.memory[0x1002] = 0x30;
    computer.memory[0x3012] = 0x00;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x00);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_TRUE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(OraInstructionTests, oraAbX_CanAndFromAbsoluteX_CrossingPageBoundary) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.cpu.A = 0x00;
    computer.cpu.X = 0x86;
    computer.memory[0x1000] = CPU::oraAbX;
    computer.memory[0x1001] = 0x84;
    computer.memory[0x1002] = 0x30;
    // 0x3084 + 0x86 = 0x310A
    computer.memory[0x310A] = 0x35;
    const int EXPECTED_CYCLES = 5;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x35);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

// ================== //
//       oraAbY       //
// ================== //

TEST_F(OraInstructionTests, oraAbY_CanAndFromAbsoluteY) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.cpu.A = 0x00;
    computer.cpu.Y = 0x02;
    computer.memory[0x1000] = CPU::oraAbY;
    computer.memory[0x1001] = 0x10;
    computer.memory[0x1002] = 0x30;
    computer.memory[0x3012] = 0x35;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x35);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(OraInstructionTests, oraAbY_CanAndFromAbsoluteY_SetNegativeFlag) {
    // Given:
    computer.cpu.flag.N = false;
    computer.cpu.flag.Z = true;
    computer.cpu.A = 0x00;
    computer.cpu.Y = 0x02;
    computer.memory[0x1000] = CPU::oraAbY;
    computer.memory[0x1001] = 0x10;
    computer.memory[0x1002] = 0x30;
    computer.memory[0x3012] = 0x95;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x95);
    EXPECT_TRUE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(OraInstructionTests, oraAbY_CanAndFromAbsoluteY_SetZeroFlag) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = false;
    computer.cpu.A = 0x00;
    computer.cpu.Y = 0x02;
    computer.memory[0x1000] = CPU::oraAbY;
    computer.memory[0x1001] = 0x10;
    computer.memory[0x1002] = 0x30;
    computer.memory[0x3012] = 0x00;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x00);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_TRUE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(OraInstructionTests, oraAbY_CanAndFromAbsoluteY_CrossingPageBoundary) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.cpu.A = 0x00;
    computer.cpu.Y = 0x86;
    computer.memory[0x1000] = CPU::oraAbY;
    computer.memory[0x1001] = 0x84;
    computer.memory[0x1002] = 0x30;
    // 0x3084 + 0x86 = 0x310A
    computer.memory[0x310A] = 0x35;
    const int EXPECTED_CYCLES = 5;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x35);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

// ================== //
//       oraIdX       //
// ================== //

TEST_F(OraInstructionTests, oraIdX_CanAndFromIndirectX) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.cpu.A = 0x00;
    computer.cpu.X = 0x02;
    computer.memory[0x1000] = CPU::oraIdX;
    computer.memory[0x1001] = 0x80;
    computer.memory[0x0082] = 0x10;
    computer.memory[0x0083] = 0x30;
    computer.memory[0x3010] = 0x35;
    const int EXPECTED_CYCLES = 6;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x35);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(OraInstructionTests, oraIdX_CanAndFromIndirectX_SetNegativeFlag) {
    // Given:
    computer.cpu.flag.N = false;
    computer.cpu.flag.Z = true;
    computer.cpu.A = 0x00;
    computer.cpu.X = 0x02;
    computer.memory[0x1000] = CPU::oraIdX;
    computer.memory[0x1001] = 0x80;
    computer.memory[0x0082] = 0x10;
    computer.memory[0x0083] = 0x30;
    computer.memory[0x3010] = 0x95;
    const int EXPECTED_CYCLES = 6;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x95);
    EXPECT_TRUE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(OraInstructionTests, oraIdX_CanAndFromIndirectX_SetZeroFlag) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = false;
    computer.cpu.A = 0x00;
    computer.cpu.X = 0x02;
    computer.memory[0x1000] = CPU::oraIdX;
    computer.memory[0x1001] = 0x80;
    computer.memory[0x0082] = 0x10;
    computer.memory[0x0083] = 0x30;
    computer.memory[0x3010] = 0x00;
    const int EXPECTED_CYCLES = 6;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x00);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_TRUE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(OraInstructionTests, oraIdX_CanAndFromIndirectX_Wraps) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.cpu.A = 0x00;
    computer.cpu.X = 0x86;
    computer.memory[0x1000] = CPU::oraIdX;
    computer.memory[0x1001] = 0x84;
    // 0x84 + 0x86 = 0x10A  => 0x0A (with wrap)
    computer.memory[0x000A] = 0x10;
    computer.memory[0x000B] = 0x30;
    computer.memory[0x3010] = 0x35;
    const int EXPECTED_CYCLES = 6;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x35);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

// ================== //
//       oraIdY       //
// ================== //

TEST_F(OraInstructionTests, oraIdY_CanAndFromIndirectY) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.cpu.A = 0x00;
    computer.cpu.Y = 0x02;
    computer.memory[0x1000] = CPU::oraIdY;
    computer.memory[0x1001] = 0x80;
    computer.memory[0x0080] = 0x10;
    computer.memory[0x0081] = 0x30;
    computer.memory[0x3012] = 0x35;
    const int EXPECTED_CYCLES = 5;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x35);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(OraInstructionTests, oraIdY_CanAndFromIndirectY_SetNegativeFlag) {
    // Given:
    computer.cpu.flag.N = false;
    computer.cpu.flag.Z = true;
    computer.cpu.A = 0x00;
    computer.cpu.Y = 0x02;
    computer.memory[0x1000] = CPU::oraIdY;
    computer.memory[0x1001] = 0x80;
    computer.memory[0x0080] = 0x10;
    computer.memory[0x0081] = 0x30;
    computer.memory[0x3012] = 0x95;
    const int EXPECTED_CYCLES = 5;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x95);
    EXPECT_TRUE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(OraInstructionTests, oraIdY_CanAndFromIndirectY_SetZeroFlag) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = false;
    computer.cpu.A = 0x00;
    computer.cpu.Y = 0x02;
    computer.memory[0x1000] = CPU::oraIdY;
    computer.memory[0x1001] = 0x80;
    computer.memory[0x0080] = 0x10;
    computer.memory[0x0081] = 0x30;
    computer.memory[0x3012] = 0x00;
    const int EXPECTED_CYCLES = 5;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x00);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_TRUE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(OraInstructionTests, oraIdY_CanAndFromIndirectY_CrossingPageBoundary) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.cpu.A = 0x00;
    computer.cpu.Y = 0x86;
    computer.memory[0x1000] = CPU::oraIdY;
    computer.memory[0x1001] = 0x80;
    computer.memory[0x0080] = 0x84;
    computer.memory[0x0081] = 0x30;
    // 0x3084 + 0x86 = 0x310A
    computer.memory[0x310A] = 0x35;
    const int EXPECTED_CYCLES = 6;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.A, 0x35);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}
