
#include "gtest/gtest.h"
#include "../src/Computer.h"

class LoadRegisterATests : public ::testing::Test {
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
//       ldaImm       //
// ================== //

TEST_F(LoadRegisterATests, ldaImm_CanLoadToARegister) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.memory[0x1000] = CPU::ldaImm;
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

TEST_F(LoadRegisterATests, ldaImm_CanLoadToARegister_SetNegativeFlag) {
    // Given:
    computer.cpu.flag.N = false;
    computer.cpu.flag.Z = true;
    computer.memory[0x1000] = CPU::ldaImm;
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

TEST_F(LoadRegisterATests, ldaImm_CanLoadToARegister_SetZeroFlag) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = false;
    computer.memory[0x1000] = CPU::ldaImm;
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
//       ldaZpg       //
// ================== //

TEST_F(LoadRegisterATests, ldaZpg_CanLoadToARegister) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.memory[0x1000] = CPU::ldaZpg;
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

TEST_F(LoadRegisterATests, ldaZpg_CanLoadToARegister_SetNegativeFlag) {
    // Given:
    computer.cpu.flag.N = false;
    computer.cpu.flag.Z = true;
    computer.memory[0x1000] = CPU::ldaZpg;
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

TEST_F(LoadRegisterATests, ldaZpg_CanLoadToARegister_SetZeroFlag) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = false;
    computer.memory[0x1000] = CPU::ldaZpg;
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
//       ldaZpX       //
// ================== //

TEST_F(LoadRegisterATests, ldaZpX_CanLoadToARegister) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.cpu.X = 0x02;
    computer.memory[0x1000] = CPU::ldaZpX;
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

TEST_F(LoadRegisterATests, ldaZpX_CanLoadToARegister_SetNegativeFlag) {
    // Given:
    computer.cpu.flag.N = false;
    computer.cpu.flag.Z = true;
    computer.cpu.X = 0x02;
    computer.memory[0x1000] = CPU::ldaZpX;
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

TEST_F(LoadRegisterATests, ldaZpX_CanLoadToARegister_SetZeroFlag) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = false;
    computer.cpu.X = 0x02;
    computer.memory[0x1000] = CPU::ldaZpX;
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

TEST_F(LoadRegisterATests, ldaZpX_CanLoadToARegister_Wraps) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.cpu.X = 0x86;
    computer.memory[0x1000] = CPU::ldaZpX;
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
//       ldaAbs       //
// ================== //

TEST_F(LoadRegisterATests, ldaAbs_CanLoadToARegister) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.memory[0x1000] = CPU::ldaAbs;
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

TEST_F(LoadRegisterATests, ldaAbs_CanLoadToARegister_SetNegativeFlag) {
    // Given:
    computer.cpu.flag.N = false;
    computer.cpu.flag.Z = true;
    computer.memory[0x1000] = CPU::ldaAbs;
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

TEST_F(LoadRegisterATests, ldaAbs_CanLoadToARegister_SetZeroFlag) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = false;
    computer.memory[0x1000] = CPU::ldaAbs;
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
//       ldaAbX       //
// ================== //

TEST_F(LoadRegisterATests, ldaAbX_CanLoadToARegister) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.cpu.X = 0x02;
    computer.memory[0x1000] = CPU::ldaAbX;
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

TEST_F(LoadRegisterATests, ldaAbX_CanLoadToARegister_SetNegativeFlag) {
    // Given:
    computer.cpu.flag.N = false;
    computer.cpu.flag.Z = true;
    computer.cpu.X = 0x02;
    computer.memory[0x1000] = CPU::ldaAbX;
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

TEST_F(LoadRegisterATests, ldaAbX_CanLoadToARegister_SetZeroFlag) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = false;
    computer.cpu.X = 0x02;
    computer.memory[0x1000] = CPU::ldaAbX;
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

TEST_F(LoadRegisterATests, ldaAbX_CanLoadToARegister_CrossingPageBoundary) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.cpu.X = 0x86;
    computer.memory[0x1000] = CPU::ldaAbX;
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
//       ldaAbY       //
// ================== //

TEST_F(LoadRegisterATests, ldaAbY_CanLoadToARegister) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.cpu.Y = 0x02;
    computer.memory[0x1000] = CPU::ldaAbY;
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

TEST_F(LoadRegisterATests, ldaAbY_CanLoadToARegister_SetNegativeFlag) {
    // Given:
    computer.cpu.flag.N = false;
    computer.cpu.flag.Z = true;
    computer.cpu.Y = 0x02;
    computer.memory[0x1000] = CPU::ldaAbY;
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

TEST_F(LoadRegisterATests, ldaAbY_CanLoadToARegister_SetZeroFlag) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = false;
    computer.cpu.Y = 0x02;
    computer.memory[0x1000] = CPU::ldaAbY;
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

TEST_F(LoadRegisterATests, ldaAbY_CanLoadToARegister_CrossingPageBoundary) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.cpu.Y = 0x86;
    computer.memory[0x1000] = CPU::ldaAbY;
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
//       ldaIdX       //
// ================== //

TEST_F(LoadRegisterATests, ldaIdX_CanLoadToARegister) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.cpu.X = 0x02;
    computer.memory[0x1000] = CPU::ldaIdX;
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

TEST_F(LoadRegisterATests, ldaIdX_CanLoadToARegister_SetNegativeFlag) {
    // Given:
    computer.cpu.flag.N = false;
    computer.cpu.flag.Z = true;
    computer.cpu.X = 0x02;
    computer.memory[0x1000] = CPU::ldaIdX;
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

TEST_F(LoadRegisterATests, ldaIdX_CanLoadToARegister_SetZeroFlag) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = false;
    computer.cpu.X = 0x02;
    computer.memory[0x1000] = CPU::ldaIdX;
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

TEST_F(LoadRegisterATests, ldaIdX_CanLoadToARegister_Wraps) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.cpu.X = 0x86;
    computer.memory[0x1000] = CPU::ldaIdX;
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
//       ldaIdY       //
// ================== //

TEST_F(LoadRegisterATests, ldaIdY_CanLoadToARegister) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.cpu.Y = 0x02;
    computer.memory[0x1000] = CPU::ldaIdY;
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

TEST_F(LoadRegisterATests, ldaIdY_CanLoadToARegister_SetNegativeFlag) {
    // Given:
    computer.cpu.flag.N = false;
    computer.cpu.flag.Z = true;
    computer.cpu.Y = 0x02;
    computer.memory[0x1000] = CPU::ldaIdY;
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

TEST_F(LoadRegisterATests, ldaIdY_CanLoadToARegister_SetZeroFlag) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = false;
    computer.cpu.Y = 0x02;
    computer.memory[0x1000] = CPU::ldaIdY;
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

TEST_F(LoadRegisterATests, ldaIdY_CanLoadToARegister_CrossingPageBoundary) {
    // Given:
    computer.cpu.flag.N = true;
    computer.cpu.flag.Z = true;
    computer.cpu.Y = 0x86;
    computer.memory[0x1000] = CPU::ldaIdY;
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
