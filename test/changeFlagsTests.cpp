
#include "gtest/gtest.h"
#include "../src/Computer.h"

class SetAndClearFlagInstructions : public ::testing::Test {
public:
    Computer computer;

    void SetUp() override { computer.reset(); }
    void TearDown() override {}

    void VerifyUnchangedFlags(byte unchangedFlags, const CPU& copy) const {
        EXPECT_EQ(computer.cpu.status & unchangedFlags, copy.status & unchangedFlags);
    }

    void clearTest(CPU::Instruction instruction, byte flag) {
        // Given:
        byte otherFlags = (~flag) & 0b11001111;
        computer.cpu.status = flag;
        computer.memory[0x1000] = instruction;
        const int EXPECTED_CYCLES = 2;
        const CPU cpuCopy = computer.cpu;

        // When:
        int cyclesExecuted = computer.run(EXPECTED_CYCLES);

        // Then:
        EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
        EXPECT_FALSE(computer.cpu.status & flag);
        VerifyUnchangedFlags(otherFlags, cpuCopy);

    }

    void setTest(CPU::Instruction instruction, byte flag) {
        // Given:
        byte otherFlags = (~flag) & 0b11001111;
        computer.cpu.status = otherFlags;
        computer.memory[0x1000] = instruction;
        const int EXPECTED_CYCLES = 2;
        const CPU cpuCopy = computer.cpu;

        // When:
        int cyclesExecuted = computer.run(EXPECTED_CYCLES);

        // Then:
        EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
        EXPECT_TRUE(computer.cpu.status & flag);
        VerifyUnchangedFlags(otherFlags, cpuCopy);
    }
};

// ================== //
//       clcImp       //
// ================== //

TEST_F(SetAndClearFlagInstructions, clcImp_CanClearCarryFlag) {
    clearTest(CPU::clcImp, CPU::FLAG_C);
}

// ================== //
//       cldImp       //
// ================== //

TEST_F(SetAndClearFlagInstructions, cldImp_CanClearDecimalFlag) {
    clearTest(CPU::cldImp, CPU::FLAG_D);
}

// ================== //
//       cliImp       //
// ================== //

TEST_F(SetAndClearFlagInstructions, cliImp_CanClearInterruptFlag) {
    clearTest(CPU::cliImp, CPU::FLAG_I);
}

// ================== //
//       clvImp       //
// ================== //

TEST_F(SetAndClearFlagInstructions, clvImp_CanClearOverflowFlag) {
    clearTest(CPU::clvImp, CPU::FLAG_V);
}

// ================== //
//       secImp       //
// ================== //

TEST_F(SetAndClearFlagInstructions, secImp_CanSetCarryFlag) {
    setTest(CPU::secImp, CPU::FLAG_C);
}

// ================== //
//       sedImp       //
// ================== //

TEST_F(SetAndClearFlagInstructions, sedImp_CanSetDecimalFlag) {
    setTest(CPU::sedImp, CPU::FLAG_D);
}

// ================== //
//       seiImp       //
// ================== //

TEST_F(SetAndClearFlagInstructions, seiImp_CanSetInterruptFlag) {
    setTest(CPU::seiImp, CPU::FLAG_I);
}
