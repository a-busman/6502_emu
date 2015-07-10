///////////////////////////////////////////////////////////////////////////////
// cpu.cpp
//
// Implimentation of Cpu class
//
// Author: Alex Busman
// Date: July 10, 2015
///////////////////////////////////////////////////////////////////////////////

#include "cpu.h"

void Cpu::ADC(uint16_t operand, AddressMode mode)
{
  uint8_t a = _a;
  uint8_t adder = 0;

  switch(mode) {
  case IMM:
    adder = operand;
    _cycles += 2;
    _pc += 2;
    break;
  case ZER:
    adder = _mem[operand];
    _cycles += 3;
    _pc += 2;
    break;
  case IDX1:
    adder = _mem[static_cast<int8_t>(operand) + _x];
    _cycles += 4;
    _pc += 2;
    break;
  case IDX1_PRE:
    adder = _mem[getMemWord(operand + _x)];
    _cycles += 6;
    _pc += 2;
    break;
  case IDX1_POST:
    adder = _mem[getMemWord(operand) + _y];
    _cycles += 5;
    _pc += 2;
    break;
  case ABS:
    adder = _mem[operand];
    _cycles += 4;
    _pc += 3;
    break;
  case IDX2_X:
    adder = _mem[static_cast<int16_t>(operand) + _x];
    _cycles += 4;
    _pc += 3;
    break;
  case IDX2_Y:
    adder = _mem[static_cast<int16_t>(operand) + _y];
    _cycles += 4;
    _pc += 3;
    break;
  default:
    adder = 0;
  }
  _a = a + adder + C();
  _a < a ? setCarry() : clearCarry();
  _a & 0x80 ? setNegative() : clearNegative();

  (a > 127 && adder > 127 && _a < 128) || (a < 128 && adder < 128 && _a > 127) ?
  setOverflow() : clearOverflow();

  _a ? clearZero() : setZero();
}

void Cpu::AND(uint16_t operand, AddressMode mode)
{
  uint8_t ander = 0;

  switch(mode) {
  case IMM:
    ander = operand;
    _cycles += 2;
    _pc += 2;
    break;
  case ZER:
    ander = _mem[operand];
    _cycles += 3;
    _pc += 2;
    break;
  case IDX1:
    ander = _mem[static_cast<int8_t>(operand) + _x];
    _cycles += 4;
    _pc += 2;
    break;
  case IDX1_PRE:
    ander = _mem[getMemWord(operand + _x)];
    _cycles += 6;
    _pc += 2;
    break;
  case IDX1_POST:
    ander = _mem[getMemWord(operand) + _y];
    _cycles += 5;
    _pc += 2;
    break;
  case ABS:
    ander = _mem[operand];
    _cycles += 4;
    _pc += 3;
    break;
  case IDX2_X:
    ander = _mem[static_cast<int16_t>(operand) + _x];
    _cycles += 4;
    _pc += 3;
    break;
  case IDX2_Y:
    ander = _mem[static_cast<int16_t>(operand) + _y];
    _cycles += 4;
    _pc += 3;
    break;
  default:
    ander = 0;
  }
  _a &= ander;

  _a & 0x80 ? setNegative() : clearNegative();
  _a ? clearZero() : setZero();
}

void Cpu::ASL(uint16_t operand, AddressMode mode)
{
  uint8_t offset = 0;
  switch (mode) {
  case ACC:
    _a & 0x80 ? setCarry() : clearCarry();
    _a <<= 1;

    _a & 0x80 ? setNegative() : clearNegative();
    _a ? clearZero() : setZero();
    _cycles += 2;
    _pc++;
    return;
  case ZER:
    offset = operand;
    _cycles += 5;
    _pc += 2;
    break;
  case IDX1:
    offset = static_cast<int8_t>(operand) + _x;
    _cycles += 6;
    _pc += 2;
    break;
  case ABS:
    offset = operand;
    _cycles += 6;
    _pc += 3;
    break;
  case IDX2_X:
    offset = static_cast<int16_t>(operand) + _x;
    _cycles += 7;
    _pc += 3;
    break;
  default:
    offset = 0;
  }
  _mem[offset] & 0x80 ? setCarry() : clearCarry();
  _mem[offset] <<= 1;

  _mem[offset] & 0x80 ? setNegative() : clearNegative();
  _mem[offset] ? clearZero() : setZero();
}

