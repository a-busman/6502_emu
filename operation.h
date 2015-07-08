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
    adder = cpu->getMemByte(static_cast<int8_t>(operand) + cpu->X());
    cpu->addToCycles(4);
    break;
  case IDX1_PRE:
    adder = cpu->getMemByte(cpu->getMemWord(operand + cpu->X()));
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

  (a > 127 && adder > 127 && cpu->A() < 128) ||
  (a < 128 && adder < 128 && cpu->A() > 127) ?
  cpu->setOverflow() : cpu->clearOverflow();

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
    adder = cpu->getMemByte(static_cast<int16_t>(operand) + cpu->X());
    cpu->addToCycles(4);
    break;
  case IDX2_Y:
    adder = cpu->getMemByte(static_cast<int16_t>(operand) + cpu->Y());
    cpu->addToCycles(4);
    break;
  default:
    adder = 0;
  }

  cpu->setA(a + adder + cpu->C());
  cpu->A() < a ? cpu->setCarry() : cpu->clearCarry();
  cpu->A() & 0x80 ? cpu->setNegative() : cpu->clearNegative();

  (a > 127 && adder > 127 && cpu->A() < 128) ||
  (a < 128 && adder < 128 && cpu->A() > 127) ?
  cpu->setOverflow() : cpu->clearOverflow();

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
    ander = cpu->getMemByte(static_cast<int8_t>(operand) + cpu->X());
    cpu->addToCycles(4);
    break;
  case IDX1_PRE:
    ander = cpu->getMemByte(cpu->getMemWord(operand + cpu->X()));
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
    ander = cpu->getMemByte(static_cast<int16_t>(operand) + cpu->X());
    cpu->addToCycles(4);
    break;
  case IDX2_Y:
    ander = cpu->getMemByte(static_cast<int16_t>(operand) + cpu->Y());
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
    offset = static_cast<int8_t>(operand) + cpu->X();
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
    offset = static_cast<int16_t>(operand) + cpu->X();
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

void BCC(Cpu *cpu, int8_t offset)
{
  cpu->addToCycles(2);
  cpu->C() ? cpu->incPC(2) : cpu->setPC(cpu->PC() + offset);
}

void BCS(Cpu *cpu, int8_t offset)
{
  cpu->addToCycles(2);
  cpu->C() ? cpu->setPC(cpu->PC() + offset) : cpu->incPC(2);
}

void BEQ(Cpu *cpu, int8_t offset)
{
  cpu->addToCycles(2);
  cpu->Z() ? cpu->setPC(cpu->PC() + offset) : cpu->incPC(2);
}

uint8_t BIT(Cpu *cpu, uint8_t operand)
{
  uint8_t a = cpu->A();
  uint8_t ander = cpu->getMemByte(operand);
  uint8_t res = a & ander;
  cpu->addToCycles(3);

  res & 0x80 ? cpu->setNegative() : cpu->clearNegative();

  (a > 127 && ander > 127 && res < 128) ||
  (a < 128 && ander < 128 && res > 127) ?
  cpu->setOverflow() : cpu->clearOverflow();

  res ? cpu->clearZero() : cpu->setZero();
  cpu->incPC(2);
  return cpu->SR();
}

uint8_t BIT(Cpu *cpu, uint16_t operand)
{
  uint8_t a = cpu->A();
  uint8_t ander = cpu->getMemByte(operand);
  uint8_t res = a & ander;
  cpu->addToCycles(4);

  res & 0x80 ? cpu->setNegative() : cpu->clearNegative();

  (a > 127 && ander > 127 && res < 128) ||
  (a < 128 && ander < 128 && res > 127) ?
  cpu->setOverflow() : cpu->clearOverflow();

  res ? cpu->clearZero() : cpu->setZero();
  cpu->incPC(3);
  return cpu->SR();
}

void BMI(Cpu *cpu, int8_t offset)
{
  cpu->addToCycles(2);
  cpu->N() ? cpu->setPC(cpu->PC() + offset) : cpu->incPC(2);
}

void BNE(Cpu *cpu, int8_t offset)
{
  cpu->addToCycles(2);
  cpu->Z() ? cpu->incPC(2) : cpu->setPC(cpu->PC() + offset);
}

void BPL(Cpu *cpu, int8_t offset)
{
  cpu->addToCycles(2);
  cpu->N() ? cpu->incPC(2) : cpu->setPC(cpu->PC() + offset);
}

uint8_t BRK(Cpu *cpu) {
  cpu->addToCycles(7);
  cpu->incPC();
  cpu->setBreak();
  cpu->setIMask();
  return cpu->SR();
}

void BVC(Cpu *cpu, int8_t offset)
{
  cpu->addToCycles(2);
  cpu->V() ? cpu->incPC(2) : cpu->setPC(cpu->PC() + offset);
}

void BVS(Cpu *cpu, int8_t offset)
{
  cpu->addToCycles(2);
  cpu->V() ? cpu->setPC(cpu->PC() + offset) : cpu->incPC(2);
}

uint8_t CLC(Cpu *cpu)
{
  cpu->addToCycles(2);
  cpu->clearCarry();
  cpu->incPC();
  return cpu->SR();
}

uint8_t CLD(Cpu *cpu)
{
  cpu->addToCycles(2);
  cpu->clearDecimal();
  cpu->incPC();
  return cpu->SR();
}

uint8_t CLI(Cpu *cpu)
{
  cpu->addToCycles(2);
  cpu->clearIMask();
  cpu->incPC();
  return cpu->SR();
}

