#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
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
  std::uint64_t aX;
  std::uint64_t aY;
  std::uint64_t bX;
  std::uint64_t bY;
  std::uint64_t pX;
  std::uint64_t pY;
  std::uint64_t aCost = 3;
  std::uint64_t bCost = 1;
  std::uint64_t maxPresses = 100;
  bool found;
  std::uint64_t sum = 0;

  for (std::size_t index = 0; index < lines.size(); index += 4) {
    sscanf(lines[index].c_str(), "Button A: X+%lu, Y+%lu", &aX, &aY);
    sscanf(lines[index + 1].c_str(), "Button B: X+%lu, Y+%lu", &bX, &bY);
    sscanf(lines[index + 2].c_str(), "Prize: X=%lu, Y=%lu", &pX, &pY);

    found = false;

    for (std::uint64_t aCount = 0; aCount < maxPresses; ++aCount) {
      if ((found) || (aCount * aX > pX) || (aCount * aY > pY)) {
        break;
      }

      for (std::uint64_t bCount = 0; bCount < maxPresses; ++bCount) {
        if ((aCount * aX + bCount * bX > pX) || (aCount * aY + bCount * bY > pY)) {
          break;
        }

        if ((aCount * aX + bCount * bX == pX) && (aCount * aY + bCount * bY == pY)) {
          sum += aCount * aCost + bCount * bCost;
        }
      }
    }
  }

  std::cout << "Fewest tokens needed = " << sum << std::endl;
}
