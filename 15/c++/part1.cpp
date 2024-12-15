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

class Tile {
 public:
  Tile(const bool isWall,
       const bool hasBox) : isWall(isWall),
                            hasBox(hasBox) {}

  bool isWall;
  bool hasBox;
};

bool move(Tile*** tiles,
          std::int64_t x,
          std::int64_t y,
          std::int64_t xStep,
          std::int64_t yStep) {
  bool returnVal = true;

  if (tiles[y + yStep][x + xStep]->isWall) {
    returnVal = false;
  } else if (tiles[y + yStep][x + xStep]->hasBox) {
    returnVal = move(tiles, x + xStep, y + yStep, xStep, yStep);
  }

  if (returnVal) {
    tiles[y + yStep][x + xStep]->hasBox = tiles[y][x]->hasBox;
    tiles[y][x]->hasBox = false;
  }

  return returnVal;
}

void print(Tile*** tiles,
           const std::int64_t robotX,
           const std::int64_t robotY,
           const std::int64_t dimension) {
  for (std::int64_t y = 0; y < dimension; ++y) {
    for (std::int64_t x = 0; x < dimension; ++x) {
      if (tiles[y][x]->hasBox) {
        std::cout << 'O';
      } else if (tiles[y][x]->isWall) {
        std::cout << '#';
      } else if ((x == robotX) && (y == robotY)) {
        std::cout << '@';
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

  Tile*** tiles = new Tile**[lines[0].size()];
  std::vector<std::string> moveLists;
  std::int64_t robotX = 1;
  std::int64_t robotY = 1;

  for (std::int64_t y = 0; y < (std::int64_t) lines[0].size(); ++y) {
    tiles[y] = new Tile*[lines[0].size()];

    for (std::int64_t x = 0; x < (std::int64_t) lines[0].size(); ++x) {
      tiles[y][x] = new Tile((lines[y][x] == '#'), (lines[y][x] == 'O'));

      if (lines[y][x] == '@') {
        robotX = x;
        robotY = y;
      }
    }
  }

  for (std::int64_t index = lines[0].size() + 1; index < (std::int64_t) lines.size(); ++index) {
    moveLists.push_back(lines[index]);
  }

  // print(tiles, robotX, robotY, lines[0].size());

  for (const auto& moveList : moveLists) {
    for (const auto& movement : moveList) {
      switch (movement) {
        case '>': {
          if (move(tiles, robotX, robotY, 1, 0)) {
            ++robotX;
          }

          break;
        }

        case '<': {
          if (move(tiles, robotX, robotY, -1, 0)) {
            --robotX;
          }

          break;
        }

        case '^': {
          if (move(tiles, robotX, robotY, 0, -1)) {
            --robotY;
          }

          break;
        }

        case 'v': {
          if (move(tiles, robotX, robotY, 0, 1)) {
            ++robotY;
          }

          break;
        }
      }

      // print(tiles, robotX, robotY, lines[0].size());
    }
  }

  std::int64_t sum = 0;

  for (std::int64_t y = 0; y < (std::int64_t) lines[0].size(); ++y) {
    for (std::int64_t x = 0; x < (std::int64_t) lines[0].size(); ++x) {
      if (tiles[y][x]->hasBox) {
        sum += (100 * y) + x;
      }
    }
  }

  std::cout << "Sum of GPS co-ordinates = " << sum << std::endl;

  for (std::int64_t y = 0; y < (std::int64_t) lines[0].size(); ++y) {
    for (std::int64_t x = 0; x < (std::int64_t) lines[0].size(); ++x) {
      delete tiles[y][x];
    }

    delete [] tiles[y];
  }

  delete [] tiles;
}
