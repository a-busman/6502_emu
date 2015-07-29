#include <vector>
#include <sstream>
#include <cwctype>
#include <clocale>

#include <iostream>

#include "parser.h"

Parser::Parser()
{
  string currentOperation = "adc";
  _operationMap[currentOperation][IMM]       = 0x69;
  _operationMap[currentOperation][ZER]       = 0x65;
  _operationMap[currentOperation][ABS]       = 0x6D;
  _operationMap[currentOperation][IDX1]      = 0x75;
  _operationMap[currentOperation][IDX2_X]    = 0x7D;
  _operationMap[currentOperation][IDX2_Y]    = 0x79;
  _operationMap[currentOperation][IDX1_PRE]  = 0x61;
  _operationMap[currentOperation][IDX1_POST] = 0x71;

  currentOperation = "and";
  _operationMap[currentOperation][IMM]       = 0x29;
  _operationMap[currentOperation][ZER]       = 0x25;
  _operationMap[currentOperation][ABS]       = 0x2D;
  _operationMap[currentOperation][IDX1]      = 0x35;
  _operationMap[currentOperation][IDX2_X]    = 0x3D;
  _operationMap[currentOperation][IDX2_Y]    = 0x39;
  _operationMap[currentOperation][IDX1_PRE]  = 0x21;
  _operationMap[currentOperation][IDX1_POST] = 0x31;

  currentOperation = "asl";
  _operationMap[currentOperation][ACC]    = 0x0A;
  _operationMap[currentOperation][ZER]    = 0x06;
  _operationMap[currentOperation][ABS]    = 0x0E;
  _operationMap[currentOperation][IDX1]   = 0x16;
  _operationMap[currentOperation][IDX2_X] = 0x1E;

  currentOperation = "bcc";
  _operationMap[currentOperation][REL] = 0x90;

  currentOperation = "bcs";
  _operationMap[currentOperation][REL] = 0xB0;

  currentOperation = "beq";
  _operationMap[currentOperation][REL] = 0xF0;

  currentOperation = "bit";
  _operationMap[currentOperation][ZER] = 0x24;
  _operationMap[currentOperation][ABS] = 0x2C;

  currentOperation = "bmi";
  _operationMap[currentOperation][REL] = 0x30;

  currentOperation = "bne";
  _operationMap[currentOperation][REL] = 0xD0;

  currentOperation = "bpl";
  _operationMap[currentOperation][REL] = 0x10;

  currentOperation = "bra";
  _operationMap[currentOperation][REL] = 0x80;

  currentOperation = "brk";
  _operationMap[currentOperation][IMP] = 0x00;

  currentOperation = "bvc";
  _operationMap[currentOperation][REL] = 0x50;

  currentOperation = "bvs";
  _operationMap[currentOperation][REL] = 0x70;

  currentOperation = "clc";
  _operationMap[currentOperation][IMP] = 0x18;

  currentOperation = "cld";
  _operationMap[currentOperation][IMP] = 0xD8;

  currentOperation = "cli";
  _operationMap[currentOperation][IMP] = 0x58;

  currentOperation = "clv";
  _operationMap[currentOperation][IMP] = 0xB8;

  currentOperation = "cmp";
  _operationMap[currentOperation][IMM]       = 0xC9;
  _operationMap[currentOperation][ZER]       = 0xC5;
  _operationMap[currentOperation][ABS]       = 0xCD;
  _operationMap[currentOperation][IDX1]      = 0xD5;
  _operationMap[currentOperation][IDX2_X]    = 0xDD;
  _operationMap[currentOperation][IDX2_Y]    = 0xD9;
  _operationMap[currentOperation][IDX1_PRE]  = 0xC1;
  _operationMap[currentOperation][IDX1_POST] = 0xD1;

  currentOperation = "cpx";
  _operationMap[currentOperation][IMM] = 0xE0;
  _operationMap[currentOperation][ZER] = 0xE4;
  _operationMap[currentOperation][ABS] = 0xEC;

  currentOperation = "cpy";
  _operationMap[currentOperation][IMM] = 0xC0;
  _operationMap[currentOperation][ZER] = 0xC4;
  _operationMap[currentOperation][ABS] = 0xCC;

  currentOperation = "dec";
  _operationMap[currentOperation][ZER]    = 0xC6;
  _operationMap[currentOperation][ABS]    = 0xCE;
  _operationMap[currentOperation][IDX1]   = 0xD6;
  _operationMap[currentOperation][IDX2_X] = 0xDE;

  currentOperation = "dex";
  _operationMap[currentOperation][IMP] = 0xCA;

  currentOperation = "dey";
  _operationMap[currentOperation][IMP] = 0x88;

  currentOperation = "eor";
  _operationMap[currentOperation][IMM]       = 0x49;
  _operationMap[currentOperation][ZER]       = 0x45;
  _operationMap[currentOperation][ABS]       = 0x4D;
  _operationMap[currentOperation][IDX1]      = 0x55;
  _operationMap[currentOperation][IDX2_X]    = 0x5D;
  _operationMap[currentOperation][IDX2_Y]    = 0x59;
  _operationMap[currentOperation][IDX1_PRE]  = 0x41;
  _operationMap[currentOperation][IDX1_POST] = 0x51;

  currentOperation = "inc";
  _operationMap[currentOperation][ZER]    = 0xE6;
  _operationMap[currentOperation][ABS]    = 0xEE;
  _operationMap[currentOperation][IDX1]   = 0xF6;
  _operationMap[currentOperation][IDX2_X] = 0xFE;

  currentOperation = "inx";
  _operationMap[currentOperation][IMP] = 0xE8;

  currentOperation = "iny";
  _operationMap[currentOperation][IMP] = 0xC8;

  currentOperation = "jmp";
  _operationMap[currentOperation][ABS] = 0x4C;
  _operationMap[currentOperation][IND] = 0x6C;

  currentOperation = "jsr";
  _operationMap[currentOperation][ABS] = 0x20;

  currentOperation = "lda";
  _operationMap[currentOperation][IMM]       = 0xA9;
  _operationMap[currentOperation][ZER]       = 0xA5;
  _operationMap[currentOperation][ABS]       = 0xAD;
  _operationMap[currentOperation][IDX1]      = 0xB5;
  _operationMap[currentOperation][IDX2_X]    = 0xBD;
  _operationMap[currentOperation][IDX2_Y]    = 0xB9;
  _operationMap[currentOperation][IDX1_PRE]  = 0xA1;
  _operationMap[currentOperation][IDX1_POST] = 0xB1;

  currentOperation = "ldx";
  _operationMap[currentOperation][IMM]    = 0xA2;
  _operationMap[currentOperation][ZER]    = 0xA6;
  _operationMap[currentOperation][ABS]    = 0xAE;
  _operationMap[currentOperation][IDX1]   = 0xB6;
  _operationMap[currentOperation][IDX2_Y] = 0xBE;

  currentOperation = "ldy";
  _operationMap[currentOperation][IMM]    = 0xA0;
  _operationMap[currentOperation][ZER]    = 0xA4;
  _operationMap[currentOperation][ABS]    = 0xAC;
  _operationMap[currentOperation][IDX1]   = 0xB4;
  _operationMap[currentOperation][IDX2_Y] = 0xBC;

  currentOperation = "lsr";
  _operationMap[currentOperation][ACC]    = 0x4A;
  _operationMap[currentOperation][ZER]    = 0x46;
  _operationMap[currentOperation][ABS]    = 0x4E;
  _operationMap[currentOperation][IDX1]   = 0x56;
  _operationMap[currentOperation][IDX2_X] = 0x5E;

  currentOperation = "nop";
  _operationMap[currentOperation][IMP] = 0xEA;

  currentOperation = "ora";
  _operationMap[currentOperation][IMM]       = 0x09;
  _operationMap[currentOperation][ZER]       = 0x05;
  _operationMap[currentOperation][ABS]       = 0x0D;
  _operationMap[currentOperation][IDX1]      = 0x15;
  _operationMap[currentOperation][IDX2_X]    = 0x1D;
  _operationMap[currentOperation][IDX2_Y]    = 0x19;
  _operationMap[currentOperation][IDX1_PRE]  = 0x01;
  _operationMap[currentOperation][IDX1_POST] = 0x11;

  currentOperation = "pha";
  _operationMap[currentOperation][IMP] = 0x48;

  currentOperation = "php";
  _operationMap[currentOperation][IMP] = 0x08;

  currentOperation = "phx";
  _operationMap[currentOperation][IMP] = 0xDA;

  currentOperation = "phy";
  _operationMap[currentOperation][IMP] = 0x5A;

  currentOperation = "pla";
  _operationMap[currentOperation][IMP] = 0x68;

  currentOperation = "plp";
  _operationMap[currentOperation][IMP] = 0x28;

  currentOperation = "plx";
  _operationMap[currentOperation][IMP] = 0xFA;

  currentOperation = "ply";
  _operationMap[currentOperation][IMP] = 0x7A;

  currentOperation = "rol";
  _operationMap[currentOperation][ACC]    = 0x2A;
  _operationMap[currentOperation][ZER]    = 0x26;
  _operationMap[currentOperation][ABS]    = 0x2E;
  _operationMap[currentOperation][IDX1]   = 0x36;
  _operationMap[currentOperation][IDX2_X] = 0x3E;

  currentOperation = "ror";
  _operationMap[currentOperation][ACC]    = 0x6A;
  _operationMap[currentOperation][ZER]    = 0x66;
  _operationMap[currentOperation][ABS]    = 0x6E;
  _operationMap[currentOperation][IDX1]   = 0x76;
  _operationMap[currentOperation][IDX2_X] = 0x7E;

  currentOperation = "rti";
  _operationMap[currentOperation][IMP] = 0x40;

  currentOperation = "rts";
  _operationMap[currentOperation][IMP] = 0x60;

  currentOperation = "sbc";
  _operationMap[currentOperation][IMM]       = 0xE9;
  _operationMap[currentOperation][ZER]       = 0xE5;
  _operationMap[currentOperation][ABS]       = 0xED;
  _operationMap[currentOperation][IDX1]      = 0xF5;
  _operationMap[currentOperation][IDX2_X]    = 0xFD;
  _operationMap[currentOperation][IDX2_Y]    = 0xF9;
  _operationMap[currentOperation][IDX1_PRE]  = 0xE1;
  _operationMap[currentOperation][IDX1_POST] = 0xF1;

  currentOperation = "sec";
  _operationMap[currentOperation][IMP] = 0x38;

  currentOperation = "sed";
  _operationMap[currentOperation][IMP] = 0xF8;

  currentOperation = "sei";
  _operationMap[currentOperation][IMP] = 0x78;

  currentOperation = "sta";
  _operationMap[currentOperation][ZER]       = 0x85;
  _operationMap[currentOperation][ABS]       = 0x8D;
  _operationMap[currentOperation][IDX1]      = 0x95;
  _operationMap[currentOperation][IDX2_X]    = 0x9D;
  _operationMap[currentOperation][IDX2_Y]    = 0x99;
  _operationMap[currentOperation][IDX1_PRE]  = 0x81;
  _operationMap[currentOperation][IDX1_POST] = 0x91;

  currentOperation = "stx";
  _operationMap[currentOperation][ZER]  = 0x86;
  _operationMap[currentOperation][ABS]  = 0x8E;
  _operationMap[currentOperation][IDX1] = 0x96;

  currentOperation = "sty";
  _operationMap[currentOperation][ZER]  = 0x84;
  _operationMap[currentOperation][ABS]  = 0x8C;
  _operationMap[currentOperation][IDX1] = 0x94;

  currentOperation = "stz";
  _operationMap[currentOperation][ZER]    = 0x64;
  _operationMap[currentOperation][ABS]    = 0x9C;
  _operationMap[currentOperation][IDX1]   = 0x74;
  _operationMap[currentOperation][IDX2_X] = 0x9E;

  currentOperation = "tax";
  _operationMap[currentOperation][IMP] = 0xAA;

  currentOperation = "tay";
  _operationMap[currentOperation][IMP] = 0xA8;

  currentOperation = "trb";
  _operationMap[currentOperation][ZER] = 0x14;
  _operationMap[currentOperation][ABS] = 0x1C;

  currentOperation = "tsb";
  _operationMap[currentOperation][ZER] = 0x04;
  _operationMap[currentOperation][ABS] = 0x0C;

  currentOperation = "tsx";
  _operationMap[currentOperation][IMP] = 0xBA;

  currentOperation = "txa";
  _operationMap[currentOperation][IMP] = 0x8A;

  currentOperation = "txs";
  _operationMap[currentOperation][IMP] = 0x9A;

  currentOperation = "tya";
  _operationMap[currentOperation][IMP] = 0x98;
}

