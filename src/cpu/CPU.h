
#ifndef CPU6502_CPU_H
#define CPU6502_CPU_H

#include "../memory/Memory.h"
#include "../types.h"

// https://www.masswerk.at/6502/6502_instruction_set.html

class CPU {
    static bool isNeg(byte value);
    static bool isZero(byte value);
    static word addOffsetWithPageBoundary(word address, byte offset, int& cycles);
    static word addRelativeOffsetWithPageBoundary(word address, sbyte offset, int& cycles);
    void setAssignmentFlags(byte reg);
public:
    static const byte STATUS_MASK = 0b11011111;
    static const word RESET_ADRESS = 0xFFFC;
    union {
        byte status = 0x00; /// Status Register [NV-BDIZC]
        struct {
            byte C : 1; /// Carry
            byte Z : 1; /// Zero
            byte I : 1; /// Interrupt (IRQ disable)
            byte D : 1; /// Decimal (use BCD for arithmetics)
            byte B : 1; /// Break
            byte   : 1; /// 5th bit Unused
            byte V : 1; /// Overflow
            byte N : 1; /// Negative
        } flag;
    };
    word PC; /// Program Counter
    union {
        byte SP; /// Stack Pointer
        word _SPaddress = 0x1FF;
    };
    byte A = 0x00;  /// Accumulator
    byte X = 0x00;  /// X register
    byte Y = 0x00;  /// Y register

    /// @brief Instructions
    enum Instruction {
        // LDA
        ldaImm = 0xA9,
        ldaZpg = 0xA5,
        ldaZpX = 0xB5,
        ldaAbs = 0xAD,
        ldaAbX = 0xBD,
        ldaAbY = 0xB9,
        ldaIdX = 0xA1,
        ldaIdY = 0xB1,
        // LDX
        ldxImm = 0xA2,
        ldxZpg = 0xA6,
        ldxZpY = 0xB6,
        ldxAbs = 0xAE,
        ldxAbY = 0xBE,
        // LDY
        ldyImm = 0xA0,
        ldyZpg = 0xA4,
        ldyZpX = 0xB4,
        ldyAbs = 0xAC,
        ldyAbX = 0xBC,
        // STA
        staZpg = 0x85,
        staZpX = 0x95,
        staAbs = 0x8D,
        staAbX = 0x9D,
        staAbY = 0x99,
        staIdX = 0x81,
        staIdY = 0x91,
        // STX
        stxZpg = 0x86,
        stxZpY = 0x96,
        stxAbs = 0x8E,
        // STY
        styZpg = 0x84,
        styZpX = 0x94,
        styAbs = 0x8C,
        // AND
        andImm = 0x29,
        andZpg = 0x25,
        andZpX = 0x35,
        andAbs = 0x2D,
        andAbX = 0x3D,
        andAbY = 0x39,
        andIdX = 0x21,
        andIdY = 0x31,
        // EOR
        eorImm = 0x49,
        eorZpg = 0x45,
        eorZpX = 0x55,
        eorAbs = 0x4D,
        eorAbX = 0x5D,
        eorAbY = 0x59,
        eorIdX = 0x41,
        eorIdY = 0x51,
        // ORA
        oraImm = 0x09,
        oraZpg = 0x05,
        oraZpX = 0x15,
        oraAbs = 0x0D,
        oraAbX = 0x1D,
        oraAbY = 0x19,
        oraIdX = 0x01,
        oraIdY = 0x11,
        // BIT
        bitZpg = 0x24,
        bitAbs = 0x2C,
        // Transfer
        taxImp = 0xAA,
        txaImp = 0x8A,
        tayImp = 0xA8,
        tyaImp = 0x98,
        tsxImp = 0xBA,
        txsImp = 0x9A,
        // Stack
        phaImp = 0x48,
        plaImp = 0x68,
        phpImp = 0x08,
        plpImp = 0x28,
        // Increments
        incZpg = 0xE6,
        incZpX = 0xF6,
        incAbs = 0xEE,
        incAbX = 0xFE,
        inxImp = 0xE8,
        inyImp = 0xC8,
        // Decrements
        decZpg = 0xC6,
        decZpX = 0xD6,
        decAbs = 0xCE,
        decAbX = 0xDE,
        dexImp = 0xCA,
        deyImp = 0x88,
        // Branches
        bccRel = 0x90,
        bcsRel = 0xB0,
        beqRel = 0xF0,
        bmiRel = 0x30,
        bneRel = 0xD0,
        bplRel = 0x10,
        bvcRel = 0x50,
        bvsRel = 0x70,
        // Jump & Calls
        jmpAbs = 0x4C,
        jmpInd = 0x6C,
        jsrAbs = 0x20,
        rtsImp = 0x60,
        // No Operation
        nop = 0xEA,
    };
    /// @brief Default Constructor
    CPU();

