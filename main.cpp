#include <iostream>
#include <cstdint>

#include "cpu.h"
#include "operations.h"

int main(int argc, char *argv[]) {
  Cpu cpu;
  cpu.setA(20);
  ADC(&cpu, static_cast<uint8_t>(20), IMM);
  std::cout << (int)cpu.A() << std::endl;
}
