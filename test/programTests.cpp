
#include "gtest/gtest.h"
#include "../src/Computer.h"

class ProgramTests : public ::testing::Test {
public:
    Computer computer;

    void SetUp() override { computer.reset(); }
    void TearDown() override {}
};

// ================== //
//      Program1      //
// ================== //

TEST_F(ProgramTests, loadProgram1Test) {
    //Given:
    const dword noBytes = 6;
    const byte program[noBytes] = {0x00, 0x80, CPU::ldaImm, 0xF0, CPU::andImm, 0x1F};

    // When:
    computer.loadProgram(program, noBytes);

    // Then:
    EXPECT_EQ(computer.memory[CPU::RESET_ADRESS], 0x00);
    EXPECT_EQ(computer.memory[CPU::RESET_ADRESS + 1], 0x80);
    for (int i = 0; i < 4; i++) {
        EXPECT_EQ(program[i + 2], computer.memory[0x8000 + i]);
    }
}

TEST_F(ProgramTests, loadProgram1Test_ResetPCWorks) {
    //Given:
    const dword noBytes = 6;
    const byte program[noBytes] = {0x00, 0x80, CPU::ldaImm, 0xF0, CPU::andImm, 0x1F};

    // When:
    computer.loadProgram(program, noBytes);
    computer.resetPC();

    // Then:
    EXPECT_EQ(computer.cpu.PC, 0x8000);
}

TEST_F(ProgramTests, runProgram1) {
    //Given:
    const dword noBytes = 6;
    const byte program[noBytes] = {0x00, 0x80, CPU::ldaImm, 0xF0, CPU::andImm, 0x1F};

    // When:
    computer.loadProgram(program, noBytes);
    computer.resetPC();
    computer.run(2 + 2);

    // Then:
    EXPECT_EQ(computer.cpu.A, 0xF0 & 0x1F);
}