Parser::Line Parser::parseLine(string line)
{
  Line parsedLine;
  ParserErrorException syntaxError;
  string buf;
  stringstream ss(line);

  parsedLine.label = "";
  parsedLine.operation = "";
  parsedLine.operand = "";
  parsedLine.comment = "";
  parsedLine.opcode = 0xFFFF;

  if (!line.compare("")) {
    return parsedLine;
  }

  vector<string> tokens;
  while (ss >> buf) {
    tokens.push_back(buf);
  }

  // If starts with whitespace, there is no label
  if (line.at(0) == ' ' || line.at(0) == '\t') {
    parsedLine.label = "";
    bool isComment = false;
    bool isString = false;
    bool isList = false;
    char openString = '\0';
    for (int i = 0; i < tokens.size(); i++) {
      if (isComment) {
        parsedLine.comment += " " + tokens.at(i);
      } else if (isString) {
        parsedLine.operand += " " + tokens.at(i);
        if (tokens.at(i).at(0) == openString ||
            tokens.at(i).at(tokens.at(i).size() - 1) == openString) {
          openString = '\0';
          isString = false;
          continue;
        }
      } else if (isList) {
        parsedLine.operand += " " + tokens.at(i);
        if (tokens.at(i).at(tokens.at(i).size() - 1) != ',') {
          isList = false;
          continue;
        }
      } else {
        if (tokens.at(i).at(0) == ';') {
            parsedLine.comment = tokens.at(i);
            isComment = true;
            continue;
        }
        switch (i) {
        case 0:
          parsedLine.operation = tokens.at(i);
          break;
        case 1:
          parsedLine.operand = tokens.at(i);
          if (tokens.at(i).at(0) == '\'' &&
              tokens.at(i).at(tokens.at(i).size() - 1) != '\'') {
            openString = '\'';
            isString = true;
          } else if (tokens.at(i).at(0) == '\"' &&
                     tokens.at(i).at(tokens.at(i).size() - 1) != '\"') {
            openString = '\"';
            isString = true;
          } else if (tokens.at(i).at(tokens.at(i).size() - 1) == ',') {
            isList = true;
          }
          break;
        default:
          cout << "No label " << i << endl;
          throw syntaxError;
        }
      }
    }
  } else {
    bool isComment = false;
    bool isString = false;
    bool isList = false;
    char openString = '\0';
    for (int i = 0; i < tokens.size(); i++) {
      if (isComment) {
        parsedLine.comment += " " + tokens.at(i);
      } else if (isString) {
        parsedLine.operand += " " + tokens.at(i);
        if (tokens.at(i).at(0) == openString ||
            tokens.at(i).at(tokens.at(i).size() - 1) == openString) {
          openString = '\0';
          isString = false;
          continue;
        }
      } else if (isList) {
        parsedLine.operand += " " + tokens.at(i);
        if (tokens.at(i).at(tokens.at(i).size() - 1) != ',') {
          isList = false;
          continue;
        }
      } else {
        if (tokens.at(i).at(0) == ';') {
          parsedLine.comment = tokens.at(i);
          isComment = true;
          continue;
        }
        switch (i) {
        case 0:
          parsedLine.label = tokens.at(i);
          break;
        case 1:
          parsedLine.operation = tokens.at(i);
          break;
        case 2:
          parsedLine.operand = tokens.at(i);
          if (tokens.at(i).at(0) == '\'' &&
              tokens.at(i).at(tokens.at(i).size() - 1) != '\'') {
            openString = '\'';
            isString = true;
          } else if (tokens.at(i).at(0) == '\"' &&
                     tokens.at(i).at(tokens.at(i).size() - 1) != '\"') {
            openString = '\"';
            isString = true;
          } else if (tokens.at(i).at(tokens.at(i).size() - 1) == ',') {
            isList = true;
          }
          break;
        default:
          cout << i << endl;
          throw syntaxError;
        }
      }
    }
  }
  if (parsedLine.operand == "" && parsedLine.operation != "") {
    string tempString = parsedLine.operation;
    for (int i = 0; i < 3; i++) {
      tempString[i] = tolower(tempString[i]);
    }
    parsedLine.opcode = _operationMap[tempString][IMP];
  }
  return parsedLine;
}
