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

enum AddressMode{
  ACC,
  IMP,
  IMM,
  ZER,
  ABS,
  REL,
  IDX1,
  IDX2,
  IDX1_IND
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
using namespace std;
class Instruction {
public:
  /*!
   * \brief Default constructor
   */
  Instruction() : _cpu(NULL), _mode(IMP), _operationName(""), _opcode(0),
                  _cycles(0) {}
  /*!
   * \brief Constructor with just Cpu
   * \param cpu Pointer to cpu object
   */
  Instruction(Cpu *cpu) : _cpu(cpu), _mode(IMP), _operationName(""), _opcode(0),
                          _cycles(0) {}
  /*!
   * \brief Constructor with all initializers
   * \param cpu Pointer to cpu object
   * \param mode Addressing mode
   * \param name Operation name (mnemonic)
   * \param opcode Opcode of Operation
   * \param cycles How many cycles operation will take
   * \param operation Function pointer to operation this will perform
   */
  Instruction(Cpu *cpu, AddressMode mode, string name, uint8_t opcode,
              uint8_t cycles) : _cpu(cpu), _mode(mode), _operationName(name),
                                _opcode(opcode), _cycles(cycles) {}
  ~Instruction() {}

  virtual uint8_t operation(uint16_t operand);
protected:
  Cpu *_cpu;
  AddressMode _mode;
  string _operationName;
  uint8_t _opcode;
  uint8_t _cycles;

};

#endif // INSTRUCTION_H