void Cpu::BCC(uint16_t operand, AddressMode mode)
{
  _cycles += 2;
  C() ? _pc += 2 : _pc += static_cast<int8_t>(operand);
}

void Cpu::BCS(uint16_t operand, AddressMode mode)
{
  _cycles += 2;
  C() ? _pc += static_cast<int8_t>(operand) : _pc += 2;
}

void Cpu::BEQ(uint16_t operand, AddressMode mode)
{
  _cycles += 2;
  Z() ? _pc += static_cast<int8_t>(operand) : _pc += 2;
}

void Cpu::BIT(uint16_t operand, AddressMode mode)
{
  uint8_t ander = _mem[operand];
  uint8_t res = _a & ander;
  switch(mode) {
  case ZER:
    _cycles += 3;
    _pc += 2;
    break;
  case ABS:
    _cycles += 4;
    _pc += 3;
    break;
  default:
    break;
  }

  res & 0x80 ? setNegative() : clearNegative();

  (_a > 127 && ander > 127 && res < 128) ||
  (_a < 128 && ander < 128 && res > 127) ?
  setOverflow() : clearOverflow();

  res ? clearZero() : setZero();
}

void Cpu::BMI(uint16_t operand, AddressMode mode)
{
  _cycles += 2;
  N() ? _pc += static_cast<int8_t>(operand) : _pc += 2;
}

void Cpu::BNE(uint16_t operand, AddressMode mode)
{
  _cycles += 2;
  Z() ? _pc += 2 : _pc += static_cast<int8_t>(operand);
}

void Cpu::BPL(uint16_t operand, AddressMode mode)
{
  _cycles += 2;
  N() ? _pc += 2 : _pc += static_cast<int8_t>(operand);
}

void Cpu::BRK(uint16_t operand, AddressMode mode)
{
  uint16_t pc = _pc + 1;
  _mem[_sp] = (pc >> 8) & 0x00FF;
  _sp--;
  _mem[_sp] = pc & 0x00FF;
  _sp--;
  _mem[_sp] = _sr | BREAK_MASK;
  _sp--;
  _cycles += 7;
  _pc = getMemWord(IRQ_BRK_IV);
}

void Cpu::BVC(uint16_t operand, AddressMode mode)
{
  _cycles += 2;
  V() ? _pc += 2 : _pc += static_cast<int8_t>(operand);
}

void Cpu::BVS(uint16_t operand, AddressMode mode)
{
  _cycles += 2;
  V() ? _pc += static_cast<int8_t>(operand) : _pc += 2;
}

void Cpu::CLC(uint16_t operand, AddressMode mode)
{
  clearCarry();
  _cycles += 2;
  _pc++;
}

void Cpu::CLD(uint16_t operand, AddressMode mode)
{
  clearDecimal();
  _cycles += 2;
  _pc++;
}

void Cpu::CLI(uint16_t operand, AddressMode mode)
{
  clearIMask();
  _cycles += 2;
  _pc++;
}

void Cpu::CLV(uint16_t operand, AddressMode mode)
{
  clearOverflow();
  _cycles += 2;
  _pc++;
}

void Cpu::CMP(uint16_t operand, AddressMode mode)
{
  uint8_t comparitor = 0;
  uint8_t res = 0;

  switch(mode) {
  case IMM:
    comparitor = operand;
    _cycles += 2;
    _pc += 2;
    break;
  case ZER:
    comparitor = _mem[operand];
    _cycles += 3;
    _pc += 2;
    break;
  case IDX1:
    comparitor = _mem[static_cast<int8_t>(operand) + _x];
    _cycles += 4;
    _pc += 2;
    break;
  case IDX1_PRE:
    comparitor = _mem[getMemWord(operand + _x)];
    _cycles += 6;
    _pc += 2;
    break;
  case IDX1_POST:
    comparitor = _mem[getMemWord(operand) + _y];
    _cycles += 5;
    _pc += 2;
    break;
  case ABS:
    comparitor = _mem[operand];
    _cycles += 4;
    _pc += 3;
    break;
  case IDX2_X:
    comparitor = _mem[static_cast<int16_t>(operand) + _x];
    _cycles += 4;
    _pc += 3;
    break;
  case IDX2_Y:
    comparitor = _mem[static_cast<int16_t>(operand) + _y];
    _cycles += 4;
    _pc += 3;
    break;
  default:
    comparitor = 0;
  }
  res = _a - comparitor;

  res & 0x80 ? setNegative() : clearNegative();

  (_a > 127 && comparitor > 127 && res < 128) ||
  (_a < 128 && comparitor < 128 && res > 127) ?
  setOverflow() : clearOverflow();

  res ? clearZero() : setZero();
}

