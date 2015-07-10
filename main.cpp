#include <iostream>
#include <cstdint>

#include "cpu.h"
#include "operations.h"

int main(int argc, char *argv[]) {
  Cpu cpu;
  cpu.setA(25);
  ASL(&cpu, 0, ACC);
  STA(&cpu, 0, ZER);
  ASL(&cpu, 0, ACC);
  ASL(&cpu, 0, ACC);
  CLC(&cpu, 0, IMP);
  ADC(&cpu, 0, ZER);
  std::cout << (int)cpu.A() << std::endl;
}
