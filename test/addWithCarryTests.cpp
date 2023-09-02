
#include "gtest/gtest.h"
#include "../src/Computer.h"

class AddWithCarryTests : public ::testing::Test {
    void AddTest(int expectedCycles, byte op1, byte op2) {
        byte res = op1 + op2;
        bool flagC = res < op1;
        if (computer.cpu.flag.C) { res++; if (res == 0) flagC = true; }
        bool flagZ = res == 0;
        bool flagN = (res & 0x80) == 0x80;
        bool flagV = ((op1 & 0x80) ^ (op2 & 0x80)) == 0x00 && ((op1 & 0x80) ^ (res & 0x80)) == 0x80;

        // Given:
        computer.cpu.flag.Z = not flagZ;
        computer.cpu.flag.N = not flagN;
        computer.cpu.flag.V = not flagV;
        computer.cpu.A = op1;
        // OTHER MEMORY SET BY CALLER
        const CPU cpuCopy = computer.cpu;

        // When:
        int cyclesExecuted = computer.run(expectedCycles);

        // Then:
        EXPECT_EQ(cyclesExecuted, expectedCycles);
        EXPECT_EQ(computer.cpu.A, res);
        EXPECT_EQ(computer.cpu.flag.C, flagC);
        EXPECT_EQ(computer.cpu.flag.Z, flagZ);
        EXPECT_EQ(computer.cpu.flag.N, flagN);
        EXPECT_EQ(computer.cpu.flag.V, flagV);
        VerifyUnchangedFlags(cpuCopy);
    }
public:
    static const byte UNCHANGED_FLAGS = CPU::FLAG_I | CPU::FLAG_D | CPU::FLAG_B;
    Computer computer;

    void SetUp() override { computer.reset(); }
    void TearDown() override {}

    void VerifyUnchangedFlags(const CPU& copy) const {
        EXPECT_EQ(computer.cpu.status & UNCHANGED_FLAGS, copy.status & UNCHANGED_FLAGS);
    }

    void AddImmTest(byte op1, byte op2, bool flagC) {
        computer.cpu.flag.C = flagC;
        computer.memory[0x1000] = CPU::adcImm;
        computer.memory[0x1001] = op2;
        AddTest(2, op1, op2);
    }

    void AddZpgTest(byte op1, byte op2, byte zpgAddress, bool flagC) {
        computer.cpu.flag.C = flagC;
        computer.memory[0x1000] = CPU::adcZpg;
        computer.memory[0x1001] = zpgAddress;
        computer.memory[zpgAddress] = op2;
        AddTest(3, op1, op2);
    }

    void AddZpXTest(byte op1, byte op2, byte zpgAddress, byte offset, bool flagC) {
        computer.cpu.flag.C = flagC;
        computer.cpu.X = offset;
        computer.memory[0x1000] = CPU::adcZpX;
        computer.memory[0x1001] = zpgAddress;
        word address = (byte) (zpgAddress + offset);
        computer.memory[address] = op2;
        AddTest(4, op1, op2);
    }

    void AddAbsTest(byte op1, byte op2, word address, bool flagC) {
        computer.cpu.flag.C = flagC;
        computer.memory[0x1000] = CPU::adcAbs;
        computer.memory[0x1001] = (byte) address;
        computer.memory[0x1002] = (byte) (address >> 8);
        computer.memory[address] = op2;
        AddTest(4, op1, op2);
    }

    void AddAbXTest(byte op1, byte op2, word address, byte offset, bool flagC) {
        computer.cpu.flag.C = flagC;
        computer.cpu.X = offset;
        computer.memory[0x1000] = CPU::adcAbX;
        computer.memory[0x1001] = (byte) address;
        computer.memory[0x1002] = (byte) (address >> 8);
        int expectedCycles = 4;
        word final = address + offset;
        if ((final & 0x0100) != (address & 0x0100)) expectedCycles++;
        computer.memory[final] = op2;
        AddTest(expectedCycles, op1, op2);
    }

