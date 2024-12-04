#include <algorithm>
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

struct MAS {
  std::int64_t row;
  std::int64_t col;
};

int main() {
  const std::string word = "MAS";

  const auto& data = readFileToVector();

  const std::int64_t rowLength = data[0].size();
  const std::int64_t colLength = data.size();

  bool found;
  std::uint64_t foundCount = 0;

  std::vector<MAS> masses;

  for (std::int64_t row = 0; row < colLength; ++row) {
    for (std::int64_t col = 0; col < rowLength; ++col) {
      if (data[row][col] == word[0]) {
        for (std::int8_t rowChange = -1; rowChange <= 1; rowChange += 2) {
          for (std::int8_t colChange = -1; colChange <= 1; colChange += 2) {
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
              if (std::find_if(masses.begin(),
                            masses.end(),
                            [&] (const auto& mas) {
                              return ((mas.row == row + rowChange) && (mas.col == col + colChange));
                            }) == masses.end()) {
                masses.emplace_back(row + rowChange, col + colChange);
              } else {
                ++foundCount;
              }
            }
          }
        }
      }
    }
  }

  std::cout << "Number of times X-MAS appears = " << foundCount << std::endl;
}
