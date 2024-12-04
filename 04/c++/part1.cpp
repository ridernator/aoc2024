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
  const std::string word = "XMAS";

  const auto& data = readFileToVector();

  const std::int64_t rowLength = data[0].size();
  const std::int64_t colLength = data.size();

  bool found;
  std::uint64_t foundCount = 0;

  for (std::int64_t row = 0; row < colLength; ++row) {
    for (std::int64_t col = 0; col < rowLength; ++col) {
      if (data[row][col] == word[0]) {
        for (std::int8_t rowChange = -1; rowChange <= 1; ++rowChange) {
          for (std::int8_t colChange = -1; colChange <= 1; ++colChange) {
            if ((rowChange == 0) && (colChange == 0)) {
              continue;
            }

            found = true;

            for (std::int64_t index = 1; index < (std::int64_t) word.size(); ++index) {
              if ((row + (index * rowChange) < 0) ||
                  (row + (index * rowChange) >= rowLength) ||
                  (col + (index * colChange) < 0) ||
                  (col + (index * colChange) >= colLength)) {
                found = false;

                break;
              }

              if (data[row + (index * rowChange)][col + (index * colChange)] != word[index]) {
                found = false;

                break;
              }
            }

            if (found) {
              ++foundCount;
            }
          }
        }
      }
    }
  }

  std::cout << "Number of times " << word << " appears = " << foundCount << std::endl;
}