void Cpu::CPX(uint16_t operand, AddressMode mode)
{
  uint8_t comparitor = 0;
  uint8_t res = 0;

  switch(mode) {
  case IMM:
    comparitor = operand;
    _cycles += 2;
    _pc += 2;
    break;
  case ZER:
    comparitor = _mem[operand];
    _cycles += 3;
    _pc += 2;
    break;
  case ABS:
    comparitor = _mem[operand];
    _cycles += 4;
    _pc += 3;
  default:
    comparitor = 0;
  }
  res = _x - comparitor;

  res & 0x80 ? setNegative() : clearNegative();

  (_x > 127 && comparitor > 127 && res < 128) ||
  (_x < 128 && comparitor < 128 && res > 127) ?
  setOverflow() : clearOverflow();

  res ? clearZero() : setZero();
}

void Cpu::CPY(uint16_t operand, AddressMode mode)
{
  uint8_t comparitor = 0;
  uint8_t res = 0;

  switch(mode) {
  case IMM:
    comparitor = operand;
    _cycles += 2;
    _pc += 2;
    break;
  case ZER:
    comparitor = _mem[operand];
    _cycles += 3;
    _pc += 2;
    break;
  case ABS:
    comparitor = _mem[operand];
    _cycles += 4;
    _pc += 3;
  default:
    comparitor = 0;
  }
  res = _y - comparitor;

  res & 0x80 ? setNegative() : clearNegative();

  (_y > 127 && comparitor > 127 && res < 128) ||
  (_y < 128 && comparitor < 128 && res > 127) ?
  setOverflow() : clearOverflow();

  res ? clearZero() : setZero();
}

void Cpu::DEC(uint16_t operand, AddressMode mode)
{
  uint8_t address = 0;

  switch(mode) {
  case ZER:
    address = operand;
    _cycles += 5;
    _pc += 2;
    break;
  case IDX1:
    address = static_cast<int8_t>(operand) + _x;
    _cycles += 6;
    _pc += 2;
    break;
  case ABS:
    address = operand;
    _cycles += 6;
    _pc += 3;
    break;
  case IDX2_X:
    address = static_cast<int16_t>(operand) + _x;
    _cycles += 7;
    _pc += 3;
    break;
  default:
    address = 0;
  }

  _mem[address] -= 1;

  _mem[address] & 0x80 ? setNegative() : clearNegative();
  _mem[address] ? clearZero() : setZero();
}

void Cpu::DEX(uint16_t operand, AddressMode mode)
{
  _x--;
  _cycles += 2;
  _x & 0x80 ? setNegative() : clearNegative();
  _x ? clearZero() : setZero();
  _pc++;
}

void Cpu::DEY(uint16_t operand, AddressMode mode)
{
  _y--;
  _cycles += 2;
  _y & 0x80 ? setNegative() : clearNegative();
  _y ? clearZero() : setZero();
  _pc++;
}

void Cpu::EOR(uint16_t operand, AddressMode mode)
{
  uint8_t eorer = 0;

  switch (mode) {
  case IMM:
    eorer = operand;
    _cycles += 2;
    _pc += 2;
    break;
  case ZER:
    eorer = _mem[operand];
    _cycles += 3;
    _pc += 2;
    break;
  case IDX1:
    eorer = _mem[static_cast<int8_t>(operand) + _x];
    _cycles += 4;
    _pc += 2;
    break;
  case IDX1_PRE:
    eorer = _mem[getMemWord(operand + _x)];
    _cycles += 6;
    _pc += 2;
    break;
  case IDX1_POST:
    eorer = _mem[getMemWord(operand) + _y];
    _cycles += 5;
    _pc += 2;
    break;
  case ABS:
    eorer = _mem[operand];
    _cycles += 4;
    _pc += 3;
    break;
  case IDX2_X:
    eorer = _mem[static_cast<int16_t>(operand) + _x];
    _cycles += 4;
    _pc += 3;
    break;
  case IDX2_Y:
    eorer = _mem[static_cast<int16_t>(operand) + _y];
    _cycles += 4;
    _pc += 3;
    break;
  default:
    eorer = 0;
  }

  _a ^= eorer;

  _a & 0x80 ? setNegative() : clearNegative();
  _a ? clearZero() : setZero();
}

