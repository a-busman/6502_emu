#include <iostream>
#include <cstdint>

#include "cpu.h"
#include "operations.h"

int main(int argc, char *argv[]) {
  Cpu cpu;
  cpu.setA(25);
  ASL(&cpu);
  STA(&cpu, 0, ZER);
  ASL(&cpu);
  ASL(&cpu);
  CLC(&cpu);
  ADC(&cpu, 0, ZER);
  std::cout << (int)cpu.A() << std::endl;
}
