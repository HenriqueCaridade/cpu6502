
#include "gtest/gtest.h"
#include "../src/Computer.h"

class StoreRegisterYTests : public ::testing::Test {
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
//       styZpg       //
// ================== //

TEST_F(StoreRegisterYTests, styZpg_CanStoreYRegisterIntoMemory) {
    // Given:
    computer.cpu.Y = 0x35;
    computer.memory[0x1000] = CPU::styZpg;
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
//       styZpX       //
// ================== //

TEST_F(StoreRegisterYTests, styZpX_CanStoreYRegisterIntoMemory) {
    // Given:
    computer.cpu.Y = 0x35;
    computer.cpu.X = 0x02;
    computer.memory[0x1000] = CPU::styZpX;
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

TEST_F(StoreRegisterYTests, styZpX_CanStoreYRegisterIntoMemory_Wraps) {
    // Given:
    computer.cpu.Y = 0x35;
    computer.cpu.X = 0x86;
    computer.memory[0x1000] = CPU::styZpX;
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
//       styAbs       //
// ================== //

TEST_F(StoreRegisterYTests, styAbs_CanStoreYRegisterIntoMemory) {
    // Given:
    computer.cpu.Y = 0x35;
    computer.memory[0x1000] = CPU::styAbs;
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