    void AddAbYTest(byte op1, byte op2, word address, byte offset, bool flagC) {
        computer.cpu.flag.C = flagC;
        computer.cpu.Y = offset;
        computer.memory[0x1000] = CPU::adcAbY;
        computer.memory[0x1001] = (byte) address;
        computer.memory[0x1002] = (byte) (address >> 8);
        int expectedCycles = 4;
        word final = address + offset;
        if ((final & 0x0100) != (address & 0x0100)) expectedCycles++;
        computer.memory[final] = op2;
        AddTest(expectedCycles, op1, op2);
    }

    void AddIdXTest(byte op1, byte op2, byte zpgAddress, word address, byte offset, bool flagC) {
        computer.cpu.flag.C = flagC;
        computer.cpu.X = offset;
        computer.memory[0x1000] = CPU::adcIdX;
        computer.memory[0x1001] = zpgAddress;
        zpgAddress += offset;
        computer.memory[zpgAddress] = address;
        computer.memory[zpgAddress + 1] = (byte) (address >> 8);
        computer.memory[address] = op2;
        AddTest(6, op1, op2);
    }

    void AddIdYTest(byte op1, byte op2, byte zpgAddress, word address, byte offset, bool flagC) {
        computer.cpu.flag.C = flagC;
        computer.cpu.Y = offset;
        computer.memory[0x1000] = CPU::adcIdY;
        computer.memory[0x1001] = zpgAddress;
        computer.memory[zpgAddress] = address;
        computer.memory[zpgAddress + 1] = (byte) (address >> 8);
        int expectedCycles = 5;
        word final = address + offset;
        if ((final & 0x0100) != (address & 0x0100)) expectedCycles++;
        computer.memory[final] = op2;
        AddTest(expectedCycles, op1, op2);
    }
};

// ================== //
//       adcImm       //
// ================== //

TEST_F(AddWithCarryTests, adcImm_CanAddZeroandZero_SetZeroFlag) { AddImmTest(0x00, 0x00, false); }
TEST_F(AddWithCarryTests, adcImm_CanAddAccumulatorWithZero) { AddImmTest(0x35, 0x00, false);}
TEST_F(AddWithCarryTests, adcImm_CanAddZeroWithNumber) { AddImmTest(0x00, 0x35, false); }
TEST_F(AddWithCarryTests, adcImm_CanAddTwoNumbers) { AddImmTest(0x12, 0x23, false); }
TEST_F(AddWithCarryTests, adcImm_CanAddTwoNumbers_SetNegativeFlag) { AddImmTest(0x12, 0x83, false); }
TEST_F(AddWithCarryTests, adcImm_CanAddTwoNumbers_SetOverflowFlag) { AddImmTest(0x32, 0x63, false); }
TEST_F(AddWithCarryTests, adcImm_CanAddTwoNumbers_SetCarryFlag) { AddImmTest(0x82, 0x83, false); }
TEST_F(AddWithCarryTests, adcImm_CanAddZeroandZero_SetZeroFlag_WithCarryFlag) { AddImmTest(0x00, 0x00, true); }
TEST_F(AddWithCarryTests, adcImm_CanAddAccumulatorWithZero_WithCarryFlag) { AddImmTest(0x35, 0x00, true);}
TEST_F(AddWithCarryTests, adcImm_CanAddZeroWithNumber_WithCarryFlag) { AddImmTest(0x00, 0x35, true); }
TEST_F(AddWithCarryTests, adcImm_CanAddTwoNumbers_WithCarryFlag) { AddImmTest(0x12, 0x23, true); }
TEST_F(AddWithCarryTests, adcImm_CanAddTwoNumbers_SetNegativeFlag_WithCarryFlag) { AddImmTest(0x12, 0x83, true); }
TEST_F(AddWithCarryTests, adcImm_CanAddTwoNumbers_SetOverflowFlag_WithCarryFlag) { AddImmTest(0x32, 0x63, true); }
TEST_F(AddWithCarryTests, adcImm_CanAddTwoNumbers_SetCarryFlag_WithCarryFlag) { AddImmTest(0x82, 0x83, true); }

// ================== //
//       adcZpg       //
// ================== //

