#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
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

struct Position {
  std::int32_t x;
  std::int32_t y;

  friend bool operator<(const Position& left, const Position& right) {
    return (((std::int64_t) left.x << 32) + left.y) < (((std::int64_t) right.x << 32) + right.y);
  }

  friend bool operator==(const Position& left, const Position& right) {
    return (((std::int64_t) left.x << 32) + left.y) == (((std::int64_t) right.x << 32) + right.y);
  }

  friend bool operator!=(const Position& left, const Position& right) {
    return (((std::int64_t) left.x << 32) + left.y) != (((std::int64_t) right.x << 32) + right.y);
  }
};

int main() {
  const auto& lines = readFileToVector();

  std::map<char, std::vector<Position>> antennas;
  std::int32_t maxX = lines[0].size();
  std::int32_t maxY = lines.size();
  std::set<Position> antinodes;

  for (std::int32_t y = 0; y < (std::int32_t) lines.size(); ++y) {
    for (std::int32_t x = 0; x < (std::int32_t) lines[0].size(); ++x) {
      if (lines[y][x] != '.') {
        antennas[lines[y][x]].emplace_back(x, y);
      }
    }
  }

  for (const auto& [symbol, positions] : antennas) {
    for (const auto& position1 : positions) {
      for (const auto& position2 : positions) {
        if (position1 != position2) {
          Position antinode;
          antinode.x = position1.x - (2 * (position1.x - position2.x));
          antinode.y = position1.y - (2 * (position1.y - position2.y));

          if ((antinode.x >= 0) && (antinode.x < maxX) &&
              (antinode.y >= 0) && (antinode.y < maxY)) {
            antinodes.insert(antinode);
          }
        }
      }
    }
  }

  std::cout << "Antinode locations = " << antinodes.size() << std::endl;
}
