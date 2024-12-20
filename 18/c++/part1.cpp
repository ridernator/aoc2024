#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
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

class Tile {
 public:
  bool isWall = false;
  std::int64_t distance = std::numeric_limits<std::int64_t>::max();
};

void print(Tile*** tiles,
           const std::int64_t maxX,
           const std::int64_t maxY) {
  for (std::int64_t y = 0; y < maxY; ++y) {
    for (std::int64_t x = 0; x < maxX; ++x) {
      if (tiles[y][x]->isWall) {
        std::cout << '#';
      } else if (tiles[y][x]->distance != std::numeric_limits<std::int64_t>::max()) {
        std::cout << std::to_string(tiles[y][x]->distance);
      } else {
        std::cout << '.';
      }
    }

    std::cout << std::endl;
  }

  std::cout << std::endl;
}

int main() {
  const auto& lines = readFileToVector();

  std::int64_t maxSize = 71;

  Tile*** tiles = new Tile**[maxSize];

  for (std::int64_t y = 0; y < maxSize; ++y) {
    tiles[y] = new Tile*[maxSize];

    for (std::int64_t x = 0; x < maxSize; ++x) {
      tiles[y][x] = new Tile();
    }
  }

  std::int64_t x;
  std::int64_t y;

  for (std::int64_t index = 0; index < 1024; ++index) {
    sscanf(lines[index].c_str(), "%li,%li", &x, &y);

    tiles[y][x]->isWall = true;
  }

  tiles[0][0]->distance = 0;

  // print(tiles, maxSize, maxSize);

  std::int64_t distance = 0;

  while (tiles[maxSize - 1][maxSize - 1]->distance == std::numeric_limits<std::int64_t>::max()) {
    for (std::int64_t y = 0; y < maxSize; ++y) {
      for (std::int64_t x = 0; x < maxSize; ++x) {
        if ((!tiles[y][x]->isWall) && (tiles[y][x]->distance == distance)) {
          for (std::int64_t step = -1; step <= 1; step += 2) {
            if ((x + step >= 0) && (x + step < maxSize)) {
              if ((!tiles[y][x + step]->isWall) && (tiles[y][x + step]->distance > distance + 1)) {
                tiles[y][x + step]->distance = distance + 1;
              }
            }

            if ((y + step >= 0) && (y + step < maxSize)) {
              if ((!tiles[y + step][x]->isWall) && (tiles[y + step][x]->distance > distance + 1)) {
                tiles[y + step][x]->distance = distance + 1;
              }
            }
          }
        }
      }
    }

    ++distance;
  }

  std::cout << "Shortest distance is " << tiles[maxSize - 1][maxSize - 1]->distance << std::endl;

  for (std::int64_t y = 0; y < maxSize; ++y) {
    for (std::int64_t x = 0; x < maxSize; ++x) {
      delete tiles[y][x];
    }

    delete [] tiles[y];
  }

  delete [] tiles;
}
