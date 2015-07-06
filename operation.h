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

struct Instruction {
  uint8_t opcode;
  uint8_t cycles;
  AddressMode mode;

  Instruction() : opcode(0), cycles(0), mode(ACC) {}
  Instruction(uint8_t op, uint8_t cyc, AddressMode m) : opcode(op), cycles(cyc),
              mode(m) {}
};
using namespace std;
class Operation {
public:
  /*!
   * \brief Default constructor
   */
  Operation() : _cpu(NULL), _operation(NULL) {}
  /*!
   * \brief Constructor with just Cpu
   * \param cpu Pointer to cpu object
   */
  Operation(Cpu *cpu) : _cpu(cpu), _operation(NULL) {}
  /*!
   * \brief Constructor with all initializers
   * \param cpu Pointer to cpu object
   * \param operation function pointer to operation
   */
  Operation(Cpu *cpu, uint8_t (*operation)(uint16_t operand)) : _cpu(cpu),
            _operation(operation){}
  ~Operation() {}

private:
  Cpu *_cpu;
  Instruction *_instructions;
  uint8_t (*_operation)(uint16_t operand);
};

#endif // OPERATION_H
