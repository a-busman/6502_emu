///////////////////////////////////////////////////////////////////////////////
// operations.h
//
// Header file containing all operation functions
//
// Author: Alex Busman
// Date: July 8, 2015
///////////////////////////////////////////////////////////////////////////////
#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <cstdint>

#include "cpu.h"

enum AddressMode {
  ACC,
  IMP,
  IMM,
  ZER,
  ABS,
  REL,
  IND,
  IDX1,
  IDX2_X,
  IDX2_Y,
  IDX1_PRE,
  IDX1_POST
};

uint8_t ADC(Cpu *cpu, uint16_t operand, AddressMode mode)
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
  case ABS:
    adder = cpu->getMemByte(operand);
    cpu->addToCycles(4);
    cpu->incPC();
    break;
  case IDX2_X:
    adder = cpu->getMemByte(static_cast<int16_t>(operand) + cpu->X());
    cpu->addToCycles(4);
    cpu->incPC();
    break;
  case IDX2_Y:
    adder = cpu->getMemByte(static_cast<int16_t>(operand) + cpu->Y());
    cpu->addToCycles(4);
    cpu->incPC();
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

uint8_t AND(Cpu *cpu, uint16_t operand, AddressMode mode)
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
  case ABS:
    ander = cpu->getMemByte(operand);
    cpu->addToCycles(4);
    cpu->incPC();
    break;
  case IDX2_X:
    ander = cpu->getMemByte(static_cast<int16_t>(operand) + cpu->X());
    cpu->addToCycles(4);
    cpu->incPC();
    break;
  case IDX2_Y:
    ander = cpu->getMemByte(static_cast<int16_t>(operand) + cpu->Y());
    cpu->addToCycles(4);
    cpu->incPC();
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

uint8_t ASL(Cpu *cpu, uint16_t operand, AddressMode mode)
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
  case ABS:
    offset = operand;
    cpu->addToCycles(6);
    cpu->incPC();
    break;
  case IDX2_X:
    offset = static_cast<int16_t>(operand) + cpu->X();
    cpu->addToCycles(7);
    cpu->incPC();
    break;
  default:
    offset = 0;
  }
  *(memBase + offset) & 0x80 ? cpu->setCarry() : cpu->clearCarry();
  *(memBase + offset) <<= 1;

  *(memBase + offset) & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  *(memBase + offset) ? cpu->clearZero() : cpu->setZero();

  cpu->incPC(2);
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