TEST_F(AddWithCarryTests, adcZpg_CanAddZeroandZero_SetZeroFlag) { AddZpgTest(0x00, 0x00, 0x80, false); }
TEST_F(AddWithCarryTests, adcZpg_CanAddAccumulatorWithZero) { AddZpgTest(0x35, 0x00, 0x80, false);}
TEST_F(AddWithCarryTests, adcZpg_CanAddZeroWithNumber) { AddZpgTest(0x00, 0x35, 0x80, false); }
TEST_F(AddWithCarryTests, adcZpg_CanAddTwoNumbers) { AddZpgTest(0x12, 0x23, 0x80, false); }
TEST_F(AddWithCarryTests, adcZpg_CanAddTwoNumbers_SetNegativeFlag) { AddZpgTest(0x12, 0x83, 0x80, false); }
TEST_F(AddWithCarryTests, adcZpg_CanAddTwoNumbers_SetOverflowFlag) { AddZpgTest(0x32, 0x63, 0x80, false); }
TEST_F(AddWithCarryTests, adcZpg_CanAddTwoNumbers_SetCarryFlag) { AddZpgTest(0x82, 0x83, 0x80, false); }
TEST_F(AddWithCarryTests, adcZpg_CanAddZeroandZero_SetZeroFlag_WithCarryFlag) { AddZpgTest(0x00, 0x00, 0x80, true); }
TEST_F(AddWithCarryTests, adcZpg_CanAddAccumulatorWithZero_WithCarryFlag) { AddZpgTest(0x35, 0x00, 0x80, true);}
TEST_F(AddWithCarryTests, adcZpg_CanAddZeroWithNumber_WithCarryFlag) { AddZpgTest(0x00, 0x35, 0x80, true); }
TEST_F(AddWithCarryTests, adcZpg_CanAddTwoNumbers_WithCarryFlag) { AddZpgTest(0x12, 0x23, 0x80, true); }
TEST_F(AddWithCarryTests, adcZpg_CanAddTwoNumbers_SetNegativeFlag_WithCarryFlag) { AddZpgTest(0x12, 0x83, 0x80, true); }
TEST_F(AddWithCarryTests, adcZpg_CanAddTwoNumbers_SetOverflowFlag_WithCarryFlag) { AddZpgTest(0x32, 0x63, 0x80, true); }
TEST_F(AddWithCarryTests, adcZpg_CanAddTwoNumbers_SetCarryFlag_WithCarryFlag) { AddZpgTest(0x82, 0x83, 0x80, true); }

// ================== //
//       adcZpX       //
// ================== //

TEST_F(AddWithCarryTests, adcZpX_CanAddZeroandZero_SetZeroFlag) { AddZpXTest(0x00, 0x00, 0x80, 0x02, false); }
TEST_F(AddWithCarryTests, adcZpX_CanAddAccumulatorWithZero) { AddZpXTest(0x35, 0x00, 0x80, 0x02, false);}
TEST_F(AddWithCarryTests, adcZpX_CanAddZeroWithNumber) { AddZpXTest(0x00, 0x35, 0x80, 0x02, false); }
TEST_F(AddWithCarryTests, adcZpX_CanAddTwoNumbers) { AddZpXTest(0x12, 0x23, 0x80, 0x02, false); }
TEST_F(AddWithCarryTests, adcZpX_CanAddTwoNumbers_SetNegativeFlag) { AddZpXTest(0x12, 0x83, 0x80, 0x02, false); }
TEST_F(AddWithCarryTests, adcZpX_CanAddTwoNumbers_SetOverflowFlag) { AddZpXTest(0x32, 0x63, 0x80, 0x02, false); }
TEST_F(AddWithCarryTests, adcZpX_CanAddTwoNumbers_SetCarryFlag) { AddZpXTest(0x82, 0x83, 0x80, 0x02, false); }
TEST_F(AddWithCarryTests, adcZpX_CanAddTwoNumbers_Wraps) { AddZpXTest(0x82, 0x83, 0x84, 0x86, false); }
TEST_F(AddWithCarryTests, adcZpX_CanAddZeroandZero_SetZeroFlag_WithCarryFlag) { AddZpXTest(0x00, 0x00, 0x80, 0x02, true); }
TEST_F(AddWithCarryTests, adcZpX_CanAddAccumulatorWithZero_WithCarryFlag) { AddZpXTest(0x35, 0x00, 0x80, 0x02, true);}
TEST_F(AddWithCarryTests, adcZpX_CanAddZeroWithNumber_WithCarryFlag) { AddZpXTest(0x00, 0x35, 0x80, 0x02, true); }
TEST_F(AddWithCarryTests, adcZpX_CanAddTwoNumbers_WithCarryFlag) { AddZpXTest(0x12, 0x23, 0x80, 0x02, true); }
TEST_F(AddWithCarryTests, adcZpX_CanAddTwoNumbers_SetNegativeFlag_WithCarryFlag) { AddZpXTest(0x12, 0x83, 0x80, 0x02, true); }
TEST_F(AddWithCarryTests, adcZpX_CanAddTwoNumbers_SetOverflowFlag_WithCarryFlag) { AddZpXTest(0x32, 0x63, 0x80, 0x02, true); }
TEST_F(AddWithCarryTests, adcZpX_CanAddTwoNumbers_SetCarryFlag_WithCarryFlag) { AddZpXTest(0x82, 0x83, 0x80, 0x02, true); }
TEST_F(AddWithCarryTests, adcZpX_CanAddTwoNumbers_Wraps_WithCarryFlag) { AddZpXTest(0x82, 0x83, 0x84, 0x86, true); }

