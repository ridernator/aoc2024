#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
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

struct Rule {
  std::uint64_t before;
  std::uint64_t after;
};

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

enum class Operation : std::uint8_t {
  PLUS,
  TIMES,
  CONCAT
};

bool increment(std::vector<Operation>& operations) {
  std::size_t index = 0;

  while (index != operations.size()) {
    switch (operations[index]) {
      case Operation::PLUS: {
        operations[index] = Operation::TIMES;

        break;
      }

      case Operation::TIMES: {
        operations[index] = Operation::CONCAT;

        break;
      }

      case Operation::CONCAT: {
        operations[index] = Operation::PLUS;

        break;
      }
    }

    if (operations[index] == Operation::PLUS) {
      ++index;
    } else {
      break;
    }
  }

  return (index != operations.size());
}

int main() {
  const auto& lines = readFileToVector();

  std::string tempString;
  std::int64_t total;
  std::vector<std::int64_t> values;
  std::vector<Operation> operations;
  std::int64_t calcTotal = 0;
  std::int64_t sum = 0;

  for (const auto& line : lines) {
    std::stringstream ss(line);

    std::getline(ss, tempString, ':');
    total = std::strtol(tempString.c_str(), nullptr, 10);

    values.clear();
    while (std::getline(ss, tempString, ' ')) {
      values.emplace_back(std::strtol(tempString.c_str(), nullptr, 10));
    }

    operations.clear();
    for (std::size_t index = 0; index < values.size(); ++index) {
      operations.emplace_back(Operation::PLUS);
    }

    do {
      calcTotal = values[0];

      for (std::size_t index = 1; index < values.size(); ++index) {
        switch (operations[index]) {
          case Operation::PLUS: {
            calcTotal += values[index];

            break;
          }

          case Operation::TIMES: {
            calcTotal *= values[index];

            break;
          }

          case Operation::CONCAT: {
            calcTotal = std::strtol((std::to_string(calcTotal) + std::to_string(values[index])).c_str(), nullptr, 10);

            break;
          }
        }
      }

      if (calcTotal == total) {
        sum += total;

        break;
      }
    } while (increment(operations));
  }

  std::cout << "Total calibration result = " << sum << std::endl;
}
