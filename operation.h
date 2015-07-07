///////////////////////////////////////////////////////////////////////////////
// operation.h
//
// Header file containing operation base class
//
// Author: Alex Busman
// Date: July 6, 2015
///////////////////////////////////////////////////////////////////////////////
#ifndef OPERATION_H
#define OPERATION_H

#include <cstdint>

#include "cpu.h"

enum AddressMode{
  ACC,
  IMP,
  IMM,
  ZER,
  ABS,
  REL,
  IDX1,
  IDX2_X,
  IDX2_Y,
  IDX1_PRE,
  IDX1_POST
};

enum Operand{
  NONE,
  EIGHT_BIT,
  SIXTEEN_BIT
};

enum Register{
  A,
  X,
  Y,
  SP,
  PC,
  SR
};

uint8_t ADC(Cpu *cpu, uint8_t operand, AddressMode mode) {
  uint8_t sr = cpu->SR();
  uint8_t a = cpu->A();
  uint8_t adder = 0;

  switch(mode) {
  case IMM:
    adder = operand;
    cpu->addToCycles(2);
    break;
  case ZER:
    adder = cpu->getMemByte(operand);
    cpu->addToCycles(3);
    break;
  case IDX1:
    adder = cpu->getMemByte(operand + cpu->X());
    cpu->addToCycles(4);
    break;
  case IDX1_PRE:
    adder = cpu->getMemByte(cpu->getMemWord(static_cast<uint8_t>(operand +
                                                                 cpu->X())));
    cpu->addToCycles(6);
    break;
  case IDX1_POST:
    adder = cpu->getMemByte(cpu->getMemWord(operand) + cpu->Y());
    cpu->addToCycles(5);
  }

  cpu->setA(a + adder + cpu->SR() & CARRY ? 1 : 0);
  cpu->A() < a ? cpu->setCarry() : cpu->clearCarry();
  cpu->A() & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  (a > 127 && operand > 127 && cpu->A() > 127) ||
  (a < 128 && operand < 128 && cpu->A() < 128) ?
  cpu->clearOverflow() : cpu->setOverflow();
  cpu->A() ? cpu->clearZero() : cpu->setZero();

  return cpu->SR();
}

uint8_t ADC(Cpu *cpu, uint16_t operand, AddressMode mode) {
  uint8_t sr = cpu->SR();
  uint8_t a = cpu->A();
  uint8_t adder = 0;
  switch(mode) {
  case ABS:
    adder = cpu->getMemByte(operand);
    cpu->addToCycles(4);
    break;
  case IDX2_X:
    adder = cpu->getMemByte(operand + cpu->X());
    cpu->addToCycles(4);
    break;
  case IDX2_Y:
    adder = cpu->getMemByte(operand + cpu->Y());
    cpu->addToCycles(4);
    break;
  }
  cpu->setA(a + adder + cpu->SR() & CARRY ? 1 : 0);
  cpu->A() < a ? cpu->setCarry() : cpu->clearCarry();
  cpu->A() & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  (a > 127 && operand > 127 && cpu->A() > 127) ||
  (a < 128 && operand < 128 && cpu->A() < 128) ?
  cpu->clearOverflow() : cpu->setOverflow();
  cpu->A() ? cpu->clearZero() : cpu->setZero();
}

#endif // OPERATION_H
