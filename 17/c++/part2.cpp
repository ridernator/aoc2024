#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>

#define INPUT "../data/input"

/**
 * Read a file entirely into a stringstream
 *
 * @param The name of the file to read
 * @return The contents of the file
 **/
std::stringstream readFile(const std::string& filename = INPUT) {
  std::ifstream fileStream(filename);
  std::stringstream returnVal;

  returnVal << fileStream.rdbuf();

  return returnVal;
}

/**
 * Read a file entirely into a vector of strings
 *
 * @param The name of the file to read
 * @return The vector of lines in the file
 **/
std::vector<std::string> readFileToVector(const std::string& filename = INPUT) {
  std::ifstream fileStream(filename);
  std::string string;
  std::vector<std::string> returnVal;

  while (std::getline(fileStream, string)) {
    returnVal.push_back(string);
  }

  return returnVal;
}

// Program: 2,4,1,1,7,5,1,5,4,3,0,3,5,5,3,0
// B = A % 8
// B = B xor 1 // flip last bit
// C = A / 2^B
// B = B xor 5
// B = B xor C
// A = A / 8
// OUT B
// restart if A != 0
//
// A, B, C after 4th step
// A=?, B=(A%8) xor 1, C=A/(2^((A%8) xor 1))
//
//
// length of program is 16, so try numbers 8^15 -> 8^16
// The algorithm below works, changing the powers of 8 is different numbers match
// Not good though as I'm not sure why it works and it gets a working value but
// not the right value

int main() {
  const auto& lines = readFileToVector();

  std::int64_t operands[8];
  operands[0] = 0;
  operands[1] = 1;
  operands[2] = 2;
  operands[3] = 3;

  std::int64_t& a = operands[4];
  std::int64_t& b = operands[5];
  std::int64_t& c = operands[6];
  std::int64_t bVal;
  std::int64_t cVal;

  std::size_t i;

  std::vector<std::int64_t> program;
  std::vector<std::int64_t> output = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  sscanf(lines[1].c_str(), "Register B: %li", &bVal);
  sscanf(lines[2].c_str(), "Register C: %li", &cVal);

  std::string programString = lines[4].substr(9);
  std::string outputString;

  std::stringstream ss(programString);
  std::string tempString;

  while (std::getline(ss, tempString, ',')) {
    program.push_back(std::strtoll(tempString.c_str(), nullptr, 10));
  }

  std::int64_t testVal = std::pow(8, 1);
  while (programString != outputString) {
    i = 0;
    for (int f = 15; f > 0; --f) {
      if (output[f] != program[f]) {
        testVal += std::pow(8, f - 5);
      }
    }
    a = ++testVal;
    b = bVal;
    c = cVal;
    output.clear();

    while ((i != program.size()) && (output.size() <= program.size())) {
      // std::cout << "A=" << a << ", B=" << b << ", C=" << c << ", i=" << i << "(" << program[i] << ", " << program[i + 1] << ")" << std::endl;

      switch (program[i]) {
        // adv
        case 0: {
          a /= std::pow(2, operands[program[i + 1]]);

          break;
        }

        // bxl
        case 1: {
          b ^= program[i + 1];

          break;
        }

        // bst
        case 2: {
          b = operands[program[i + 1]] % 8;

          break;
        }

        // jnz
        case 3: {
          if (a != 0) {
            i = program[i + 1];
            i -= 2;
          }

          break;
        }

        // bxc
        case 4: {
          b ^= c;

          break;
        }

        // out
        case 5: {
          output.push_back(operands[program[i + 1]] % 8);

          break;
        }

        // bdv
        case 6: {
          b = a / std::pow(2, operands[program[i + 1]]);

          break;
        }

        // cdv
        case 7: {
          c = a / std::pow(2, operands[program[i + 1]]);

          break;
        }
      }

      i += 2;
    }

    std::stringstream iss;
    for (std::size_t index = 0; index < output.size() - 1; ++index) {
      iss << output[index] << ',';
    }
    iss << output[output.size() - 1];
    outputString = iss.str();

    // if (testVal % 1000000 == 0) {
      std::cout << testVal << " = " << outputString << std::endl;
    // }
  }

  std::cout << "Good when a is set to " << testVal << std::endl;
}