uint8_t CLV(Cpu *cpu)
{
  cpu->addToCycles(2);
  cpu->clearOverflow();
  cpu->incPC();
  return cpu->SR();
}

uint8_t CMP(Cpu *cpu, uint8_t operand, AddressMode mode)
{
  uint8_t a = cpu->A();
  uint8_t comparitor = 0;
  uint8_t res = 0;

  switch(mode) {
  case IMM:
    comparitor = operand;
    cpu->addToCycles(2);
    break;
  case ZER:
    comparitor = cpu->getMemByte(operand);
    cpu->addToCycles(3);
    break;
  case IDX1:
    comparitor = cpu->getMemByte(static_cast<int8_t>(operand) + cpu->X());
    cpu->addToCycles(4);
    break;
  case IDX1_PRE:
    comparitor = cpu->getMemByte(cpu->getMemWord(operand + cpu->X()));
    cpu->addToCycles(6);
    break;
  case IDX1_POST:
    comparitor = cpu->getMemByte(cpu->getMemWord(operand) + cpu->Y());
    cpu->addToCycles(5);
    break;
  default:
    comparitor = 0;
    break;
  }
  res = a - comparitor;

  res & 0x80 ? cpu->setNegative() : cpu->clearNegative();

  (a > 127 && comparitor > 127 && res < 128) ||
  (a < 128 && comparitor < 128 && res > 127) ?
  cpu->setOverflow() : cpu->clearOverflow();

  res ? cpu->clearZero() : cpu->setZero();
  cpu->incPC(2);
  return cpu->SR();
}

uint8_t CMP(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  uint8_t a = cpu->A();
  uint8_t comparitor = 0;
  uint8_t res = 0;

  switch(mode) {
  case ABS:
    comparitor = cpu->getMemByte(operand);
    cpu->addToCycles(4);
    break;
  case IDX2_X:
    comparitor = cpu->getMemByte(static_cast<int16_t>(operand) + cpu->X());
    cpu->addToCycles(4);
    break;
  case IDX2_Y:
    comparitor = cpu->getMemByte(static_cast<int16_t>(operand) + cpu->Y());
    cpu->addToCycles(4);
    break;
  default:
    comparitor = 0;
    break;
  }
  res = a - comparitor;

  res & 0x80 ? cpu->setNegative() : cpu->clearNegative();

  (a > 127 && comparitor > 127 && res < 128) ||
  (a < 128 && comparitor < 128 && res > 127) ?
  cpu->setOverflow() : cpu->clearOverflow();

  res ? cpu->clearZero() : cpu->setZero();
  cpu->incPC(3);
  return cpu->SR();
}

uint8_t CPX(Cpu *cpu, uint8_t operand, AddressMode mode)
{
  uint8_t x = cpu->X();
  uint8_t comparitor = 0;
  uint8_t res = 0;

  switch(mode) {
  case IMM:
    comparitor = operand;
    cpu->addToCycles(2);
    break;
  case ZER:
    comparitor = cpu->getMemByte(operand);
    cpu->addToCycles(3);
    break;
  default:
    comparitor = 0;
    break;
  }
  res = x - comparitor;

  res & 0x80 ? cpu->setNegative() : cpu->clearNegative();

  (x > 127 && comparitor > 127 && res < 128) ||
  (x < 128 && comparitor < 128 && res > 127) ?
  cpu->setOverflow() : cpu->clearOverflow();

  res ? cpu->clearZero() : cpu->setZero();
  cpu->incPC(2);
  return cpu->SR();
}

uint8_t CPX(Cpu *cpu, uint16_t operand)
{
  uint8_t x = cpu->X();
  uint8_t comparitor = cpu->getMemByte(operand);
  uint8_t res = x - comparitor;

  cpu->addToCycles(4);

  res & 0x80 ? cpu->setNegative() : cpu->clearNegative();

  (x > 127 && comparitor > 127 && res < 128) ||
  (x < 128 && comparitor < 128 && res > 127) ?
  cpu->setOverflow() : cpu->clearOverflow();

  res ? cpu->clearZero() : cpu->setZero();
  cpu->incPC(3);
  return cpu->SR();
}

uint8_t CPY(Cpu *cpu, uint8_t operand, AddressMode mode)
{
  uint8_t y = cpu->Y();
  uint8_t comparitor = 0;
  uint8_t res = 0;

  switch(mode) {
  case IMM:
    comparitor = operand;
    cpu->addToCycles(2);
    break;
  case ZER:
    comparitor = cpu->getMemByte(operand);
    cpu->addToCycles(3);
    break;
  default:
    comparitor = 0;
    break;
  }
  res = y - comparitor;

  res & 0x80 ? cpu->setNegative() : cpu->clearNegative();

  (y > 127 && comparitor > 127 && res < 128) ||
  (y < 128 && comparitor < 128 && res > 127) ?
  cpu->setOverflow() : cpu->clearOverflow();

  res ? cpu->clearZero() : cpu->setZero();
  cpu->incPC(2);
  return cpu->SR();
}

uint8_t CPY(Cpu *cpu, uint16_t operand)
{
  uint8_t y = cpu->Y();
  uint8_t comparitor = cpu->getMemByte(operand);
  uint8_t res = y - comparitor;

  cpu->addToCycles(4);

  res & 0x80 ? cpu->setNegative() : cpu->clearNegative();

  (y > 127 && comparitor > 127 && res < 128) ||
  (y < 128 && comparitor < 128 && res > 127) ?
  cpu->setOverflow() : cpu->clearOverflow();

  res ? cpu->clearZero() : cpu->setZero();
  cpu->incPC(3);
  return cpu->SR();
}
#endif // OPERATION_H
