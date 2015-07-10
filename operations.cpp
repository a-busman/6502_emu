///////////////////////////////////////////////////////////////////////////////
// operations.pp
//
// Implimentation of operations
//
// Author: Alex Busman
// Date: July 10, 2015
///////////////////////////////////////////////////////////////////////////////

#include "operations.h"

void ADC(Cpu *cpu, uint16_t operand, AddressMode mode)
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
}

void AND(Cpu *cpu, uint16_t operand, AddressMode mode)
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
}

void ASL(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  uint8_t* memBase = cpu->getMemBase();
  uint8_t offset = 0;
  uint8_t a = cpu->A();
  switch (mode) {
  case ACC:
    a & 0x80 ? cpu->setCarry() : cpu->clearCarry();

    cpu->setA(a << 1);

    cpu->A() & 0x80 ? cpu->setNegative() : cpu->clearNegative();
    cpu->A() ? cpu->clearZero() : cpu->setZero();
    cpu->addToCycles(2);
    cpu->incPC();
    return;
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
}

void BCC(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->addToCycles(2);
  cpu->C() ? cpu->incPC(2) : cpu->setPC(cpu->PC() +
                                        static_cast<int8_t>(operand));
}

void BCS(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->addToCycles(2);
  cpu->C() ? cpu->setPC(cpu->PC() + static_cast<int8_t>(operand)) :
             cpu->incPC(2);
}

void BEQ(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->addToCycles(2);
  cpu->Z() ? cpu->setPC(cpu->PC() + static_cast<int8_t>(operand)) :
             cpu->incPC(2);
}

void BIT(Cpu *cpu, uint16_t operand, AddressMode mode)
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
}

void BMI(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->addToCycles(2);
  cpu->N() ? cpu->setPC(cpu->PC() + static_cast<int8_t>(operand)) :
             cpu->incPC(2);
}

void BNE(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->addToCycles(2);
  cpu->Z() ? cpu->incPC(2) : cpu->setPC(cpu->PC() +
                                        static_cast<int8_t>(operand));
}

void BPL(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->addToCycles(2);
  cpu->N() ? cpu->incPC(2) : cpu->setPC(cpu->PC() +
                                        static_cast<int8_t>(operand));
}

void BRK(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  uint16_t pc = cpu->PC() + 1;
  cpu->writeToStack((pc >> 8) & 0x00FF);
  cpu->setSP(cpu->SP() - 1);
  cpu->writeToStack(pc & 0x00FF);
  cpu->setSP(cpu->SP() - 1);
  cpu->writeToStack(cpu->SR() | BREAK_MASK);
  cpu->setSP(cpu->SP() - 1);
  cpu->addToCycles(7);
  cpu->setPC(cpu->getMemWord(IRQ_BRK_IV));
}

void BVC(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->addToCycles(2);
  cpu->V() ? cpu->incPC(2) : cpu->setPC(cpu->PC() +
                                        static_cast<int8_t>(operand));
}

void BVS(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->addToCycles(2);
  cpu->V() ? cpu->setPC(cpu->PC() + static_cast<int8_t>(operand)) :
             cpu->incPC(2);
}

void CLC(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->addToCycles(2);
  cpu->clearCarry();
  cpu->incPC();
}

void CLD(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->addToCycles(2);
  cpu->clearDecimal();
  cpu->incPC();
}

void CLI(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->addToCycles(2);
  cpu->clearIMask();
  cpu->incPC();
}

void CLV(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->addToCycles(2);
  cpu->clearOverflow();
  cpu->incPC();
}

void CMP(Cpu *cpu, uint16_t operand, AddressMode mode)
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
}

void CPX(Cpu *cpu, uint16_t operand, AddressMode mode)
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
}

void CPY(Cpu *cpu, uint16_t operand, AddressMode mode)
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
}

void DEC(Cpu *cpu, uint16_t operand, AddressMode mode)
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
}

void DEX(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->setX(cpu->X() - 1);
  cpu->addToCycles(2);
  cpu->X() & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  cpu->X() ? cpu->clearZero() : cpu->setZero();
  cpu->incPC();
}

void DEY(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->setY(cpu->Y() - 1);
  cpu->addToCycles(2);
  cpu->Y() & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  cpu->Y() ? cpu->clearZero() : cpu->setZero();
  cpu->incPC();
}

void EOR(Cpu *cpu, uint16_t operand, AddressMode mode)
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
}

void INC(Cpu *cpu, uint16_t operand, AddressMode mode)
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
}

void INX(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->setX(cpu->X() + 1);
  cpu->addToCycles(2);
  cpu->X() & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  cpu->X() ? cpu->clearZero() : cpu->setZero();
  cpu->incPC();
}

void INY(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->setY(cpu->Y() + 1);
  cpu->addToCycles(2);
  cpu->Y() & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  cpu->Y() ? cpu->clearZero() : cpu->setZero();
  cpu->incPC();
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

void JSR(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  uint16_t returnAddress = cpu->PC() + 2;
  cpu->writeToStack((returnAddress >> 8) & 0x00FF);
  cpu->setSP(cpu->SP() - 1);
  cpu->writeToStack(returnAddress & 0x00FF);
  cpu->setSP(cpu->SP() - 1);
  cpu->setPC(operand);
  cpu->addToCycles(6);
}

void LDA(Cpu *cpu, uint16_t operand, AddressMode mode)
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
}