void Cpu::INC(uint16_t operand, AddressMode mode)
{
  uint8_t address = 0;

  switch(mode) {
  case ZER:
    address = operand;
    _cycles += 5;
    _pc += 2;
    break;
  case IDX1:
    address = static_cast<int8_t>(operand) + _x;
    _cycles += 6;
    _pc += 2;
    break;
  case ABS:
    address = operand;
    _cycles += 6;
    _pc += 3;
    break;
  case IDX2_X:
    address = static_cast<int16_t>(operand) + _x;
    _cycles += 7;
    _pc += 3;
    break;
  default:
    address = 0;
  }
  _mem[address]++;

  _mem[address] & 0x80 ? setNegative() : clearNegative();
  _mem[address] ? clearZero() : setZero();
}

void Cpu::INX(uint16_t operand, AddressMode mode)
{
  _x++;
  _cycles += 2;
  _pc++;
  _x & 0x80 ? setNegative() : clearNegative();
  _x ? clearZero() : setZero();
}

void Cpu::INY(uint16_t operand, AddressMode mode)
{
  _y++;
  _cycles += 2;
  _pc++;
  _y & 0x80 ? setNegative() : clearNegative();
  _y ? clearZero() : setZero();
}

void Cpu::JMP(uint16_t operand, AddressMode mode)
{
  switch(mode) {
  case ABS:
    _cycles += 3;
    _pc = operand;
    break;
  case IND:
    _cycles += 5;
    _pc = getMemWord(operand);
    break;
  default:
    break;
  }
}

void Cpu::JSR(uint16_t operand, AddressMode mode)
{
  uint16_t returnAddress = _pc + 2;
  _mem[_sp] = (returnAddress >> 8) & 0x00FF;
  _sp--;
  _mem[_sp] = returnAddress & 0x00FF;
  _sp--;
  _pc = operand;
  _cycles += 6;
}

void Cpu::LDA(uint16_t operand, AddressMode mode)
{
  switch (mode) {
  case IMM:
    _a = operand;
    _cycles += 2;
    _pc += 2;
    break;
  case ZER:
    _a = _mem[operand];
    _cycles += 3;
    _pc += 2;
    break;
  case IDX1:
    _a = _mem[static_cast<int8_t>(operand) + _x];
    _cycles += 4;
    _pc += 2;
    break;
  case IDX1_PRE:
    _a = _mem[getMemWord(operand + _x)];
    _cycles += 6;
    _pc += 2;
    break;
  case IDX1_POST:
    _a = _mem[getMemWord(operand) + _y];
    _cycles += 5;
    _pc += 2;
    break;
  case ABS:
    _a = _mem[operand];
    _cycles += 4;
    _pc += 3;
    break;
  case IDX2_X:
    _a = _mem[static_cast<int16_t>(operand) + _x];
    _cycles += 4;
    _pc += 3;
    break;
  case IDX2_Y:
    _a = _mem[static_cast<int16_t>(operand) + _y];
    _cycles += 4;
    _pc += 3;
    break;
  default:
    break;
  }
  _a & 0x80 ? setNegative() : clearNegative();
  _a ? clearZero() : setZero();
}

void Cpu::LDX(uint16_t operand, AddressMode mode)
{
  switch (mode) {
  case IMM:
    _x = operand;
    _cycles += 2;
    _pc += 2;
    break;
  case ZER:
    _x = _mem[operand];
    _cycles += 3;
    _pc += 2;
    break;
  case IDX1:
    _x = _mem[static_cast<int8_t>(operand) + _y];
    _cycles += 4;
    _pc += 2;
    break;
  case ABS:
    _x = _mem[operand];
    _cycles += 4;
    _pc += 3;
    break;
  case IDX2_Y:
    _x = _mem[static_cast<int16_t>(operand) + _y];
    _cycles += 4;
    _pc += 3;
    break;
  default:
    break;
  }
  _x & 0x80 ? setNegative() : clearNegative();
  _x ? clearZero() : setZero();
}

