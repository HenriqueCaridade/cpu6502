
#include "gtest/gtest.h"
#include "../src/Computer.h"

class SubWithCarryTests : public ::testing::Test {
public:
    static const byte UNCHANGED_FLAGS = CPU::FLAG_I | CPU::FLAG_D;
    Computer computer;

    void SetUp() override { computer.reset(); }
    void TearDown() override {}

    void VerifyUnchangedFlags(const CPU& copy) const {
        EXPECT_EQ(computer.cpu.status & UNCHANGED_FLAGS, copy.status & UNCHANGED_FLAGS);
    }
};

// ================== //
//       sbcImm       //
// ================== //