// ================== //
//       adcAbs       //
// ================== //

TEST_F(AddWithCarryTests, adcAbs_CanAddZeroandZero_SetZeroFlag) { AddAbsTest(0x00, 0x00, 0x3010, false); }
TEST_F(AddWithCarryTests, adcAbs_CanAddAccumulatorWithZero) { AddAbsTest(0x35, 0x00, 0x3010, false);}
TEST_F(AddWithCarryTests, adcAbs_CanAddZeroWithNumber) { AddAbsTest(0x00, 0x35, 0x3010, false); }
TEST_F(AddWithCarryTests, adcAbs_CanAddTwoNumbers) { AddAbsTest(0x12, 0x23, 0x3010, false); }
TEST_F(AddWithCarryTests, adcAbs_CanAddTwoNumbers_SetNegativeFlag) { AddAbsTest(0x12, 0x83, 0x3010, false); }
TEST_F(AddWithCarryTests, adcAbs_CanAddTwoNumbers_SetOverflowFlag) { AddAbsTest(0x32, 0x63, 0x3010, false); }
TEST_F(AddWithCarryTests, adcAbs_CanAddTwoNumbers_SetCarryFlag) { AddAbsTest(0x82, 0x83, 0x3010, false); }
TEST_F(AddWithCarryTests, adcAbs_CanAddZeroandZero_SetZeroFlag_WithCarryFlag) { AddAbsTest(0x00, 0x00, 0x3010, true); }
TEST_F(AddWithCarryTests, adcAbs_CanAddAccumulatorWithZero_WithCarryFlag) { AddAbsTest(0x35, 0x00, 0x3010, true);}
TEST_F(AddWithCarryTests, adcAbs_CanAddZeroWithNumber_WithCarryFlag) { AddAbsTest(0x00, 0x35, 0x3010, true); }
TEST_F(AddWithCarryTests, adcAbs_CanAddTwoNumbers_WithCarryFlag) { AddAbsTest(0x12, 0x23, 0x3010, true); }
TEST_F(AddWithCarryTests, adcAbs_CanAddTwoNumbers_SetNegativeFlag_WithCarryFlag) { AddAbsTest(0x12, 0x83, 0x3010, true); }
TEST_F(AddWithCarryTests, adcAbs_CanAddTwoNumbers_SetOverflowFlag_WithCarryFlag) { AddAbsTest(0x32, 0x63, 0x3010, true); }
TEST_F(AddWithCarryTests, adcAbs_CanAddTwoNumbers_SetCarryFlag_WithCarryFlag) { AddAbsTest(0x82, 0x83, 0x3010, true); }

// ================== //
//       adcAbX       //
// ================== //

