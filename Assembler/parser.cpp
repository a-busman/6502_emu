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

  parsedLine.label = "";
  parsedLine.operation = "";
  parsedLine.operand = "";
  parsedLine.comment = "";

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
  return parsedLine;
}
