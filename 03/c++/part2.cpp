#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <regex>
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
  const std::regex doRegex("do\\(\\)");
  const std::regex dontRegex("don't\\(\\)");
  const std::regex mulRegex("mul\\([0-9]+,[0-9]+\\)");

  const auto& line = readFile().str();

  std::int64_t num1;
  std::int64_t num2;
  std::int64_t sum = 0;
  std::size_t charIndex = 0;
  std::size_t increment;

  while (true) {
    increment = std::sregex_iterator(line.begin() + charIndex, line.end(), dontRegex)->position();

    charIndex += increment;

    for (auto iterator = std::sregex_iterator(line.begin() + charIndex - increment,
          (increment == 0) ? line.end() : line.begin() + charIndex + 1,
          mulRegex);
        iterator != std::sregex_iterator();
        ++iterator) {
      sscanf(iterator->str().c_str(), "mul(%li,%li)", &num1, &num2);
      sum += num1 * num2;
    }

    if (increment == 0) {
      break;
    }

    increment = std::sregex_iterator(line.begin() + charIndex, line.end(), doRegex)->position();

    if (increment == 0) {
      break;
    }

    charIndex += increment;
  }

  std::cout << "Sum of enabled multiplications = " << sum << std::endl;
}
