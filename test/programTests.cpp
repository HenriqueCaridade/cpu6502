
#include "gtest/gtest.h"
#include "../src/Computer.h"

class ProgramTests : public ::testing::Test {
public:
    Computer computer;

    void SetUp() override { computer.reset(); }
    void TearDown() override {}
};

TEST_F(ProgramTests, loadProgramTest) {
    //Given:
    const dword noBytes = 6;
    const byte program[noBytes] = {0x00, 0x20, 0xA9, 0xF0, 0x29, 0x1F};

    // When:
    computer.loadProgram(program, noBytes);

    // Then:
    EXPECT_EQ(computer.memory[CPU::RESET_ADRESS], 0x00);
    EXPECT_EQ(computer.memory[CPU::RESET_ADRESS + 1], 0x20);
    for (int i = 0; i < 4; i++) {
        EXPECT_EQ(program[i + 2], computer.memory[0x2000 + i]);
    }
}

TEST_F(ProgramTests, loadProgramTest_ResetPCWorks) {
    //Given:
    const dword noBytes = 6;
    const byte program[noBytes] = {0x00, 0x20, 0xA9, 0xF0, 0x29, 0x1F};

    // When:
    computer.loadProgram(program, noBytes);
    computer.resetPC();

    // Then:
    EXPECT_EQ(computer.cpu.PC, 0x2000);
}

// ================== //
//      Program1      //
// ================== //

TEST_F(ProgramTests, runProgram1) {
    //Given:
    /*
    * = $2000

    lda #$F0
    and #$1F
     */
    const dword noBytes = 6;
    const byte program[noBytes] = {0x00, 0x20, 0xA9, 0xF0, 0x29, 0x1F};

    // When:
    computer.loadProgram(program, noBytes);
    computer.resetPC();
    computer.run(2 + 2);

    // Then:
    EXPECT_EQ(computer.cpu.A, 0xF0 & 0x1F);
}

// ================== //
//      Program2      //
// ================== //

TEST_F(ProgramTests, runProgram2) {
    //Given:
    /*
    * = $2000

    lda #$10
    sta $80

    ldx #$00

    loop:
    inc $80
    inx
    jmp loop
     */
    const dword noBytes = 14;
    const byte program[noBytes] = {0x00, 0x20, 0xA9, 0x10, 0x85, 0x80, 0xA2, 0x00, 0xE6, 0x80, 0xE8, 0x4C, 0x06, 0x20};
    const int CYCLES_PER_LOOP = 5 + 2 + 3;
    const int LOOP_COUNT = 0x38;
    // 0x10 + 0x38 = 0x48
    // When:
    computer.loadProgram(program, noBytes);
    computer.resetPC();
    computer.run(2 + 3 + 2 + CYCLES_PER_LOOP * LOOP_COUNT);

    // Then:
    EXPECT_EQ(computer.memory[0x0080], 0x48);
    EXPECT_EQ(computer.cpu.X, 0x38);
}
