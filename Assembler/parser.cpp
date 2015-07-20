#include <vector>
#include <sstream>

#include <iostream>

#include "parser.h"

Line parseLine(string line)
{
  Line parsedLine;
  ParserErrorException syntaxError;
  string buf;
  stringstream ss(line);

  if (!line.compare("")) {
    parsedLine.label = "";
    parsedLine.operation = "";
    parsedLine.operand = "";
    parsedLine.comment = "";
    return parsedLine;
  }

  vector<string> tokens;
  while (ss >> buf) {
    tokens.push_back(buf);
  }
  // If starts with whitespace, there is no label
  if (line.at(0) == ' ' || line.at(0) == '\t') {
    parsedLine.label = "";
    bool comment = false;
    for (int i = 0; i < tokens.size(); i++) {
      if (comment) {
        parsedLine.comment += " " + tokens.at(i);
      } else {
        switch (i) {
        case 0:
          if (tokens.at(i).at(0) == ';') {
            parsedLine.operation = "";
            parsedLine.operand = "";
            parsedLine.comment = tokens.at(i);
            comment = true;
          } else {
            parsedLine.operation = tokens.at(i);
          }
          break;
        case 1:
          if (tokens.at(i).at(0) == ';') {
            parsedLine.operand = "";
            parsedLine.comment = tokens.at(i);
            comment = true;
          } else {
            parsedLine.operand = tokens.at(i);
          }
          break;
        case 2:
          if (tokens.at(i).at(0) == ';') {
            parsedLine.comment = tokens.at(i);
            comment = true;
          } else {
            throw syntaxError;
          }
          break;
        default:
          throw syntaxError;
        }
      }
    }
  } else {
    bool comment = false;
    for (int i = 0; i < tokens.size(); i++) {
      if (comment) {
        parsedLine.comment += " " + tokens.at(i);
      } else {
        switch (i) {
        case 0:
          if (tokens.at(i).at(0) == ';') {
            parsedLine.label = "";
            parsedLine.operation = "";
            parsedLine.operand = "";
            parsedLine.comment = tokens.at(i);
            comment = true;
          } else {
            parsedLine.label = tokens.at(i);
          }
          break;
        case 1:
          if (tokens.at(i).at(0) == ';') {
            parsedLine.operation = "";
            parsedLine.operand = "";
            parsedLine.comment = tokens.at(i);
            comment = true;
          } else {
            parsedLine.operation = tokens.at(i);
          }
          break;
        case 2:
          if (tokens.at(i).at(0) == ';') {
            parsedLine.operand = "";
            parsedLine.comment = tokens.at(i);
            comment = true;
          } else {
            parsedLine.operand = tokens.at(i);
          }
          break;
        case 3:
          if (tokens.at(i).at(0) == ';') {
            parsedLine.comment = tokens.at(i);
            comment = true;
          } else {
            cout << "Final token not comment" << endl;
            throw syntaxError;
          }
          break;
        default:
          throw syntaxError;
        }
      }
    }
  }
  return parsedLine;
}
