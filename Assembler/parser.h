///////////////////////////////////////////////////////////////////////////////
// parser.h
//
// Header file containing parser class
//
// Author: Alex Busman
// Date: July 21, 2015
///////////////////////////////////////////////////////////////////////////////

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <exception>
#include <map>
#include <cstdint>

using namespace std;

class Parser
{
public:
  struct Line {
    string label;
    string operation;
    string operand;
    string comment;
    uint16_t opcode;
  };

  class ParserErrorException : public exception
  {
    virtual const char* what() const throw()
    {
      return "Syntax error";
    }
  };

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

  Parser();
  ~Parser() {}

  Line parseLine(string line);

private:
  Line _currentLine;
  map<string, map<AddressMode, uint8_t>> _operationMap;
};




#endif // PARSER_H
