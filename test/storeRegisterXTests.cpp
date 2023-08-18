
#include "gtest/gtest.h"
#include "../src/Computer.h"

class StoreRegisterXTests : public ::testing::Test {
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
//       stxZpg       //
// ================== //

TEST_F(StoreRegisterXTests, stxZpg_CanStoreXRegisterIntoMemory) {
    // Given:
    computer.cpu.X = 0x35;
    computer.memory[0x1000] = CPU::stxZpg;
    computer.memory[0x1001] = 0x80;
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
//       stxZpY       //
// ================== //

TEST_F(StoreRegisterXTests, stxZpY_CanStoreXRegisterIntoMemory) {
    // Given:
    computer.cpu.X = 0x35;
    computer.cpu.Y = 0x02;
    computer.memory[0x1000] = CPU::stxZpY;
    computer.memory[0x1001] = 0x80;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.memory[0x0082], 0x35);
    VerifyUnchangedFlags(cpuCopy);
}

TEST_F(StoreRegisterXTests, stxZpY_CanStoreXRegisterIntoMemory_Wraps) {
    // Given:
    computer.cpu.X = 0x35;
    computer.cpu.Y = 0x86;
    computer.memory[0x1000] = CPU::stxZpY;
    computer.memory[0x1001] = 0x84;
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
//       stxAbs       //
// ================== //

TEST_F(StoreRegisterXTests, stxAbs_CanStoreXRegisterIntoMemory) {
    // Given:
    computer.cpu.X = 0x35;
    computer.memory[0x1000] = CPU::stxAbs;
    computer.memory[0x1001] = 0x10;
    computer.memory[0x1002] = 0x30;
    const int EXPECTED_CYCLES = 4;
    const CPU cpuCopy = computer.cpu;

    // When:
    int cyclesExecuted = computer.run(EXPECTED_CYCLES);

    // Then:
    EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
    EXPECT_EQ(computer.memory[0x3010], 0x35);
    VerifyUnchangedFlags(cpuCopy);
}
