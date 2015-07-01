///////////////////////////////////////////////////////////////////////////////
// cpu.h
//
// Header file containing structure of the 6502 cpu
//
// Author: Alex Busman
// Date: July 1, 2015
///////////////////////////////////////////////////////////////////////////////
#ifndef CPU_H
#define CPU_H

#include <cstdint>

#define MEM_SIZE 65536

class Cpu {
public:
  Cpu() : _a(0), _x(0), _y(0), _sp(0), _pc(0), _interrupt(false) { clearMem(); }
  ~Cpu() {}
  void setA(uint8_t a) { _a = a; }
  void setX(uint8_t x) { _x = x; }
  void setY(uint8_t y) { _y = y; }
  void setSP(uint8_t sp) { _sp = sp; }
  void setPC(uint8_t pc) { _pc = pc; }
  uint8_t a() const { return _a; }
  uint8_t x() const { return _x; }
  uint8_t y() const { return _y; }
  uint8_t sp() const { return _sp; }
  uint16_t pc() const { return _pc; }
  bool interruptWaiting() const { return _interrupt };
  uint8_t getMemByte(uint16_t address) const { return _mem[address];}
  void clearInterruptWaiting() { _interrupt = false; }
  void setInterruptWaiting() { _interrupt = true; }
  void clearMem() {
    for (uint16_t i = 0; i < MEM_SIZE; i++)
      _mem[i] = 0;
  }
  void incPC() { _pc++; }


private:
  // Accumulator
  uint8_t _a;
  // Index registers
  uint8_t _x, _y;
  // Stack pointer
  uint8_t _sp;
  // Program Counter
  uint16_t _pc;
  // Status register
  uint8_t _sr;
  // Interrupt waiting
  bool _interrupt;
  // Memory map
  uint8_t _mem[MEM_SIZE];
};
#endif // CPU_H
