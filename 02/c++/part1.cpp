#include <algorithm>
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

  std::uint64_t safeCount = 0;
  std::string tempString;

  for (const auto& line : lines) {
    std::vector<std::int64_t> data;
    std::stringstream ss(line);

    while (std::getline(ss, tempString, ' ')) {
      data.push_back(std::strtoll(tempString.c_str(), nullptr, 10));
    }

    bool ok = true;

    if (std::is_sorted(data.begin(), data.end())) {
      for (std::size_t index = 1; index < data.size(); ++index) {
        if ((data[index] == data[index - 1]) ||
            (data[index] - data[index - 1] > 3)) {
          ok = false;

          break;
        }
      }

      if (ok) {
        ++safeCount;
      }
    } else if (std::is_sorted(data.rbegin(), data.rend())) {
      for (std::size_t index = 1; index < data.size(); ++index) {
        if ((data[index] == data[index - 1]) ||
            (data[index - 1] - data[index] > 3)) {
          ok = false;

          break;
        }
      }

      if (ok) {
        ++safeCount;
      }
    }
  }

  std::cout << "Number of safe levels = " << safeCount << std::endl;
}
