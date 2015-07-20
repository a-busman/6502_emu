#include <iostream>
#include <fstream>
#include <sstream>

#include "parser.h"

int main(int argc, char *argv[])
{
  if (argc == 2) {
    std::ifstream source(argv[1]);
    std::string line;
    while(std::getline(source, line)) {
      Line myLine;
      try {
        myLine = parseLine(line);
      } catch (exception& e) {
        std::cout << e.what() << std::endl;
        return -1;
      }
      std::cout << line << std::endl << std::endl;
      std::cout << "label:     " << myLine.label << std::endl;
      std::cout << "operation: " << myLine.operation << std::endl;
      std::cout << "operand:   " << myLine.operand << std::endl;
      std::cout << "comment:   " << myLine.comment << std::endl << std::endl;
    }
  } else {
    return -1;
  }
  return 0;
}
