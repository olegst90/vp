/* Instruction set

1st byte:
  3 bits - flags
  5 bits - opcode

2nd/3rd byte: operand (optional)

*/

#define CPU_DEFAULT_OPCODE(C) ((C) & 0x1F)
#define CPU_DEFAULT_FLAGS(C) (((C) & ~0x1F) >> 5)

enum cpu_default_instruction {
  CPU_DEFAULT_LDA,
  CPU_DEFAULT_STA,
  CPU_DEFAULT_ADD,
  CPU_DEFAULT_SUB,
  CPU_DEFAULT_DIV,
  CPU_DEFAULT_MUL,
  CPU_DEFAULT_TEST,
  CPU_DEFAULT_JMP, //imm8
  CPU_DEFAULT_JMPF, //imm16
  CPU_DEFAULT_JMPR, //mem[I]
  CPU_DEFAULT_NOP,
  CPU_DEFAULT_NONE = 0xffffffff
};

enum cpu_default_jmp_flavor {
  JMP_N,JMP_Z,JMP_C
};

#define CPU_DEFAULT_OPFLAG_IMM16 0x1

// test gave zero
#define CPU_DEFAULT_FLAG_Z 0x1
// negative
#define CPU_DEFAULT_FLAG_N 0x2
// integer overflow
#define CPU_DEFAULT_FLAG_C 0x4
