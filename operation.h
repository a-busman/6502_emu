///////////////////////////////////////////////////////////////////////////////
// operation.h
//
// Header file containing all operation functions
//
// Author: Alex Busman
// Date: July 7, 2015
///////////////////////////////////////////////////////////////////////////////
#ifndef OPERATION_H
#define OPERATION_H

#include <cstdint>

#include "cpu.h"

enum AddressMode {
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

uint8_t ADC(Cpu *cpu, uint8_t operand, AddressMode mode)
{
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
    break;
  default:
    adder = 0;
  }

  cpu->setA(a + adder + cpu->C());
  cpu->A() < a ? cpu->setCarry() : cpu->clearCarry();
  cpu->A() & 0x80 ? cpu->setNegative() : cpu->clearNegative();

  (a > 127 && operand > 127 && cpu->A() > 127) ||
  (a < 128 && operand < 128 && cpu->A() < 128) ||
  (a > 127 && operand < 128)                   ||
  (a < 128 && operand > 127)                   ?
  cpu->clearOverflow() : cpu->setOverflow();

  cpu->A() ? cpu->clearZero() : cpu->setZero();

  cpu->incPC(2);
  return cpu->SR();
}

uint8_t ADC(Cpu *cpu, uint16_t operand, AddressMode mode)
{
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
  default:
    adder = 0;
  }

  cpu->setA(a + adder + cpu->C());
  cpu->A() < a ? cpu->setCarry() : cpu->clearCarry();
  cpu->A() & 0x80 ? cpu->setNegative() : cpu->clearNegative();

  (a > 127 && operand > 127 && cpu->A() > 127) ||
  (a < 128 && operand < 128 && cpu->A() < 128) ||
  (a > 127 && operand < 128)                   ||
  (a < 128 && operand > 127)                   ?
  cpu->clearOverflow() : cpu->setOverflow();

  cpu->A() ? cpu->clearZero() : cpu->setZero();

  cpu->incPC(3);
  return cpu->SR();
}

uint8_t AND(Cpu *cpu, uint8_t operand, AddressMode mode)
{
  uint8_t a = cpu->A();
  uint8_t ander = 0;

  switch(mode) {
  case IMM:
    ander = operand;
    cpu->addToCycles(2);
    break;
  case ZER:
    ander = cpu->getMemByte(operand);
    cpu->addToCycles(3);
    break;
  case IDX1:
    ander = cpu->getMemByte(operand + cpu->X());
    cpu->addToCycles(4);
    break;
  case IDX1_PRE:
    ander = cpu->getMemByte(cpu->getMemWord(static_cast<uint8_t>(operand +
                                                                 cpu->X())));
    cpu->addToCycles(6);
    break;
  case IDX1_POST:
    ander = cpu->getMemByte(cpu->getMemWord(operand) + cpu->Y());
    cpu->addToCycles(5);
    break;
  default:
    ander = 0;
  }
  cpu->setA(a & ander);

  cpu->A() & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  cpu->A() ? cpu->clearZero() : cpu->setZero();

  cpu->incPC(2);
  return cpu->SR();
}

uint8_t AND(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  uint8_t a = cpu->A();
  uint8_t ander = 0;

  switch(mode) {
  case ABS:
    ander = cpu->getMemByte(operand);
    cpu->addToCycles(4);
    break;
  case IDX2_X:
    ander = cpu->getMemByte(operand + cpu->X());
    cpu->addToCycles(4);
    break;
  case IDX2_Y:
    ander = cpu->getMemByte(operand + cpu->Y());
    cpu->addToCycles(4);
    break;
  default:
    ander = 0;
  }
  cpu->setA(a & ander);

  cpu->A() & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  cpu->A() ? cpu->clearZero() : cpu->setZero();

  cpu->incPC(3);
  return cpu->SR();
}

uint8_t ASL(Cpu *cpu)
{
  uint8_t a = cpu->A();
  a & 0x80 ? cpu->setCarry() : cpu->clearCarry();

  cpu->setA(a << 1);

  cpu->A() & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  cpu->A() ? cpu->clearZero() : cpu->setZero();
  cpu->addToCycles(2);
  cpu->incPC();
  return cpu->SR();
}

uint8_t ASL(Cpu *cpu, uint8_t operand, AddressMode mode)
{
  uint8_t* memBase = cpu->getMemBase();
  uint8_t offset = 0;
  switch (mode) {
  case ZER:
    offset = operand;
    cpu->addToCycles(5);
    break;
  case IDX1:
    offset = operand + cpu->X();
    cpu->addToCycles(6);
    break;
  default:
    offset = 0;
    break;
  }
  *(memBase + offset) & 0x80 ? cpu->setCarry() : cpu->clearCarry();
  *(memBase + offset) <<= 1;

  *(memBase + offset) & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  *(memBase + offset) ? cpu->clearZero() : cpu->setZero();

  cpu->incPC(2);
  return cpu->SR();
}

uint8_t ASL(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  uint8_t* memBase = cpu->getMemBase();
  uint16_t offset = 0;
  switch (mode) {
  case ABS:
    offset = operand;
    cpu->addToCycles(6);
    break;
  case IDX2_X:
    offset = operand + cpu->X();
    cpu->addToCycles(7);
    break;
  default:
    offset = 0;
    break;
  }
  *(memBase + offset) & 0x80 ? cpu->setCarry() : cpu->clearCarry();
  *(memBase + offset) <<= 1;

  *(memBase + offset) & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  *(memBase + offset) ? cpu->clearZero() : cpu->setZero();

  cpu->incPC(3);
  return cpu->SR();
}

#endif // OPERATION_H
