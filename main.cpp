#include <iostream>
#include <cstdint>

#include "cpu.h"
#include "instruction.h"

class LDA_IMM : public Instruction {
public:
  using Instruction::Instruction;
  uint8_t operation(uint16_t operand) {
    _cpu->setA(static_cast<uint8_t>(operand));
    return 0;
  }
};

int main(int argc, char *argv[]) {
  Cpu cpu;
  LDA_IMM lda_imm(&cpu, IMM, "lda", 0xA9, 2);
  lda_imm.operation(80);
  cout << (int)cpu.A() << endl;
}
