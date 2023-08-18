
#include "gtest/gtest.h"
#include "../src/Computer.h"

class StoreRegisterATests : public ::testing::Test {
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
//       staZpg       //
// ================== //

TEST_F(StoreRegisterATests, staZpg_CanStoreARegisterIntoMemory) {
    // Given:
    computer.cpu.A = 0x35;
    computer.memory[0xFFFC] = CPU::staZpg;
    computer.memory[0xFFFD] = 0x80;
    const int EXPECTED_CYCLES = 3;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.memory[0x0080], 0x35);
    VerifyUnchangedFlags(cpuCopy);
}

// ================== //
//       staZpX       //
// ================== //

TEST_F(StoreRegisterATests, staZpX_CanStoreARegisterIntoMemory) {
    // Given:
    computer.cpu.A = 0x35;
    computer.cpu.X = 0x02;
    computer.memory[0xFFFC] = CPU::staZpX;
    computer.memory[0xFFFD] = 0x80;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.memory[0x0082], 0x35);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(StoreRegisterATests, staZpX_CanStoreARegisterIntoMemory_Wraps) {
    // Given:
    computer.cpu.A = 0x35;
    computer.cpu.X = 0x86;
    computer.memory[0xFFFC] = CPU::staZpX;
    computer.memory[0xFFFD] = 0x84;
    // 0x84 + 0x86 = 0x10A  => 0x0A (with wrap)
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.memory[0x000A], 0x35);
    VerifyUnchangedFlags(cpuCopy);
}

// ================== //
//       staAbs       //
// ================== //

TEST_F(StoreRegisterATests, staAbs_CanStoreARegisterIntoMemory) {
    // Given:
    computer.cpu.A = 0x35;
    computer.memory[0xFFFC] = CPU::staAbs;
    computer.memory[0xFFFD] = 0x10;
    computer.memory[0xFFFE] = 0x30;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.memory[0x3010], 0x35);
    VerifyUnchangedFlags(cpuCopy);
}

// ================== //
//       staAbX       //
// ================== //

TEST_F(StoreRegisterATests, staAbX_CanStoreARegisterIntoMemory) {
    // Given:
    computer.cpu.A = 0x35;
    computer.cpu.X = 0x02;
    computer.memory[0xFFFC] = CPU::staAbX;
    computer.memory[0xFFFD] = 0x10;
    computer.memory[0xFFFE] = 0x30;
    const int EXPECTED_CYCLES = 5;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.memory[0x3012], 0x35);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(StoreRegisterATests, staAbX_CanStoreARegisterIntoMemory_CrossingPageBoundary) {
    // Given:
    computer.cpu.A = 0x35;
    computer.cpu.X = 0x86;
    computer.memory[0xFFFC] = CPU::staAbX;
    computer.memory[0xFFFD] = 0x84;
    computer.memory[0xFFFE] = 0x30;
    // 0x3084 + 0x86 = 0x310A
    const int EXPECTED_CYCLES = 5;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.memory[0x310A], 0x35);
    VerifyUnchangedFlags(cpuCopy);
}

// ================== //
//       staAbY       //
// ================== //

TEST_F(StoreRegisterATests, staAbY_CanStoreARegisterIntoMemory) {
    // Given:
    computer.cpu.A = 0x35;
    computer.cpu.Y = 0x02;
    computer.memory[0xFFFC] = CPU::staAbY;
    computer.memory[0xFFFD] = 0x10;
    computer.memory[0xFFFE] = 0x30;
    const int EXPECTED_CYCLES = 5;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.memory[0x3012], 0x35);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(StoreRegisterATests, staAbY_CanStoreARegisterIntoMemory_CrossingPageBoundary) {
    // Given:
    computer.cpu.A = 0x35;
    computer.cpu.Y = 0x86;
    computer.memory[0xFFFC] = CPU::staAbY;
    computer.memory[0xFFFD] = 0x84;
    computer.memory[0xFFFE] = 0x30;
    // 0x3084 + 0x86 = 0x310A
    const int EXPECTED_CYCLES = 5;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.memory[0x310A], 0x35);
    VerifyUnchangedFlags(cpuCopy);
}

// ================== //
//       staIdX       //
// ================== //

TEST_F(StoreRegisterATests, staIdX_CanStoreARegisterIntoMemory) {
    // Given:
    computer.cpu.A = 0x35;
    computer.cpu.X = 0x02;
    computer.memory[0xFFFC] = CPU::staIdX;
    computer.memory[0xFFFD] = 0x80;
    computer.memory[0x0082] = 0x10;
    computer.memory[0x0083] = 0x30;
    const int EXPECTED_CYCLES = 6;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.memory[0x3010], 0x35);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(StoreRegisterATests, staIdX_CanStoreARegisterIntoMemory_Wraps) {
    // Given:
    computer.cpu.A = 0x35;
    computer.cpu.X = 0x86;
    computer.memory[0xFFFC] = CPU::staIdX;
    computer.memory[0xFFFD] = 0x84;
    // 0x84 + 0x86 = 0x10A  => 0x0A (with wrap)
    computer.memory[0x000A] = 0x10;
    computer.memory[0x000B] = 0x30;
    const int EXPECTED_CYCLES = 6;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.memory[0x3010], 0x35);
    VerifyUnchangedFlags(cpuCopy);
}

// ================== //
//       staIdY       //
// ================== //

TEST_F(StoreRegisterATests, staIdY_CanStoreARegisterIntoMemory) {
    // Given:
    computer.cpu.A = 0x35;
    computer.cpu.Y = 0x02;
    computer.memory[0xFFFC] = CPU::staIdY;
    computer.memory[0xFFFD] = 0x80;
    computer.memory[0x0080] = 0x10;
    computer.memory[0x0081] = 0x30;
    const int EXPECTED_CYCLES = 6;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.memory[0x3012], 0x35);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(StoreRegisterATests, staIdY_CanStoreARegisterIntoMemory_CrossingPageBoundary) {
    // Given:
    computer.cpu.A = 0x35;
    computer.cpu.Y = 0x86;
    computer.memory[0xFFFC] = CPU::staIdY;
    computer.memory[0xFFFD] = 0x80;
    computer.memory[0x0080] = 0x84;
    computer.memory[0x0081] = 0x30;
    // 0x3084 + 0x86 = 0x310A
    const int EXPECTED_CYCLES = 6;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.memory[0x310A], 0x35);
    VerifyUnchangedFlags(cpuCopy);
}