void LDX(Cpu *cpu, uint16_t operand, AddressMode mode)
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
}

void LDY(Cpu *cpu, uint16_t operand, AddressMode mode)
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
}

void LSR(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  uint8_t* memBase = cpu->getMemBase();
  uint8_t offset = 0;
  uint8_t a = cpu->A();
  switch (mode) {
  case ACC:
    a & 0x01 ? cpu->setCarry() : cpu->clearCarry();

    cpu->setA((a >> 1) & 0x7F);

    cpu->A() ? cpu->clearZero() : cpu->setZero();
    cpu->addToCycles(2);
    cpu->incPC();
    return;
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
}

void NOP(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->addToCycles(2);
  cpu->incPC();
}

void ORA(Cpu *cpu, uint16_t operand, AddressMode mode)
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
}

void PHA(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->writeToStack(cpu->A());
  cpu->setSP(cpu->SP() - 1);
  cpu->addToCycles(3);
  cpu->incPC();
}

void PHP(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->writeToStack(cpu->SR());
  cpu->setSP(cpu->SP() - 1);
  cpu->addToCycles(3);
  cpu->incPC();
}

void PLA(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->setSP(cpu->SP() + 1);
  cpu->setA(cpu->getMemByte(cpu->SP()));

  cpu->A() & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  cpu->A() ? cpu->clearZero() : cpu->setZero();

  cpu->addToCycles(4);
  cpu->incPC();
}

void PLP(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->setSP(cpu->SP() + 1);
  cpu->setSR(cpu->getMemByte(cpu->SP()));

  cpu->addToCycles(4);
  cpu->incPC();
}

