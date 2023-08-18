
#include "gtest/gtest.h"
#include "../src/Computer.h"

class LoadRegisterXTests : public ::testing::Test {
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
//       ldxImm       //
// ================== //

TEST_F(LoadRegisterXTests, ldxImm_CanLoadToXRegister) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.memory[0xFFFC] = CPU::ldxImm;
    computer.memory[0xFFFD] = 0x35;
    const int EXPECTED_CYCLES = 2;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.X, 0x35);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(LoadRegisterXTests, ldxImm_CanLoadToXRegister_SetNegativeFlag) {
    // Given:
    computer.cpu.flag.N = false;
    computer.cpu.flag.Z = true;
    computer.memory[0xFFFC] = CPU::ldxImm;
    computer.memory[0xFFFD] = 0x95;
    const int EXPECTED_CYCLES = 2;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.X, 0x95);
    EXPECT_TRUE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(LoadRegisterXTests, ldxImm_CanLoadToXRegister_SetZeroFlag) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = false;
    computer.memory[0xFFFC] = CPU::ldxImm;
    computer.memory[0xFFFD] = 0x00;
    const int EXPECTED_CYCLES = 2;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.X, 0x00);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_TRUE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

// ================== //
//       ldxZpg       //
// ================== //

TEST_F(LoadRegisterXTests, ldxZpg_CanLoadToXRegister) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.memory[0xFFFC] = CPU::ldxZpg;
    computer.memory[0xFFFD] = 0x80;
    computer.memory[0x0080] = 0x35;
    const int EXPECTED_CYCLES = 3;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.X, 0x35);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(LoadRegisterXTests, ldxZpg_CanLoadToXRegister_SetNegativeFlag) {
    // Given:
    computer.cpu.flag.N = false;
    computer.cpu.flag.Z = true;
    computer.memory[0xFFFC] = CPU::ldxZpg;
    computer.memory[0xFFFD] = 0x80;
    computer.memory[0x0080] = 0x95;
    const int EXPECTED_CYCLES = 3;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.X, 0x95);
    EXPECT_TRUE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(LoadRegisterXTests, ldxZpg_CanLoadToXRegister_SetZeroFlag) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = false;
    computer.memory[0xFFFC] = CPU::ldxZpg;
    computer.memory[0xFFFD] = 0x80;
    computer.memory[0x0080] = 0x00;
    const int EXPECTED_CYCLES = 3;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.X, 0x00);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_TRUE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

// ================== //
//       ldxZpY       //
// ================== //

TEST_F(LoadRegisterXTests, ldxZpY_CanLoadToXRegister) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.cpu.Y = 0x02;
    computer.memory[0xFFFC] = CPU::ldxZpY;
    computer.memory[0xFFFD] = 0x80;
    computer.memory[0x0082] = 0x35;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.X, 0x35);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(LoadRegisterXTests, ldxZpY_CanLoadToXRegister_SetNegativeFlag) {
    // Given:
    computer.cpu.flag.N = false;
    computer.cpu.flag.Z = true;
    computer.cpu.Y = 0x02;
    computer.memory[0xFFFC] = CPU::ldxZpY;
    computer.memory[0xFFFD] = 0x80;
    computer.memory[0x0082] = 0x95;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.X, 0x95);
    EXPECT_TRUE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(LoadRegisterXTests, ldxZpY_CanLoadToXRegister_SetZeroFlag) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = false;
    computer.cpu.Y = 0x02;
    computer.memory[0xFFFC] = CPU::ldxZpY;
    computer.memory[0xFFFD] = 0x80;
    computer.memory[0x0082] = 0x00;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.X, 0x00);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_TRUE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(LoadRegisterXTests, ldxZpY_CanLoadToXRegister_Wraps) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.cpu.Y = 0x86;
    computer.memory[0xFFFC] = CPU::ldxZpY;
    computer.memory[0xFFFD] = 0x84;
    // 0x84 + 0x86 = 0x10A  => 0x0A (with wrap)
    computer.memory[0x000A] = 0x35;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.X, 0x35);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

// ================== //
//       ldxAbs       //
// ================== //

TEST_F(LoadRegisterXTests, ldxAbs_CanLoadToXRegister) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.memory[0xFFFC] = CPU::ldxAbs;
    computer.memory[0xFFFD] = 0x10;
    computer.memory[0xFFFE] = 0x30;
    computer.memory[0x3010] = 0x35;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.X, 0x35);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(LoadRegisterXTests, ldxAbs_CanLoadToXRegister_SetNegativeFlag) {
    // Given:
    computer.cpu.flag.N = false;
    computer.cpu.flag.Z = true;
    computer.memory[0xFFFC] = CPU::ldxAbs;
    computer.memory[0xFFFD] = 0x10;
    computer.memory[0xFFFE] = 0x30;
    computer.memory[0x3010] = 0x95;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.X, 0x95);
    EXPECT_TRUE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(LoadRegisterXTests, ldxAbs_CanLoadToXRegister_SetZeroFlag) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = false;
    computer.memory[0xFFFC] = CPU::ldxAbs;
    computer.memory[0xFFFD] = 0x10;
    computer.memory[0xFFFE] = 0x30;
    computer.memory[0x3010] = 0x00;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.X, 0x00);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_TRUE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

// ================== //
//       ldxAbY       //
// ================== //

TEST_F(LoadRegisterXTests, ldxAbY_CanLoadToXRegister) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.cpu.Y = 0x02;
    computer.memory[0xFFFC] = CPU::ldxAbY;
    computer.memory[0xFFFD] = 0x10;
    computer.memory[0xFFFE] = 0x30;
    computer.memory[0x3012] = 0x35;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.X, 0x35);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(LoadRegisterXTests, ldxAbY_CanLoadToXRegister_SetNegativeFlag) {
    // Given:
    computer.cpu.flag.N = false;
    computer.cpu.flag.Z = true;
    computer.cpu.Y = 0x02;
    computer.memory[0xFFFC] = CPU::ldxAbY;
    computer.memory[0xFFFD] = 0x10;
    computer.memory[0xFFFE] = 0x30;
    computer.memory[0x3012] = 0x95;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.X, 0x95);
    EXPECT_TRUE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(LoadRegisterXTests, ldxAbY_CanLoadToXRegister_SetZeroFlag) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = false;
    computer.cpu.Y = 0x02;
    computer.memory[0xFFFC] = CPU::ldxAbY;
    computer.memory[0xFFFD] = 0x10;
    computer.memory[0xFFFE] = 0x30;
    computer.memory[0x3012] = 0x00;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.X, 0x00);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_TRUE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(LoadRegisterXTests, ldxAbY_CanLoadToXRegister_CrossingPageBoundary) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.cpu.Y = 0x86;
    computer.memory[0xFFFC] = CPU::ldxAbY;
    computer.memory[0xFFFD] = 0x84;
    computer.memory[0xFFFE] = 0x30;
    // 0x3084 + 0x86 = 0x310A
    computer.memory[0x310A] = 0x35;
    const int EXPECTED_CYCLES = 5;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.cpu.X, 0x35);
    EXPECT_FALSE(computer.cpu.flag.N);
    EXPECT_FALSE(computer.cpu.flag.Z);
    VerifyUnchangedFlags(cpuCopy);
}
