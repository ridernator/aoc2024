#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#define INPUT "../data/input"

const std::uint64_t EMPTY = std::numeric_limits<std::uint64_t>::max();

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

static const std::array<std::uint64_t, 10> POWS = {(std::uint64_t) std::pow(10, 0),
                                                   (std::uint64_t) std::pow(10, 1),
                                                   (std::uint64_t) std::pow(10, 2),
                                                   (std::uint64_t) std::pow(10, 3),
                                                   (std::uint64_t) std::pow(10, 4),
                                                   (std::uint64_t) std::pow(10, 5),
                                                   (std::uint64_t) std::pow(10, 6),
                                                   (std::uint64_t) std::pow(10, 7),
                                                   (std::uint64_t) std::pow(10, 8),
                                                   (std::uint64_t) std::pow(10, 9)};

std::uint64_t calculateNumStones(const std::uint64_t number,
                                 const std::uint64_t depth,
                                 const std::uint64_t maxDepth) {
  if (depth == maxDepth) {
    return 1;
  }

  if (number == 0) {
    return calculateNumStones(1, depth + 1, maxDepth);
  }

  std::uint64_t numDigits = (std::uint64_t) log10(number) + 1;

  if (numDigits % 2 == 0) {
    std::uint64_t splitter = POWS[numDigits / 2];

    return calculateNumStones(number % splitter, depth + 1, maxDepth) + calculateNumStones(number / splitter, depth + 1, maxDepth);
  } else {
    return calculateNumStones(number * 2024, depth + 1, maxDepth);
  }
}

int main() {
  auto data = readFile();
  std::string tempString;
  std::uint64_t maxBlinks = 75;
  std::uint64_t numStones = 0;
  std::uint64_t firstStep = 37;
  std::vector<std::uint64_t> stones;
  std::uint64_t numDigits;
  std::uint64_t splitter;
  std::uint64_t maxIndex;

  while (std::getline(data, tempString, ' ')) {
    stones.push_back(std::strtoul(tempString.c_str(), nullptr, 10));
  }

  for (std::uint64_t blink = 0; blink < firstStep; ++blink) {
    maxIndex = stones.size();

    for (std::size_t index = 0; index < maxIndex; ++index) {
      if (stones[index] == 0) {
        stones[index] = 1;
      } else {
        numDigits = (std::uint64_t) log10(stones[index]) + 1;

        if (numDigits % 2 == 0) {
          splitter = POWS[numDigits / 2];

          stones.emplace_back(stones[index] / splitter);
          stones[index] %= splitter;
        } else {
          stones[index] *= 2024;
        }
      }
    }
  }

  std::map<std::uint64_t, std::uint64_t> lookup;
  for (std::uint64_t index = 0; index < stones.size(); ++index) {
    if (lookup.find(stones[index]) == lookup.end()) {
      lookup.emplace(stones[index], calculateNumStones(stones[index], firstStep, maxBlinks));
    }

    numStones += lookup[stones[index]];

    if (index % 10000 == 0) {
      std::cout << index << " of " << stones.size() << std::endl;
    }
  }

  std::cout << "Number of stones after " << maxBlinks << " blinks = " << numStones << std::endl;
}