void ROL(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  uint8_t c = cpu->C();
  uint8_t a = cpu->A();
  uint16_t offset = 0;
  uint8_t *memBase = cpu->getMemBase();

  switch(mode) {
  case ACC:
    a & 0x80 ? cpu->setCarry() : cpu->clearCarry();
    cpu->setA((a << 1) | c);

    cpu->A() & 0x80 ? cpu->setNegative() : cpu->clearNegative();
    cpu->A() ? cpu->clearZero() : cpu->setZero();

    cpu->addToCycles(2);
    cpu->incPC();
    return;
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
  *(memBase + offset) = (*(memBase + offset) << 1) | c;

  *(memBase + offset) & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  *(memBase + offset) ? cpu->clearZero() : cpu->setZero();

  cpu->incPC(2);
}

void ROR(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  uint8_t c = cpu->C();
  uint8_t a = cpu->A();
  uint16_t offset = 0;
  uint8_t *memBase = cpu->getMemBase();

  switch(mode) {
  case ACC:
    a & 0x01 ? cpu->setCarry() : cpu->clearCarry();
    cpu->setA((a >> 1) | (c << 7));

    cpu->A() & 0x80 ? cpu->setNegative() : cpu->clearNegative();
    cpu->A() ? cpu->clearZero() : cpu->setZero();

    cpu->addToCycles(2);
    cpu->incPC();
    return;
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
  *(memBase + offset) = (*(memBase + offset) >> 1) | (c << 7);

  *(memBase + offset) & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  *(memBase + offset) ? cpu->clearZero() : cpu->setZero();

  cpu->incPC(2);
}

void RTI(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  uint16_t returnAddress = 0;
  cpu->setSP(cpu->SP() + 1);
  cpu->setSR(cpu->getMemByte(cpu->SP()));
  cpu->setSP(cpu->SP() + 1);
  returnAddress |= cpu->getMemByte(cpu->SP());
  cpu->setSP(cpu->SP() + 1);
  returnAddress |= static_cast<uint16_t>(cpu->getMemByte(cpu->SP())) << 8;
  cpu->setPC(returnAddress);
  cpu->addToCycles(6);
}

void RTS(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  uint16_t returnAddress = 0;
  cpu->setSP(cpu->SP() + 1);
  returnAddress |= cpu->getMemByte(cpu->SP());
  cpu->setSP(cpu->SP() + 1);
  returnAddress |= static_cast<uint16_t>(cpu->getMemByte(cpu->SP())) << 8;
  cpu->setPC(returnAddress + 1);
  cpu->addToCycles(6);
}

void SBC(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  uint8_t a = cpu->A();
  uint8_t subber = 0;

  switch(mode) {
  case IMM:
    subber = operand;
    cpu->addToCycles(2);
    break;
  case ZER:
    subber = cpu->getMemByte(operand);
    cpu->addToCycles(3);
    break;
  case IDX1:
    subber = cpu->getMemByte(static_cast<int8_t>(operand) + cpu->X());
    cpu->addToCycles(4);
    break;
  case IDX1_PRE:
    subber = cpu->getMemByte(cpu->getMemWord(operand + cpu->X()));
    cpu->addToCycles(6);
    break;
  case IDX1_POST:
    subber = cpu->getMemByte(cpu->getMemWord(operand) + cpu->Y());
    cpu->addToCycles(5);
    break;
  case ABS:
    subber = cpu->getMemByte(operand);
    cpu->addToCycles(4);
    cpu->incPC();
    break;
  case IDX2_X:
    subber = cpu->getMemByte(static_cast<int16_t>(operand) + cpu->X());
    cpu->addToCycles(4);
    cpu->incPC();
    break;
  case IDX2_Y:
    subber = cpu->getMemByte(static_cast<int16_t>(operand) + cpu->Y());
    cpu->addToCycles(4);
    cpu->incPC();
    break;
  default:
    subber = 0;
  }

  cpu->setA(a - subber - cpu->C());
  cpu->A() > a ? cpu->setCarry() : cpu->clearCarry();
  cpu->A() & 0x80 ? cpu->setNegative() : cpu->clearNegative();

  (a > 127 && subber > 127 && cpu->A() < 128) ||
  (a < 128 && subber < 128 && cpu->A() > 127) ?
  cpu->setOverflow() : cpu->clearOverflow();

  cpu->A() ? cpu->clearZero() : cpu->setZero();

  cpu->incPC(2);
}

void SEC(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->setCarry();
  cpu->addToCycles(2);
  cpu->incPC();
}

void SED(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->setDecimal();
  cpu->addToCycles(2);
  cpu->incPC();
}

void SEI(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->setIMask();
  cpu->addToCycles(2);
  cpu->incPC();
}

void STA(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  uint16_t offset = 0;
  uint8_t *memBase = cpu->getMemBase();

  switch(mode) {
  case ZER:
    offset = operand;
    cpu->addToCycles(3);
    break;
  case IDX1:
    offset = static_cast<int8_t>(operand) + cpu->X();
    cpu->addToCycles(4);
    break;
  case IDX1_PRE:
    offset = cpu->getMemWord(operand + cpu->X());
    cpu->addToCycles(6);
    break;
  case IDX1_POST:
    offset = cpu->getMemWord(operand) + cpu->Y();
    cpu->addToCycles(6);
    break;
  case ABS:
    offset = operand;
    cpu->addToCycles(4);
    cpu->incPC();
    break;
  case IDX2_X:
    offset = static_cast<int16_t>(operand) + cpu->X();
    cpu->addToCycles(5);
    cpu->incPC();
    break;
  case IDX2_Y:
    offset = static_cast<int16_t>(operand) + cpu->Y();
    cpu->addToCycles(5);
    cpu->incPC();
    break;
  default:
    offset = 0;
  }

  *(memBase + offset) = cpu->A();

  cpu->incPC(2);
}

void STX(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  uint16_t offset = 0;
  uint8_t *memBase = cpu->getMemBase();

  switch(mode) {
  case ZER:
    offset = operand;
    cpu->addToCycles(3);
    break;
  case IDX1:
    offset = static_cast<int8_t>(operand) + cpu->Y();
    cpu->addToCycles(4);
    break;
  case ABS:
    offset = operand;
    cpu->addToCycles(4);
    cpu->incPC();
    break;
  default:
    offset = 0;
  }

  *(memBase + offset) = cpu->X();

  cpu->incPC(2);
}

void STY(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  uint16_t offset = 0;
  uint8_t *memBase = cpu->getMemBase();

  switch(mode) {
  case ZER:
    offset = operand;
    cpu->addToCycles(3);
    break;
  case IDX1:
    offset = static_cast<int8_t>(operand) + cpu->X();
    cpu->addToCycles(4);
    break;
  case ABS:
    offset = operand;
    cpu->addToCycles(4);
    cpu->incPC();
    break;
  default:
    offset = 0;
  }

  *(memBase + offset) = cpu->Y();

  cpu->incPC(2);
}

void TAX(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->setX(cpu->A());
  cpu->X() & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  cpu->X() ? cpu->clearZero() : cpu->setZero();
  cpu->addToCycles(2);
  cpu->incPC();
}

void TAY(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->setY(cpu->A());
  cpu->Y() & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  cpu->Y() ? cpu->clearZero() : cpu->setZero();
  cpu->addToCycles(2);
  cpu->incPC();
}

void TSX(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->setX(cpu->SP());
  cpu->X() & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  cpu->X() ? cpu->clearZero() : cpu->setZero();
  cpu->addToCycles(2);
  cpu->incPC();

}

void TXA(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->setA(cpu->X());
  cpu->A() & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  cpu->A() ? cpu->clearZero() : cpu->setZero();
  cpu->addToCycles(2);
  cpu->incPC();
}

void TXS(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->setSP(cpu->X());
  cpu->SP() & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  cpu->SP() ? cpu->clearZero() : cpu->setZero();
  cpu->addToCycles(2);
  cpu->incPC();
}

void TYA(Cpu *cpu, uint16_t operand, AddressMode mode)
{
  cpu->setA(cpu->Y());
  cpu->A() & 0x80 ? cpu->setNegative() : cpu->clearNegative();
  cpu->A() ? cpu->clearZero() : cpu->setZero();
  cpu->addToCycles(2);
  cpu->incPC();
}
