#include <iostream>
#include <cstdint>

#include "cpu.h"
#include "operation.h"

int main(int argc, char *argv[]) {
  Cpu cpu;
  LDA_IMM lda_imm(&cpu, IMM, "lda", 0xA9, 2);
  lda_imm.operation(80);
  cout << (int)cpu.A() << endl;
}
