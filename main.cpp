#include <iostream>
#include <cstdint>

#include "cpu.h"

int main(int argc, char *argv[]) {
  Cpu cpu;
  cpu.setA(25);
  cpu.ASL(0, ACC);
  cpu.STA(0, ZER);
  cpu.ASL(0, ACC);
  cpu.ASL(0, ACC);
  cpu.CLC(0, IMP);
  cpu.ADC(0, ZER);
  std::cout << (int)cpu.A() << std::endl;
}
