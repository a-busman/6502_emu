///////////////////////////////////////////////////////////////////////////////
// instruction.h
//
// Header file containing instruction base class
//
// Author: Alex Busman
// Date: July 1, 2015
///////////////////////////////////////////////////////////////////////////////
#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <cstdint>
#include <string>

#include "cpu.h"

enum {
  ACC,
  IMP,
  IMM,
  ZER,
  ABS,
  REL,
  IDX1,
  IDX2,
  IDX1_IND
} AddressMode;

enum {
  NONE,
  EIGHT_BIT,
  SIXTEEN_BIT
} Operand;

enum {
  A,
  X,
  Y,
  SP,
  PC,
  SR
} Register;

class Instruction {
public:
  Instruction() : _cpu(NULL), _mode(IMP), _operationName(""), _opcode(0),
                  _cycles(0) {}
  Instruction(Cpu *cpu) : _cpu(cpu), _mode(IMP), _operationName(""), _opcode(0),
                          _cycles(0) {}
  Instruction(Cpu *cpu, AddressMode mode, string name, uint8_t opcode,
              uint8_t cycles) : _cpu(cpu), _mode(mode), _operationName(name),
                                _opcode(opcode), _cyles(cycles) {}
  ~Instruction();

protected:
  Cpu *_cpu;
  AddressMode _mode;
  string _operationName;
  uint8_t _opcode;
  uint8_t _cycles;
  uint8_t _operation() { return 0; }
  uint8_t _operation(uint8_t operand) { return 0; }
  uint8_t _operation(uint16_t operand) { return 0; }
  uint8_t _operation(uint8_t operand, Register reg) { return 0; }
  uint8_t _operation(uint16_t operand, Register reg) { return 0; }
}

#endif // INSTRUCTION_H
