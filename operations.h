///////////////////////////////////////////////////////////////////////////////
// operations.h
//
// Header file containing all operation functions
//
// Author: Alex Busman
// Date: July 10, 2015
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

void ADC(Cpu *cpu, uint16_t operand, AddressMode mode);

void AND(Cpu *cpu, uint16_t operand, AddressMode mode);

void ASL(Cpu *cpu, uint16_t operand, AddressMode mode);

void BCC(Cpu *cpu, uint16_t operand, AddressMode mode);

void BCS(Cpu *cpu, uint16_t operand, AddressMode mode);

void BEQ(Cpu *cpu, uint16_t operand, AddressMode mode);

void BIT(Cpu *cpu, uint16_t operand, AddressMode mode);

void BMI(Cpu *cpu, uint16_t operand, AddressMode mode);

void BNE(Cpu *cpu, uint16_t operand, AddressMode mode);

void BPL(Cpu *cpu, uint16_t operand, AddressMode mode);

void BRK(Cpu *cpu, uint16_t operand, AddressMode mode);

void BVC(Cpu *cpu, uint16_t operand, AddressMode mode);

void BVS(Cpu *cpu, uint16_t operand, AddressMode mode);

void CLC(Cpu *cpu, uint16_t operand, AddressMode mode);

void CLD(Cpu *cpu, uint16_t operand, AddressMode mode);

void CLI(Cpu *cpu, uint16_t operand, AddressMode mode);

void CLV(Cpu *cpu, uint16_t operand, AddressMode mode);

void CMP(Cpu *cpu, uint16_t operand, AddressMode mode);

void CPX(Cpu *cpu, uint16_t operand, AddressMode mode);

void CPY(Cpu *cpu, uint16_t operand, AddressMode mode);

void DEC(Cpu *cpu, uint16_t operand, AddressMode mode);

void DEX(Cpu *cpu, uint16_t operand, AddressMode mode);

void DEY(Cpu *cpu, uint16_t operand, AddressMode mode);

void EOR(Cpu *cpu, uint16_t operand, AddressMode mode);

void INC(Cpu *cpu, uint16_t operand, AddressMode mode);

void INX(Cpu *cpu, uint16_t operand, AddressMode mode);

void INY(Cpu *cpu, uint16_t operand, AddressMode mode);

void JMP(Cpu *cpu, uint16_t operand, AddressMode mode);

void JSR(Cpu *cpu, uint16_t operand, AddressMode mode);

void LDA(Cpu *cpu, uint16_t operand, AddressMode mode);

void LDX(Cpu *cpu, uint16_t operand, AddressMode mode);

void LDY(Cpu *cpu, uint16_t operand, AddressMode mode);

void LSR(Cpu *cpu, uint16_t operand, AddressMode mode);

void NOP(Cpu *cpu, uint16_t operand, AddressMode mode);

void ORA(Cpu *cpu, uint16_t operand, AddressMode mode);

void PHA(Cpu *cpu, uint16_t operand, AddressMode mode);

void PHP(Cpu *cpu, uint16_t operand, AddressMode mode);

void PLA(Cpu *cpu, uint16_t operand, AddressMode mode);

void PLP(Cpu *cpu, uint16_t operand, AddressMode mode);

void ROL(Cpu *cpu, uint16_t operand, AddressMode mode);

void ROR(Cpu *cpu, uint16_t operand, AddressMode mode);

void RTI(Cpu *cpu, uint16_t operand, AddressMode mode);

void RTS(Cpu *cpu, uint16_t operand, AddressMode mode);

void SBC(Cpu *cpu, uint16_t operand, AddressMode mode);

void SEC(Cpu *cpu, uint16_t operand, AddressMode mode);

void SED(Cpu *cpu, uint16_t operand, AddressMode mode);

void SEI(Cpu *cpu, uint16_t operand, AddressMode mode);

void STA(Cpu *cpu, uint16_t operand, AddressMode mode);

void STX(Cpu *cpu, uint16_t operand, AddressMode mode);

void STY(Cpu *cpu, uint16_t operand, AddressMode mode);

void TAX(Cpu *cpu, uint16_t operand, AddressMode mode);

void TAY(Cpu *cpu, uint16_t operand, AddressMode mode);

void TSX(Cpu *cpu, uint16_t operand, AddressMode mode);

void TXA(Cpu *cpu, uint16_t operand, AddressMode mode);

void TXS(Cpu *cpu, uint16_t operand, AddressMode mode);

void TYA(Cpu *cpu, uint16_t operand, AddressMode mode);

#endif // OPERATIONS_H