TEST_F(AddWithCarryTests, adcAbX_CanAddZeroandZero_SetZeroFlag) { AddAbXTest(0x00, 0x00, 0x3010, 0x02, false); }
TEST_F(AddWithCarryTests, adcAbX_CanAddAccumulatorWithZero) { AddAbXTest(0x35, 0x00, 0x3010, 0x02, false);}
TEST_F(AddWithCarryTests, adcAbX_CanAddZeroWithNumber) { AddAbXTest(0x00, 0x35, 0x3010, 0x02, false); }
TEST_F(AddWithCarryTests, adcAbX_CanAddTwoNumbers) { AddAbXTest(0x12, 0x23, 0x3010, 0x02, false); }
TEST_F(AddWithCarryTests, adcAbX_CanAddTwoNumbers_SetNegativeFlag) { AddAbXTest(0x12, 0x83, 0x3010, 0x02, false); }
TEST_F(AddWithCarryTests, adcAbX_CanAddTwoNumbers_SetOverflowFlag) { AddAbXTest(0x32, 0x63, 0x3010, 0x02, false); }
TEST_F(AddWithCarryTests, adcAbX_CanAddTwoNumbers_SetCarryFlag) { AddAbXTest(0x82, 0x83, 0x3010, 0x02, false); }
TEST_F(AddWithCarryTests, adcAbX_CanAddTwoNumbers_CrossingPageBoundary) { AddAbXTest(0x82, 0x83, 0x3084, 0x86, false); }
TEST_F(AddWithCarryTests, adcAbX_CanAddZeroandZero_SetZeroFlag_WithCarryFlag) { AddAbXTest(0x00, 0x00, 0x3010, 0x02, true); }
TEST_F(AddWithCarryTests, adcAbX_CanAddAccumulatorWithZero_WithCarryFlag) { AddAbXTest(0x35, 0x00, 0x3010, 0x02, true);}
TEST_F(AddWithCarryTests, adcAbX_CanAddZeroWithNumber_WithCarryFlag) { AddAbXTest(0x00, 0x35, 0x3010, 0x02, true); }
TEST_F(AddWithCarryTests, adcAbX_CanAddTwoNumbers_WithCarryFlag) { AddAbXTest(0x12, 0x23, 0x3010, 0x02, true); }
TEST_F(AddWithCarryTests, adcAbX_CanAddTwoNumbers_SetNegativeFlag_WithCarryFlag) { AddAbXTest(0x12, 0x83, 0x3010, 0x02, true); }
TEST_F(AddWithCarryTests, adcAbX_CanAddTwoNumbers_SetOverflowFlag_WithCarryFlag) { AddAbXTest(0x32, 0x63, 0x3010, 0x02, true); }
TEST_F(AddWithCarryTests, adcAbX_CanAddTwoNumbers_SetCarryFlag_WithCarryFlag) { AddAbXTest(0x82, 0x83, 0x3010, 0x02, true); }
TEST_F(AddWithCarryTests, adcAbX_CanAddTwoNumbers_CrossingPageBoundary_WithCarryFlag) { AddAbXTest(0x82, 0x83, 0x3084, 0x86, true); }

// ================== //
//       adcAbY       //
// ================== //

