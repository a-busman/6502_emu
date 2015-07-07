///////////////////////////////////////////////////////////////////////////////
// cpu.h
//
// Header file containing structure of the 6502 cpu
//
// Author: Alex Busman
// Date: July 7, 2015
///////////////////////////////////////////////////////////////////////////////
#ifndef CPU_H
#define CPU_H

#include <cstdint>

#define MEM_SIZE 65536

#define CARRY_MASK    0x01
#define ZERO_MASK     0x02
#define IMASK_MASK    0x04
#define DECIMAL_MASK  0x08
#define BREAK_MASK    0x10
#define OVERFLOW_MASK 0x40
#define NEGATIVE_MASK 0x80

#define CARRY    0
#define ZERO     1
#define IMASK    2
#define DECIMAL  3
#define BREAK    4
#define OVERFLOW 6
#define NEGATIVE 7

class Cpu {
public:
  Cpu() : _a(0), _x(0), _y(0), _sp(0), _pc(0), _interrupt(false), _cycles(0)
  {
    clearMem();
  }

  Cpu(uint8_t mem[], uint8_t a = 0, uint8_t x = 0, uint8_t y = 0,
      uint8_t sp = 0, uint8_t pc = 0)
      : _a(a), _x(x), _y(y), _sp(sp), _pc(pc), _interrupt(false), _cycles(0)
  {
    for (uint16_t i = 0; i < MEM_SIZE; i++) {
      _mem[i] = mem[i];
    }
  }

  ~Cpu() {}

  void setA(uint8_t a)   { _a = a; }
  void setX(uint8_t x)   { _x = x; }
  void setY(uint8_t y)   { _y = y; }
  void setSP(uint8_t sp) { _sp = sp; }
  void setPC(uint8_t pc) { _pc = pc; }

  void setCarry()      { _sr |= CARRY; }
  void clearCarry()    { _sr &= ~CARRY; }
  void setZero()       { _sr |= ZERO; }
  void clearZero()     { _sr &= ~ZERO; }
  void setIMask()      { _sr |= IMASK; }
  void clearIMask()    { _sr &= ~IMASK; }
  void setDecimal()    { _sr |= DECIMAL; }
  void clearDecimal()  { _sr &= ~DECIMAL; }
  void setBreak()      { _sr |= BREAK; }
  void clearBreak()    { _sr &= ~BREAK; }
  void setOverflow()   { _sr |= OVERFLOW; }
  void clearOverflow() { _sr &= ~OVERFLOW; }
  void setNegative()   { _sr |= NEGATIVE; }
  void clearNegative() { _sr &= ~NEGATIVE; }

  uint8_t C() const { return (_sr & CARRY_MASK) >> CARRY; }
  uint8_t Z() const { return (_sr & ZERO_MASK) >> ZERO; }
  uint8_t I() const { return (_sr & IMASK_MASK) >> IMASK; }
  uint8_t D() const { return (_sr & DECIMAL_MASK) >> DECIMAL; }
  uint8_t B() const { return (_sr & BREAK_MASK) >> BREAK; }
  uint8_t V() const { return (_sr & OVERFLOW_MASK) >> OVERFLOW; }
  uint8_t N() const { return (_sr & NEGATIVE_MASK) >> NEGATIVE; }

  uint8_t  getMemByte(uint16_t address) const { return _mem[address]; }
  uint16_t getMemWord(uint16_t address) const
  {
    uint16_t ret;
    if (address == MEM_SIZE - 1) {
      ret = _mem[address] << 8;
    } else {
      ret = (_mem[address + 1] << 8) + _mem[address];
    }
    return ret;
  }

  void clearInterruptWaiting() { _interrupt = false; }
  void setInterruptWaiting()   { _interrupt = true; }

  void clearMem()
  {
    for (uint32_t i = 0; i < MEM_SIZE; i++)
      _mem[i] = 0;
  }
  uint16_t incPC()                 { _pc++; return _pc; }
  uint16_t incPC(uint8_t add)      { _pc += add; return _pc; }
  void addToCycles(uint8_t cycles) { _cycles += cycles; }

  uint8_t  A()                const { return _a; }
  uint8_t  X()                const { return _x; }
  uint8_t  Y()                const { return _y; }
  uint8_t  SP()               const { return _sp; }
  uint16_t PC()               const { return _pc; }
  uint8_t  SR()               const { return _sr; }
  uint64_t cycles()           const { return _cycles; }
  bool     interruptWaiting() const { return _interrupt; }

private:
  // Accumulator
  uint8_t  _a;
  // Index registers
  uint8_t  _x, _y;
  // Stack pointer
  uint8_t  _sp;
  // Program Counter
  uint16_t _pc;
  // Status register
  uint8_t  _sr;
  // Interrupt waiting
  bool     _interrupt;
  // Memory map
  uint8_t  _mem[MEM_SIZE];
  // Cycle Count
  uint64_t _cycles;
};
#endif // CPU_H