    /// @brief Constructor
    CPU(const Memory& memory);

    /// @brief Resets the cpu to the initial state.
    void reset(const Memory& memory);

    /// @brief Resets the PC to the reset vector.
    void resetPC(const Memory& memory);

    /** @brief Execute the number of cycles given.
     *
     *  @return Cycles Executed
     */
    int execute(int cycles, Memory& memory);

    /** @brief Fetch Instruction from Program Counter Address
     * - used for instruction fetching
     *
     * Consumes 1 cycle
     */
    Instruction fetchInstruction(int& cycles, const Memory& memory);

    /** @brief Fetch Byte from Program Counter Address
     *  - used for instruction data fetching
     *
     *  Consumes 1 cycle
     */
    byte fetchByte(int& cycles, const Memory& memory);

    /** @brief Fetch Word from Program Counter Address
     *  - used for instruction data fetching
     *
     *  Consumes 2 cycles
     */
    word fetchWord(int& cycles, const Memory& memory);

    /** @brief Read Byte from Full Address
     *
     *  Consumes 1 cycle
     */
    static byte readByte(int& cycles, const Memory& memory, word address);

    /** @brief Read Word from Full Address
     *
     *  Consumes 2 cycles
     */
    static word readWord(int& cycles, const Memory& memory, word address);

    /** @brief Write Byte to Full Address
     *
     *  Consumes 1 cycle
     */
    static void writeByte(byte value, int &cycles, Memory &memory, word address);

    /** @brief Write Word to Full Address
     *
     *  Consumes 2 cycles
     */
    static void writeWord(word value, int &cycles, Memory &memory, word address);

    /** @brief Pushes byte given onto the Stack and decrements Stack Pointer
     *
     *  Consumes 1 cycle
     */
    void stackPushByte(byte value, int &cycles, Memory &memory);

    /** @brief Pushes word given onto the Stack and decrements Stack Pointer accordingly
     *
     *  Consumes 2 cycle
     */
    void stackPushWord(word value, int &cycles, Memory &memory);

    /** @brief Pulls byte given from the Stack and increments Stack Pointer
     *
     *  Consumes 1 cycle
     */
    byte stackPullByte(int &cycles, Memory &memory);

    /** @brief Pulls word given from the Stack and increments Stack Pointer accordingly
     *
     *  Consumes 2 cycle
     */
    word stackPullWord(int &cycles, Memory &memory);

    /** @brief Copies Address to Program Counter
     */
    void jumpTo(word address);

    /** @brief Addressing Mode - Zero Page Address
     *
     *  Consumes 1 cycle
     *  @return Address
     */
    word zeroPageAddress(int& cycles, const Memory& memory);

    /** @brief Addressing Mode - Zero Page Address with Offset
     *
     *  Consumes 2 cycles
     *  @return Address
     */
    word zeroPageAddress(int& cycles, const Memory& memory, byte offset);

    /** @brief Addressing Mode - Absolute Address
     *
     *  Consumes 2 cycles
     *  @return Address
     */
    word absoluteAddress(int& cycles, const Memory& memory);

    /** @brief Addressing Mode - Absolute Address with Offset
     *
     *  Consumes 2 or 3 cycles
     *  @return Address
     */
    word absoluteAddress(int& cycles, const Memory& memory, byte offset);

    /** @brief Addressing Mode - Absolute Address with Offset Fixed Cycles
     *  Obs: Always consume the "Page Boundary Crossing" Cycle
     *
     *  Consumes 3 cycles
     *  @return Address
     */
    word absoluteAddressFixed(int& cycles, const Memory& memory, byte offset);

    /** @brief Addressing Mode - Indirect Address
     *
     *  Consumes 4 cycles
     *  @return Address
     */
    word indirectAddress(int& cycles, const Memory& memory);

    /** @brief Addressing Mode - Pre-Indexed Indirect Address
     *
     *  Consumes 4 cycles
     *  @return Address
     */
    word indirectPreAddress(int& cycles, const Memory& memory, byte offset);

    /** @brief Addressing Mode - Post-Indexed Indirect Address
     *
     *  Consumes 4 or 5 cycles
     *  @return Address
     */
    word indirectPostAddress(int& cycles, const Memory& memory, byte offset);

    /** @brief Addressing Mode - Post-Indexed Indirect Address Fixed Cycles
     *  Obs: Always consume the "Page Boundary Crossing" Cycle
     *
     *  Consumes 5 cycles
     *  @return Address
     */
    word indirectPostAddressFixed(int& cycles, const Memory& memory, byte offset);
};


#endif //CPU6502_CPU_H