TEST_F(AddWithCarryTests, adcAbY_CanAddZeroandZero_SetZeroFlag) { AddAbYTest(0x00, 0x00, 0x3010, 0x02, false); }
TEST_F(AddWithCarryTests, adcAbY_CanAddAccumulatorWithZero) { AddAbYTest(0x35, 0x00, 0x3010, 0x02, false);}
TEST_F(AddWithCarryTests, adcAbY_CanAddZeroWithNumber) { AddAbYTest(0x00, 0x35, 0x3010, 0x02, false); }
TEST_F(AddWithCarryTests, adcAbY_CanAddTwoNumbers) { AddAbYTest(0x12, 0x23, 0x3010, 0x02, false); }
TEST_F(AddWithCarryTests, adcAbY_CanAddTwoNumbers_SetNegativeFlag) { AddAbYTest(0x12, 0x83, 0x3010, 0x02, false); }
TEST_F(AddWithCarryTests, adcAbY_CanAddTwoNumbers_SetOverflowFlag) { AddAbYTest(0x32, 0x63, 0x3010, 0x02, false); }
TEST_F(AddWithCarryTests, adcAbY_CanAddTwoNumbers_SetCarryFlag) { AddAbYTest(0x82, 0x83, 0x3010, 0x02, false); }
TEST_F(AddWithCarryTests, adcAbY_CanAddTwoNumbers_CrossingPageBoundary) { AddAbYTest(0x82, 0x83, 0x3084, 0x86, false); }
TEST_F(AddWithCarryTests, adcAbY_CanAddZeroandZero_SetZeroFlag_WithCarryFlag) { AddAbYTest(0x00, 0x00, 0x3010, 0x02, true); }
TEST_F(AddWithCarryTests, adcAbY_CanAddAccumulatorWithZero_WithCarryFlag) { AddAbYTest(0x35, 0x00, 0x3010, 0x02, true);}
TEST_F(AddWithCarryTests, adcAbY_CanAddZeroWithNumber_WithCarryFlag) { AddAbYTest(0x00, 0x35, 0x3010, 0x02, true); }
TEST_F(AddWithCarryTests, adcAbY_CanAddTwoNumbers_WithCarryFlag) { AddAbYTest(0x12, 0x23, 0x3010, 0x02, true); }
TEST_F(AddWithCarryTests, adcAbY_CanAddTwoNumbers_SetNegativeFlag_WithCarryFlag) { AddAbYTest(0x12, 0x83, 0x3010, 0x02, true); }
TEST_F(AddWithCarryTests, adcAbY_CanAddTwoNumbers_SetOverflowFlag_WithCarryFlag) { AddAbYTest(0x32, 0x63, 0x3010, 0x02, true); }
TEST_F(AddWithCarryTests, adcAbY_CanAddTwoNumbers_SetCarryFlag_WithCarryFlag) { AddAbYTest(0x82, 0x83, 0x3010, 0x02, true); }
TEST_F(AddWithCarryTests, adcAbY_CanAddTwoNumbers_CrossingPageBoundary_WithCarryFlag) { AddAbYTest(0x82, 0x83, 0x3084, 0x86, true); }

// ================== //
//       adcIdX       //
// ================== //

TEST_F(AddWithCarryTests, adcIdX_CanAddZeroandZero_SetZeroFlag) { AddIdXTest(0x00, 0x00, 0x80, 0x3010, 0x02, false); }
TEST_F(AddWithCarryTests, adcIdX_CanAddAccumulatorWithZero) { AddIdXTest(0x35, 0x00, 0x80, 0x3010, 0x02, false);}
TEST_F(AddWithCarryTests, adcIdX_CanAddZeroWithNumber) { AddIdXTest(0x00, 0x35, 0x80, 0x3010, 0x02, false); }
TEST_F(AddWithCarryTests, adcIdX_CanAddTwoNumbers) { AddIdXTest(0x12, 0x23, 0x80, 0x3010, 0x02, false); }
TEST_F(AddWithCarryTests, adcIdX_CanAddTwoNumbers_SetNegativeFlag) { AddIdXTest(0x12, 0x83, 0x80, 0x3010, 0x02, false); }
TEST_F(AddWithCarryTests, adcIdX_CanAddTwoNumbers_SetOverflowFlag) { AddIdXTest(0x32, 0x63, 0x80, 0x3010, 0x02, false); }
TEST_F(AddWithCarryTests, adcIdX_CanAddTwoNumbers_SetCarryFlag) { AddIdXTest(0x82, 0x83, 0x80, 0x3010, 0x02, false); }
TEST_F(AddWithCarryTests, adcIdX_CanAddTwoNumbers_Wraps) { AddIdXTest(0x82, 0x83, 0x84, 0x3010, 0x86, false); }
TEST_F(AddWithCarryTests, adcIdX_CanAddZeroandZero_SetZeroFlag_WithCarryFlag) { AddIdXTest(0x00, 0x00, 0x80, 0x3010, 0x02, true); }
TEST_F(AddWithCarryTests, adcIdX_CanAddAccumulatorWithZero_WithCarryFlag) { AddIdXTest(0x35, 0x00, 0x80, 0x3010, 0x02, true);}
TEST_F(AddWithCarryTests, adcIdX_CanAddZeroWithNumber_WithCarryFlag) { AddIdXTest(0x00, 0x35, 0x80, 0x3010, 0x02, true); }
TEST_F(AddWithCarryTests, adcIdX_CanAddTwoNumbers_WithCarryFlag) { AddIdXTest(0x12, 0x23, 0x80, 0x3010, 0x02, true); }
TEST_F(AddWithCarryTests, adcIdX_CanAddTwoNumbers_SetNegativeFlag_WithCarryFlag) { AddIdXTest(0x12, 0x83, 0x80, 0x3010, 0x02, true); }
TEST_F(AddWithCarryTests, adcIdX_CanAddTwoNumbers_SetOverflowFlag_WithCarryFlag) { AddIdXTest(0x32, 0x63, 0x80, 0x3010, 0x02, true); }
TEST_F(AddWithCarryTests, adcIdX_CanAddTwoNumbers_SetCarryFlag_WithCarryFlag) { AddIdXTest(0x82, 0x83, 0x80, 0x3010, 0x02, true); }
TEST_F(AddWithCarryTests, adcIdX_CanAddTwoNumbers_Wraps_WithCarryFlag) { AddIdXTest(0x82, 0x83, 0x84, 0x3010, 0x86, true); }

