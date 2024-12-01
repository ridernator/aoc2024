#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
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
  std::vector<std::int64_t> first;
  std::vector<std::int64_t> second;

  std::int64_t a;
  std::int64_t b;

  for (const auto& line : lines) {
    sscanf(line.c_str(), "%li   %li", &a, &b);
    first.push_back(a);
    second.push_back(b);
  }

  std::sort(first.begin(), first.end());
  std::sort(second.begin(), second.end());

  std::int64_t totalDistance = 0;

  for (std::size_t index = 0; index < first.size(); ++index) {
    totalDistance += labs(first[index] - second[index]);
  }

  std::cout << "Total distance = " << totalDistance << std::endl;
}