uint8_t BIT(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  uint8_t a = cpu->A();
  uint8_t ander = cpu->getMemByte(operand);
  uint8_t res = a & ander;
  switch(mode) {
  case ZER:
    cpu->addToCycles(3);
    break;
  case ABS:
    cpu->addToCycles(4);
    cpu->incPC();
    break;
  default:
    break;
  }


  res & 0x80 ? cpu->setNegative() : cpu->clearNegative();

  (a > 127 && ander > 127 && res < 128) ||
  (a < 128 && ander < 128 && res > 127) ?
  cpu->setOverflow() : cpu->clearOverflow();

  res ? cpu->clearZero() : cpu->setZero();
  cpu->incPC(2);
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

void BRK(Cpu *cpu) {
  uint16_t pc = cpu->PC() + 1;
  cpu->writeToStack((pc >> 8) & 0x00FF);
  cpu->setSP(cpu->SP() - 1);
  cpu->writeToStack(pc & 0x00FF);
  cpu->setSP(cpu->SP() - 1);
  cpu->writeToStack(cpu->SR() | BREAK_MASK);
  cpu->setSP(cpu->SP() - 1);
  cpu->addToCycles(7);
  cpu->setPC(cpu->getMemWord(INTERRUPT_VECTOR));
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

uint8_t CMP(Cpu *cpu, uint16_t operand, AddressMode mode)
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
  case ABS:
    comparitor = cpu->getMemByte(operand);
    cpu->addToCycles(4);
    cpu->incPC();
    break;
  case IDX2_X:
    comparitor = cpu->getMemByte(static_cast<int16_t>(operand) + cpu->X());
    cpu->addToCycles(4);
    cpu->incPC();
    break;
  case IDX2_Y:
    comparitor = cpu->getMemByte(static_cast<int16_t>(operand) + cpu->Y());
    cpu->addToCycles(4);
    cpu->incPC();
    break;
  default:
    comparitor = 0;
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

uint8_t CPX(Cpu *cpu, uint16_t operand, AddressMode mode)
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
  case ABS:
    comparitor = cpu->getMemByte(operand);
    cpu->addToCycles(4);
    cpu->incPC();
  default:
    comparitor = 0;
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

uint8_t CPY(Cpu *cpu, uint16_t operand, AddressMode mode)
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
  case ABS:
    comparitor = cpu->getMemByte(operand);
    cpu->addToCycles(4);
    cpu->incPC();
  default:
    comparitor = 0;
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

uint8_t DEC(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  uint8_t address = 0;
  uint8_t *memBase = cpu->getMemBase();

  switch(mode) {
  case ZER:
    address = operand;
    cpu->addToCycles(5);
    break;
  case IDX1:
    address = static_cast<int8_t>(operand) + cpu->X();
    cpu->addToCycles(6);
    break;
  case ABS:
    address = operand;
    cpu->addToCycles(6);
    cpu->incPC();
    break;
  case IDX2_X:
    address = static_cast<int16_t>(operand) + cpu->X();
    cpu->addToCycles(7);
    cpu->incPC();
    break;
  default:
    address = 0;
  }

  *(memBase + address) -= 1;

  *(memBase + address) & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  *(memBase + address) ? cpu->clearZero() : cpu->setZero();
  cpu->incPC(2);
  return cpu->SR();
}

uint8_t DEX(Cpu *cpu)
{
  cpu->setX(cpu->X() - 1);
  cpu->addToCycles(2);
  cpu->X() & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  cpu->X() ? cpu->clearZero() : cpu->setZero();
  cpu->incPC();
  return cpu->SR();
}

uint8_t DEY(Cpu *cpu)
{
  cpu->setY(cpu->Y() - 1);
  cpu->addToCycles(2);
  cpu->Y() & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  cpu->Y() ? cpu->clearZero() : cpu->setZero();
  cpu->incPC();
  return cpu->SR();
}

uint8_t EOR(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  uint8_t a = cpu->A();
  uint8_t eorer = 0;
  uint8_t res = 0;

  switch (mode) {
  case IMM:
    eorer = operand;
    cpu->addToCycles(2);
    break;
  case ZER:
    eorer = cpu->getMemByte(operand);
    cpu->addToCycles(3);
    break;
  case IDX1:
    eorer = cpu->getMemByte(static_cast<int8_t>(operand) + cpu->X());
    cpu->addToCycles(4);
    break;
  case IDX1_PRE:
    eorer = cpu->getMemByte(cpu->getMemWord(operand + cpu->X()));
    cpu->addToCycles(6);
    break;
  case IDX1_POST:
    eorer = cpu->getMemByte(cpu->getMemWord(operand) + cpu->Y());
    cpu->addToCycles(5);
    break;
  case ABS:
    eorer = cpu->getMemByte(operand);
    cpu->addToCycles(4);
    cpu->incPC();
    break;
  case IDX2_X:
    eorer = cpu->getMemByte(static_cast<int16_t>(operand) + cpu->X());
    cpu->addToCycles(4);
    cpu->incPC();
    break;
  case IDX2_Y:
    eorer = cpu->getMemByte(static_cast<int16_t>(operand) + cpu->Y());
    cpu->addToCycles(4);
    cpu->incPC();
    break;
  default:
    eorer = 0;
  }

  res = a ^ eorer;

  res & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  res ? cpu->clearZero() : cpu->setZero();
  cpu->setA(res);
  cpu->incPC(2);
  return cpu->SR();
}

uint8_t INC(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  uint8_t address = 0;
  uint8_t *memBase = cpu->getMemBase();

  switch(mode) {
  case ZER:
    address = operand;
    cpu->addToCycles(5);
    break;
  case IDX1:
    address = static_cast<int8_t>(operand) + cpu->X();
    cpu->addToCycles(6);
    break;
  case ABS:
    address = operand;
    cpu->addToCycles(6);
    cpu->incPC();
    break;
  case IDX2_X:
    address = static_cast<int16_t>(operand) + cpu->X();
    cpu->addToCycles(7);
    cpu->incPC();
    break;
  default:
    address = 0;
  }

  *(memBase + address) += 1;

  *(memBase + address) & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  *(memBase + address) ? cpu->clearZero() : cpu->setZero();
  cpu->incPC(2);
  return cpu->SR();
}

uint8_t INX(Cpu *cpu)
{
  cpu->setX(cpu->X() + 1);
  cpu->addToCycles(2);
  cpu->X() & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  cpu->X() ? cpu->clearZero() : cpu->setZero();
  cpu->incPC();
  return cpu->SR();
}

uint8_t INY(Cpu *cpu)
{
  cpu->setY(cpu->Y() + 1);
  cpu->addToCycles(2);
  cpu->Y() & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  cpu->Y() ? cpu->clearZero() : cpu->setZero();
  cpu->incPC();
  return cpu->SR();
}

void JMP(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  switch(mode) {
  case ABS:
    cpu->addToCycles(3);
    cpu->setPC(operand);
    break;
  case IND:
    cpu->addToCycles(5);
    cpu->setPC(cpu->getMemWord(operand));
    break;
  default:
    break;
  }
}

void JSR(Cpu *cpu, uint16_t operand)
{
  uint16_t returnAddress = cpu->PC() + 2;
  cpu->writeToStack((returnAddress >> 8) & 0x00FF);
  cpu->setSP(cpu->SP() - 1);
  cpu->writeToStack(returnAddress & 0x00FF);
  cpu->setSP(cpu->SP() - 1);
  cpu->setPC(operand);
  cpu->addToCycles(6);
}

uint8_t LDA(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  switch (mode) {
  case IMM:
    cpu->setA(operand);
    cpu->addToCycles(2);
    break;
  case ZER:
    cpu->setA(cpu->getMemByte(operand));
    cpu->addToCycles(3);
    break;
  case IDX1:
    cpu->setA(cpu->getMemByte(static_cast<int8_t>(operand) + cpu->X()));
    cpu->addToCycles(4);
    break;
  case IDX1_PRE:
    cpu->setA(cpu->getMemByte(cpu->getMemWord(operand + cpu->X())));
    cpu->addToCycles(6);
    break;
  case IDX1_POST:
    cpu->setA(cpu->getMemByte(cpu->getMemWord(operand) + cpu->Y()));
    cpu->addToCycles(5);
    break;
  case ABS:
    cpu->setA(cpu->getMemByte(operand));
    cpu->addToCycles(4);
    cpu->incPC();
    break;
  case IDX2_X:
    cpu->setA(cpu->getMemByte(static_cast<int16_t>(operand) + cpu->X()));
    cpu->addToCycles(4);
    cpu->incPC();
    break;
  case IDX2_Y:
    cpu->setA(cpu->getMemByte(static_cast<int16_t>(operand) + cpu->Y()));
    cpu->addToCycles(4);
    cpu->incPC();
    break;
  default:
    break;
  }
  cpu->A() & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  cpu->A() ? cpu->clearZero() : cpu->setZero();
  cpu->incPC(2);
  return cpu->SR();
}

uint8_t LDX(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  switch (mode) {
  case IMM:
    cpu->setX(operand);
    cpu->addToCycles(2);
    break;
  case ZER:
    cpu->setX(cpu->getMemByte(operand));
    cpu->addToCycles(3);
    break;
  case IDX1:
    cpu->setX(cpu->getMemByte(static_cast<int8_t>(operand) + cpu->Y()));
    cpu->addToCycles(4);
    break;
  case ABS:
    cpu->setX(cpu->getMemByte(operand));
    cpu->addToCycles(4);
    cpu->incPC();
    break;
  case IDX2_Y:
    cpu->setX(cpu->getMemByte(static_cast<int16_t>(operand) + cpu->Y()));
    cpu->addToCycles(4);
    cpu->incPC();
    break;
  default:
    break;
  }
  cpu->X() & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  cpu->X() ? cpu->clearZero() : cpu->setZero();
  cpu->incPC(2);
  return cpu->SR();
}

uint8_t LDY(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  switch (mode) {
  case IMM:
    cpu->setY(operand);
    cpu->addToCycles(2);
    break;
  case ZER:
    cpu->setY(cpu->getMemByte(operand));
    cpu->addToCycles(3);
    break;
  case IDX1:
    cpu->setY(cpu->getMemByte(static_cast<int8_t>(operand) + cpu->X()));
    cpu->addToCycles(4);
    break;
  case ABS:
    cpu->setY(cpu->getMemByte(operand));
    cpu->addToCycles(4);
    cpu->incPC();
    break;
  case IDX2_Y:
    cpu->setY(cpu->getMemByte(static_cast<int16_t>(operand) + cpu->X()));
    cpu->addToCycles(4);
    cpu->incPC();
    break;
  default:
    break;
  }
  cpu->Y() & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  cpu->Y() ? cpu->clearZero() : cpu->setZero();
  cpu->incPC(2);
  return cpu->SR();
}

uint8_t LSR(Cpu *cpu)
{
  uint8_t a = cpu->A();
  a & 0x01 ? cpu->setCarry() : cpu->clearCarry();

  cpu->setA((a >> 1) & 0x7F);

  cpu->A() ? cpu->clearZero() : cpu->setZero();
  cpu->addToCycles(2);
  cpu->incPC();
  return cpu->SR();
}

uint8_t LSR(Cpu *cpu, uint16_t operand, AddressMode mode)
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
  case ABS:
    offset = operand;
    cpu->addToCycles(6);
    cpu->incPC();
    break;
  case IDX2_X:
    offset = static_cast<int16_t>(operand) + cpu->X();
    cpu->addToCycles(7);
    cpu->incPC();
    break;
  default:
    offset = 0;
  }
  *(memBase + offset) & 0x01 ? cpu->setCarry() : cpu->clearCarry();
  *(memBase + offset) >>= 1;

  *(memBase + offset) ? cpu->clearZero() : cpu->setZero();

  cpu->incPC(2);
  return cpu->SR();
}

