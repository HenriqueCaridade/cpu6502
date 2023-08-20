
#include "gtest/gtest.h"
#include "../src/Computer.h"

class BranchTests : public ::testing::Test {
public:
    static const byte UNCHANGED_FLAGS = 0b11001111;
    Computer computer;

    void SetUp() override { computer.reset(); }
    void TearDown() override {}

    void VerifyUnchangedFlags(const CPU& copy) const {
        EXPECT_EQ(computer.cpu.status & UNCHANGED_FLAGS, copy.status & UNCHANGED_FLAGS);
    }

    void DoesntBranch(CPU::Instruction instruction) {
        // Given:
        computer.memory[0x1000] = instruction;
        computer.memory[0x1001] = 0x04;
        const int EXPECTED_CYCLES = 2;
        const CPU cpuCopy = computer.cpu;

        // When:
        int cyclesExecuted = computer.run(EXPECTED_CYCLES);

        // Then:
        EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
        EXPECT_EQ(computer.cpu.PC, 0x1002);
        VerifyUnchangedFlags(cpuCopy);
    }

    void DoesBranch(CPU::Instruction instruction) {
        // Given:
        computer.memory[0x1000] = instruction;
        computer.memory[0x1001] = 0x04;
        const int EXPECTED_CYCLES = 3;
        const CPU cpuCopy = computer.cpu;

        // When:
        int cyclesExecuted = computer.run(EXPECTED_CYCLES);

        // Then:
        EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
        EXPECT_EQ(computer.cpu.PC, 0x1006);
        VerifyUnchangedFlags(cpuCopy);
    }

    void DoesBranch_CrossingPageBoundary(CPU::Instruction instruction) {
        // Given:
        computer.memory[0x1000] = instruction;
        computer.memory[0x1001] = 0x80; // -128
        const int EXPECTED_CYCLES = 4;
        const CPU cpuCopy = computer.cpu;

        // When:
        int cyclesExecuted = computer.run(EXPECTED_CYCLES);

        // Then:
        EXPECT_EQ(cyclesExecuted, EXPECTED_CYCLES);
        EXPECT_EQ(computer.cpu.PC, 0x0F82);
        VerifyUnchangedFlags(cpuCopy);
    }
};

// ================== //
//       bccRel       //
// ================== //

TEST_F(BranchTests, bccRel_DoesntBranchWhenCarryFlagSet) {
    computer.cpu.flag.C = true;
    DoesntBranch(CPU::bccRel);
}

TEST_F(BranchTests, bccRel_DoesBranchWhenCarryFlagClear) {
    computer.cpu.flag.C = false;
    DoesBranch(CPU::bccRel);
}

TEST_F(BranchTests, bccRel_DoesBranchWhenCarryFlagClear_CrossingPageBoundary) {
    computer.cpu.flag.C = false;
    DoesBranch_CrossingPageBoundary(CPU::bccRel);
}

// ================== //
//       bcsRel       //
// ================== //

TEST_F(BranchTests, bcsRel_DoesntBranchWhenCarryFlagClear) {
    computer.cpu.flag.C = false;
    DoesntBranch(CPU::bcsRel);
}

TEST_F(BranchTests, bcsRel_DoesBranchWhenCarryFlagSet) {
    computer.cpu.flag.C = true;
    DoesBranch(CPU::bcsRel);
}

TEST_F(BranchTests, bcsRel_DoesBranchWhenCarryFlagSet_CrossingPageBoundary) {
    computer.cpu.flag.C = true;
    DoesBranch_CrossingPageBoundary(CPU::bcsRel);
}

// ================== //
//       beqRel       //
// ================== //

TEST_F(BranchTests, beqRel_DoesntBranchWhenZeroFlagClear) {
    computer.cpu.flag.Z = false;
    DoesntBranch(CPU::beqRel);
}

TEST_F(BranchTests, beqRel_DoesBranchWhenZeroFlagSet) {
    computer.cpu.flag.Z = true;
    DoesBranch(CPU::beqRel);
}

TEST_F(BranchTests, beqRel_DoesBranchWhenZeroFlagSet_CrossingPageBoundary) {
    computer.cpu.flag.Z = true;
    DoesBranch_CrossingPageBoundary(CPU::beqRel);
}

// ================== //
//       bmiRel       //
// ================== //

TEST_F(BranchTests, bmiRel_DoesntBranchWhenNegativeFlagClear) {
    computer.cpu.flag.N = false;
    DoesntBranch(CPU::bmiRel);
}

TEST_F(BranchTests, bmiRel_DoesBranchWhenNegativeFlagSet) {
    computer.cpu.flag.N = true;
    DoesBranch(CPU::bmiRel);
}

TEST_F(BranchTests, bmiRel_DoesBranchWhenNegativeFlagSet_CrossingPageBoundary) {
    computer.cpu.flag.N = true;
    DoesBranch_CrossingPageBoundary(CPU::bmiRel);
}

// ================== //
//       bneRel       //
// ================== //


TEST_F(BranchTests, bneRel_DoesntBranchWhenZeroFlagSet) {
    computer.cpu.flag.Z = true;
    DoesntBranch(CPU::bneRel);
}

TEST_F(BranchTests, bneRel_DoesBranchWhenZeroFlagClear) {
    computer.cpu.flag.Z = false;
    DoesBranch(CPU::bneRel);
}

TEST_F(BranchTests, bneRel_DoesBranchWhenZeroFlagClear_CrossingPageBoundary) {
    computer.cpu.flag.Z = false;
    DoesBranch_CrossingPageBoundary(CPU::bneRel);
}

// ================== //
//       bplRel       //
// ================== //

TEST_F(BranchTests, bplRel_DoesntBranchWhenNegativeFlagSet) {
    computer.cpu.flag.N = true;
    DoesntBranch(CPU::bplRel);
}

TEST_F(BranchTests, bplRel_DoesBranchWhenNegativeFlagClear) {
    computer.cpu.flag.N = false;
    DoesBranch(CPU::bplRel);
}

TEST_F(BranchTests, bplRel_DoesBranchWhenNegativeFlagClear_CrossingPageBoundary) {
    computer.cpu.flag.N = false;
    DoesBranch_CrossingPageBoundary(CPU::bplRel);
}

// ================== //
//       bvcRel       //
// ================== //

TEST_F(BranchTests, bvcRel_DoesntBranchWhenOverflowFlagSet) {
    computer.cpu.flag.V = true;
    DoesntBranch(CPU::bvcRel);
}

TEST_F(BranchTests, bvcRel_DoesBranchWhenOverflowFlagClear) {
    computer.cpu.flag.V = false;
    DoesBranch(CPU::bvcRel);
}

TEST_F(BranchTests, bvcRel_DoesBranchWhenOverflowFlagClear_CrossingPageBoundary) {
    computer.cpu.flag.V = false;
    DoesBranch_CrossingPageBoundary(CPU::bvcRel);
}

// ================== //
//       bvsRel       //
// ================== //

TEST_F(BranchTests, bvsRel_DoesntBranchWhenOverflowFlagClear) {
    computer.cpu.flag.V = false;
    DoesntBranch(CPU::bvsRel);
}

TEST_F(BranchTests, bvsRel_DoesBranchWhenOverflowFlagSet) {
    computer.cpu.flag.V = true;
    DoesBranch(CPU::bvsRel);
}

TEST_F(BranchTests, bvsRel_DoesBranchWhenOverflowFlagSet_CrossingPageBoundary) {
    computer.cpu.flag.V = true;
    DoesBranch_CrossingPageBoundary(CPU::bvsRel);
}

