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

  std::size_t i = 0;

  std::vector<std::int64_t> program;
  std::vector<std::int64_t> output;

  sscanf(lines[0].c_str(), "Register A: %li", &a);
  sscanf(lines[1].c_str(), "Register B: %li", &b);
  sscanf(lines[2].c_str(), "Register C: %li", &c);

  std::stringstream ss(lines[4].substr(9));
  std::string tempString;

  while (std::getline(ss, tempString, ',')) {
    program.push_back(std::strtoll(tempString.c_str(), nullptr, 10));
  }

  while (i != program.size()) {
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

  std::cout << "Output: ";
  for (std::size_t index = 0; index < output.size() - 1; ++index) {
    std::cout << output[index] << ',';
  }
  std::cout << output[output.size() - 1] << std::endl;
}