void NOP(Cpu *cpu)
{
  cpu->addToCycles(2);
  cpu->incPC();
}

uint8_t ORA(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  uint8_t a = cpu->A();
  uint8_t orer = 0;

  switch(mode) {
  case IMM:
    orer = operand;
    cpu->addToCycles(2);
    break;
  case ZER:
    orer = cpu->getMemByte(operand);
    cpu->addToCycles(3);
    break;
  case IDX1:
    orer = cpu->getMemByte(static_cast<int8_t>(operand) + cpu->X());
    cpu->addToCycles(4);
    break;
  case IDX1_PRE:
    orer = cpu->getMemByte(cpu->getMemWord(operand + cpu->X()));
    cpu->addToCycles(6);
    break;
  case IDX1_POST:
    orer = cpu->getMemByte(cpu->getMemWord(operand) + cpu->Y());
    cpu->addToCycles(5);
    break;
  case ABS:
    orer = cpu->getMemByte(operand);
    cpu->addToCycles(4);
    cpu->incPC();
    break;
  case IDX2_X:
    orer = cpu->getMemByte(static_cast<int16_t>(operand) + cpu->X());
    cpu->addToCycles(4);
    cpu->incPC();
    break;
  case IDX2_Y:
    orer = cpu->getMemByte(static_cast<int16_t>(operand) + cpu->Y());
    cpu->addToCycles(4);
    cpu->incPC();
    break;
  default:
    orer = 0;
  }
  cpu->setA(a | orer);

  cpu->A() & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  cpu->A() ? cpu->clearZero() : cpu->setZero();

  cpu->incPC(2);
  return cpu->SR();
}

void PHA(Cpu *cpu)
{
  cpu->writeToStack(cpu->A());
  cpu->setSP(cpu->SP() - 1);
  cpu->addToCycles(3);
  cpu->incPC();
}

void PHP(Cpu *cpu)
{
  cpu->writeToStack(cpu->SR());
  cpu->setSP(cpu->SP() - 1);
  cpu->addToCycles(3);
  cpu->incPC();
}

uint8_t PLA(Cpu *cpu)
{
  cpu->setSP(cpu->SP() + 1);
  cpu->setA(cpu->getMemByte(cpu->SP()));

  cpu->A() & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  cpu->A() ? cpu->clearZero() : cpu->setZero();

  cpu->addToCycles(4);
  cpu->incPC();
  return cpu->SR();
}

uint8_t PLP(Cpu *cpu)
{
  cpu->setSP(cpu->SP() + 1);
  cpu->setSR(cpu->getMemByte(cpu->SP()));

  cpu->addToCycles(4);
  cpu->incPC();
  return cpu->SR();
}
#endif // OPERATIONS_H