void Cpu::LDY(uint16_t operand, AddressMode mode)
{
  switch (mode) {
  case IMM:
    _y = operand;
    _cycles += 2;
    _pc += 2;
    break;
  case ZER:
    _y = _mem[operand];
    _cycles += 3;
    _pc += 2;
    break;
  case IDX1:
    _y = _mem[static_cast<int8_t>(operand) + _x];
    _cycles += 4;
    _pc += 2;
    break;
  case ABS:
    _y = _mem[operand];
    _cycles += 4;
    _pc += 3;
    break;
  case IDX2_Y:
    _y = _mem[static_cast<int16_t>(operand) + _x];
    _cycles += 4;
    _pc += 3;
    break;
  default:
    break;
  }
  _y & 0x80 ? setNegative() : clearNegative();
  _y ? clearZero() : setZero();
}

void Cpu::LSR(uint16_t operand, AddressMode mode)
{
  uint8_t offset = 0;
  switch (mode) {
  case ACC:
    _a & 0x01 ? setCarry() : clearCarry();

    _a = (_a >> 1) & 0x7F;

    _a ? clearZero() : setZero();
    _cycles += 2;
    _pc++;
    return;
  case ZER:
    offset = operand;
    _cycles += 5;
    _pc += 2;
    break;
  case IDX1:
    offset = static_cast<int8_t>(operand) + _x;
    _cycles += 6;
    _pc += 2;
    break;
  case ABS:
    offset = operand;
    _cycles += 6;
    _pc += 3;
    break;
  case IDX2_X:
    offset = static_cast<int16_t>(operand) + _x;
    _cycles += 7;
    _pc += 3;
    break;
  default:
    offset = 0;
  }
  _mem[offset] & 0x01 ? setCarry() : clearCarry();
  _mem[offset] >>= 1;

  _mem[offset] ? clearZero() : setZero();
}

void Cpu::NOP(uint16_t operand, AddressMode mode)
{
  _cycles += 2;
  _pc++;
}

void Cpu::ORA(uint16_t operand, AddressMode mode)
{
  uint8_t orer = 0;

  switch(mode) {
  case IMM:
    orer = operand;
    _cycles += 2;
    _pc += 2;
    break;
  case ZER:
    orer = _mem[operand];
    _cycles += 3;
    _pc += 2;
    break;
  case IDX1:
    orer = _mem[static_cast<int8_t>(operand) + _x];
    _cycles += 4;
    _pc += 2;
    break;
  case IDX1_PRE:
    orer = _mem[getMemWord(operand + _x)];
    _cycles += 6;
    _pc += 2;
    break;
  case IDX1_POST:
    orer = _mem[getMemWord(operand) + _y];
    _cycles += 5;
    _pc += 2;
    break;
  case ABS:
    orer = _mem[operand];
    _cycles += 4;
    _pc += 3;
    break;
  case IDX2_X:
    orer = _mem[static_cast<int16_t>(operand) + _x];
    _cycles += 4;
    _pc += 3;
    break;
  case IDX2_Y:
    orer = _mem[static_cast<int16_t>(operand) + _y];
    _cycles += 4;
    _pc += 3;
    break;
  default:
    orer = 0;
  }
  _a |= orer;

  _a & 0x80 ? setNegative() : clearNegative();
  _a ? clearZero() : setZero();
}

void Cpu::PHA(uint16_t operand, AddressMode mode)
{
  _mem[_sp] = _a;
  _sp--;
  _cycles += 3;
  _pc++;
}

void Cpu::PHP(uint16_t operand, AddressMode mode)
{
  _mem[_sp] = _sr;
  _sp--;
  _cycles += 3;
  _pc++;
}

void Cpu::PLA(uint16_t operand, AddressMode mode)
{
  _sp++;
  _a = _mem[_sp];

  _a & 0x80 ? setNegative() : clearNegative();
  _a ? clearZero() : setZero();

  _cycles += 4;
  _pc++;
}

void Cpu::PLP(uint16_t operand, AddressMode mode)
{
  _sp++;
  _sr = _mem[_sp];

  _cycles += 4;
  _pc++;
}