// ================== //
//       adcIdY       //
// ================== //

TEST_F(AddWithCarryTests, adcIdY_CanAddZeroandZero_SetZeroFlag) { AddIdYTest(0x00, 0x00, 0x80, 0x3010, 0x02, false); }
TEST_F(AddWithCarryTests, adcIdY_CanAddAccumulatorWithZero) { AddIdYTest(0x35, 0x00, 0x80, 0x3010, 0x02, false);}
TEST_F(AddWithCarryTests, adcIdY_CanAddZeroWithNumber) { AddIdYTest(0x00, 0x35, 0x80, 0x3010, 0x02, false); }
TEST_F(AddWithCarryTests, adcIdY_CanAddTwoNumbers) { AddIdYTest(0x12, 0x23, 0x80, 0x3010, 0x02, false); }
TEST_F(AddWithCarryTests, adcIdY_CanAddTwoNumbers_SetNegativeFlag) { AddIdYTest(0x12, 0x83, 0x80, 0x3010, 0x02, false); }
TEST_F(AddWithCarryTests, adcIdY_CanAddTwoNumbers_SetOverflowFlag) { AddIdYTest(0x32, 0x63, 0x80, 0x3010, 0x02, false); }
TEST_F(AddWithCarryTests, adcIdY_CanAddTwoNumbers_SetCarryFlag) { AddIdYTest(0x82, 0x83, 0x80, 0x3010, 0x02, false); }
TEST_F(AddWithCarryTests, adcIdY_CanAddTwoNumbers_CrossingPageBoundary) { AddIdYTest(0x82, 0x83, 0x80, 0x3084, 0x86, false); }
TEST_F(AddWithCarryTests, adcIdY_CanAddZeroandZero_SetZeroFlag_WithCarryFlag) { AddIdYTest(0x00, 0x00, 0x80, 0x3010, 0x02, true); }
TEST_F(AddWithCarryTests, adcIdY_CanAddAccumulatorWithZero_WithCarryFlag) { AddIdYTest(0x35, 0x00, 0x80, 0x3010, 0x02, true);}
TEST_F(AddWithCarryTests, adcIdY_CanAddZeroWithNumber_WithCarryFlag) { AddIdYTest(0x00, 0x35, 0x80, 0x3010, 0x02, true); }
TEST_F(AddWithCarryTests, adcIdY_CanAddTwoNumbers_WithCarryFlag) { AddIdYTest(0x12, 0x23, 0x80, 0x3010, 0x02, true); }
TEST_F(AddWithCarryTests, adcIdY_CanAddTwoNumbers_SetNegativeFlag_WithCarryFlag) { AddIdYTest(0x12, 0x83, 0x80, 0x3010, 0x02, true); }
TEST_F(AddWithCarryTests, adcIdY_CanAddTwoNumbers_SetOverflowFlag_WithCarryFlag) { AddIdYTest(0x32, 0x63, 0x80, 0x3010, 0x02, true); }
TEST_F(AddWithCarryTests, adcIdY_CanAddTwoNumbers_SetCarryFlag_WithCarryFlag) { AddIdYTest(0x82, 0x83, 0x80, 0x3010, 0x02, true); }
TEST_F(AddWithCarryTests, adcIdY_CanAddTwoNumbers_CrossingPageBoundary_WithCarryFlag) { AddIdYTest(0x82, 0x83, 0x80, 0x3084, 0x86, true); }
