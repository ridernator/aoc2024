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
  const auto& lines = readFileToVector();

  std::int64_t num1;
  std::int64_t num2;
  std::int64_t sum = 0;

  for (const auto& line : lines) {
    std::regex word_regex("mul\\([0-9]+,[0-9]+\\)");

    for (auto iterator = std::sregex_iterator(line.begin(), line.end(), word_regex); iterator != std::sregex_iterator(); ++iterator) {
      sscanf(iterator->str().c_str(), "mul(%li,%li)", &num1, &num2);

      sum += num1 * num2;
    }
  }

  std::cout << "Sum of multiplications = " << sum << std::endl;
}