void Cpu::ROL(uint16_t operand, AddressMode mode)
{
  uint8_t c = C();
  uint16_t offset = 0;

  switch(mode) {
  case ACC:
    _a & 0x80 ? setCarry() : clearCarry();
    _a = (_a << 1) | c;

    _a & 0x80 ? setNegative() : clearNegative();
    _a ? clearZero() : setZero();

    _cycles += 2;
    _pc++;
    return;
  case ZER:
    offset = operand;
    _cycles += 5;
    _pc += 2;
    break;
  case IDX1:
    offset = static_cast<int8_t>(operand) + _x;
    _cycles += 6;
    _pc += 2;
    break;
  case ABS:
    offset = operand;
    _cycles += 6;
    _pc += 3;
    break;
  case IDX2_X:
    offset = static_cast<int16_t>(operand) + _x;
    _cycles += 7;
    _pc += 3;
    break;
  default:
    offset = 0;
  }

  _mem[offset] & 0x80 ? setCarry() : clearCarry();
  _mem[offset] = (_mem[offset] << 1) | c;

  _mem[offset] & 0x80 ? setNegative() : clearNegative();
  _mem[offset] ? clearZero() : setZero();
}

void Cpu::ROR(uint16_t operand, AddressMode mode)
{
  uint8_t c = C();
  uint16_t offset = 0;

  switch(mode) {
  case ACC:
    _a & 0x01 ? setCarry() : clearCarry();
    _a = (_a >> 1) | (c << 7);

    _a & 0x80 ? setNegative() : clearNegative();
    _a ? clearZero() : setZero();

    _cycles += 2;
    _pc++;
    return;
  case ZER:
    offset = operand;
    _cycles += 5;
    _pc += 2;
    break;
  case IDX1:
    offset = static_cast<int8_t>(operand) + _x;
    _cycles += 6;
    _pc += 2;
    break;
  case ABS:
    offset = operand;
    _cycles += 6;
    _pc += 3;
    break;
  case IDX2_X:
    offset = static_cast<int16_t>(operand) + _x;
    _cycles += 7;
    _pc += 3;
    break;
  default:
    offset = 0;
  }

  _mem[offset] & 0x01 ? setCarry() : clearCarry();
  _mem[offset] = (_mem[offset] >> 1) | (c << 7);

  _mem[offset] & 0x80 ? setNegative() : clearNegative();
  _mem[offset] ? clearZero() : setZero();
}

void Cpu::RTI(uint16_t operand, AddressMode mode)
{
  uint16_t returnAddress = 0;
  _sp++;
  _sr = _mem[_sp];
  _sp++;
  returnAddress |= _mem[_sp];
  _sp++;
  returnAddress |= static_cast<uint16_t>(_mem[_sp]) << 8;
  _pc = returnAddress;
  _cycles += 6;
}

void Cpu::RTS(uint16_t operand, AddressMode mode)
{
  uint16_t returnAddress = 0;
  _sp++;
  returnAddress |= _mem[_sp];
  _sp++;
  returnAddress |= static_cast<uint16_t>(_mem[_sp]) << 8;
  _pc = returnAddress + 1;
  _cycles += 6;
}

void Cpu::SBC(uint16_t operand, AddressMode mode)
{
  uint8_t a = _a;
  uint8_t subber = 0;

  switch(mode) {
  case IMM:
    subber = operand;
    _cycles += 2;
    _pc += 2;
    break;
  case ZER:
    subber = _mem[operand];
    _cycles += 3;
    _pc += 2;
    break;
  case IDX1:
    subber = _mem[static_cast<int8_t>(operand) + _x];
    _cycles += 4;
    _pc += 2;
    break;
  case IDX1_PRE:
    subber = _mem[getMemWord(operand + _x)];
    _cycles += 6;
    _pc += 2;
    break;
  case IDX1_POST:
    subber = _mem[getMemWord(operand) + _y];
    _cycles += 5;
    _pc += 2;
    break;
  case ABS:
    subber = _mem[operand];
    _cycles += 4;
    _pc += 3;
    break;
  case IDX2_X:
    subber = _mem[static_cast<int16_t>(operand) + _x];
    _cycles += 4;
    _pc += 3;
    break;
  case IDX2_Y:
    subber = _mem[static_cast<int16_t>(operand) + _y];
    _cycles += 4;
    _pc += 3;
    break;
  default:
    subber = 0;
  }

  _a = a - subber - C();
  _a > a ? setCarry() : clearCarry();
  _a & 0x80 ? setNegative() : clearNegative();

  (a > 127 && subber > 127 && _a < 128) ||
  (a < 128 && subber < 128 && _a > 127) ?
  setOverflow() : clearOverflow();

  _a ? clearZero() : setZero();
}

