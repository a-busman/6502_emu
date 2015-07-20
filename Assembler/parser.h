#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <exception>

using namespace std;
struct Line {
  string label;
  string operation;
  string operand;
  string comment;
};

class ParserErrorException : public exception
{
  virtual const char* what() const throw()
  {
    return "Syntax error";
  }
};

Line parseLine(string line);

#endif // PARSER_H