void Cpu::SEC(uint16_t operand, AddressMode mode)
{
  setCarry();
  _cycles += 2;
  _pc++;
}

void Cpu::SED(uint16_t operand, AddressMode mode)
{
  setDecimal();
  _cycles += 2;
  _pc++;
}

void Cpu::SEI(uint16_t operand, AddressMode mode)
{
  setIMask();
  _cycles += 2;
  _pc++;
}

void Cpu::STA(uint16_t operand, AddressMode mode)
{
  uint16_t offset = 0;

  switch(mode) {
  case ZER:
    offset = operand;
    _cycles += 3;
    _pc += 2;
    break;
  case IDX1:
    offset = static_cast<int8_t>(operand) + _x;
    _cycles += 4;
    _pc += 2;
    break;
  case IDX1_PRE:
    offset = getMemWord(operand + _x);
    _cycles += 6;
    _pc += 2;
    break;
  case IDX1_POST:
    offset = getMemWord(operand) + _y;
    _cycles += 6;
    _pc += 2;
    break;
  case ABS:
    offset = operand;
    _cycles += 4;
    _pc += 3;
    break;
  case IDX2_X:
    offset = static_cast<int16_t>(operand) + _x;
    _cycles += 5;
    _pc += 3;
    break;
  case IDX2_Y:
    offset = static_cast<int16_t>(operand) + _y;
    _cycles += 5;
    _pc += 3;
    break;
  default:
    offset = 0;
  }

  _mem[offset] = _a;
}

void Cpu::STX(uint16_t operand, AddressMode mode)
{
  uint16_t offset = 0;

  switch(mode) {
  case ZER:
    offset = operand;
    _cycles += 3;
    _pc += 2;
    break;
  case IDX1:
    offset = static_cast<int8_t>(operand) + _y;
    _cycles += 4;
    _pc += 2;
    break;
  case ABS:
    offset = operand;
    _cycles += 4;
    _pc += 3;
    break;
  default:
    offset = 0;
  }

  _mem[offset] = _x;
}

void Cpu::STY(uint16_t operand, AddressMode mode)
{
  uint16_t offset = 0;

  switch(mode) {
  case ZER:
    offset = operand;
    _cycles += 3;
    _pc += 2;
    break;
  case IDX1:
    offset = static_cast<int8_t>(operand) + _x;
    _cycles += 4;
    _pc += 2;
    break;
  case ABS:
    offset = operand;
    _cycles += 4;
    _pc += 3;
    break;
  default:
    offset = 0;
  }

  _mem[offset] = _y;
}

void Cpu::TAX(uint16_t operand, AddressMode mode)
{
  _x = _a;
  _x & 0x80 ? setNegative() : clearNegative();
  _x ? clearZero() : setZero();
  _cycles += 2;
  _pc++;
}

void Cpu::TAY(uint16_t operand, AddressMode mode)
{
  _y = _a;
  _y & 0x80 ? setNegative() : clearNegative();
  _y ? clearZero() : setZero();
  _cycles += 2;
  _pc++;
}

void Cpu::TSX(uint16_t operand, AddressMode mode)
{
  _x = _sp;
  _x & 0x80 ? setNegative() : clearNegative();
  _x ? clearZero() : setZero();
  _cycles += 2;
  _pc++;

}

void Cpu::TXA(uint16_t operand, AddressMode mode)
{
  _a = _x;
  _a & 0x80 ? setNegative() : clearNegative();
  _a ? clearZero() : setZero();
  _cycles += 2;
  _pc++;
}

void Cpu::TXS(uint16_t operand, AddressMode mode)
{
  _sp = _x;
  _sp & 0x80 ? setNegative() : clearNegative();
  _sp ? clearZero() : setZero();
  _cycles += 2;
  _pc++;
}

void Cpu::TYA(uint16_t operand, AddressMode mode)
{
  _a = _y;
  _a & 0x80 ? setNegative() : clearNegative();
  _a ? clearZero() : setZero();
  _cycles += 2;
  _pc++;
}
