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
       const bool hasBox,
       const bool left) : isWall(isWall),
                          hasBox(hasBox),
                          left(left) {}

  bool isWall;
  bool hasBox;
  bool left;
};

bool move(Tile*** tiles,
          std::int64_t x,
          std::int64_t y,
          std::int64_t xStep,
          std::int64_t yStep,
          bool test) {
  bool returnVal = true;

  if (tiles[y + yStep][x + xStep]->isWall) {
    returnVal = false;
  } else if (tiles[y + yStep][x + xStep]->hasBox) {
    if (xStep == 0) {
      if (tiles[y + yStep][x]->left) {
        returnVal = move(tiles, x, y + yStep, 0, yStep, test) &&
                    move(tiles, x + 1, y + yStep, 0, yStep, test);
      } else {
        returnVal = move(tiles, x, y + yStep, 0, yStep, test) &&
                    move(tiles, x - 1, y + yStep, 0, yStep, test);
      }
    } else {
      returnVal = move(tiles, x + xStep, y + yStep, xStep, yStep, test);
    }
  }

  if ((returnVal) && (!test)) {
    tiles[y + yStep][x + xStep]->hasBox = tiles[y][x]->hasBox;
    tiles[y + yStep][x + xStep]->left = tiles[y][x]->left;
    tiles[y][x]->hasBox = false;
  }

  return returnVal;
}

void print(Tile*** tiles,
           const std::int64_t robotX,
           const std::int64_t robotY,
           const std::int64_t dimension) {
  std::cout << "Robot at " << robotX << ", " << robotY << std::endl;
  for (std::int64_t y = 0; y < dimension; ++y) {
    for (std::int64_t x = 0; x < dimension * 2; ++x) {
      if (tiles[y][x]->hasBox) {
        if (tiles[y][x]->left) {
          std::cout << '[';
        } else {
          std::cout << ']';
        }
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
    tiles[y] = new Tile*[lines[0].size() * 2];

    for (std::int64_t x = 0; x < (std::int64_t) lines[0].size(); ++x) {
      tiles[y][x * 2] = new Tile((lines[y][x] == '#'), (lines[y][x] == 'O'), true);
      tiles[y][(x * 2) + 1] = new Tile((lines[y][x] == '#'), (lines[y][x] == 'O'), false);

      if (lines[y][x] == '@') {
        robotX = x * 2;
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
          if (move(tiles, robotX, robotY, 1, 0, true)) {
            move(tiles, robotX, robotY, 1, 0, false);

            ++robotX;
          }

          break;
        }

        case '<': {
          if (move(tiles, robotX, robotY, -1, 0, true)) {
            move(tiles, robotX, robotY, -1, 0, false);

            --robotX;
          }

          break;
        }

        case '^': {
          if (move(tiles, robotX, robotY, 0, -1, true)) {
            move(tiles, robotX, robotY, 0, -1, false);

            --robotY;
          }

          break;
        }

        case 'v': {
          if (move(tiles, robotX, robotY, 0, 1, true)) {
            move(tiles, robotX, robotY, 0, 1, false);

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
    for (std::int64_t x = 0; x < (std::int64_t) lines[0].size() * 2; ++x) {
      if (tiles[y][x]->hasBox && tiles[y][x]->left) {
        sum += (100 * y) + x;
      }
    }
  }

  std::cout << "Sum of GPS co-ordinates = " << sum << std::endl;

  for (std::int64_t y = 0; y < (std::int64_t) lines[0].size(); ++y) {
    for (std::int64_t x = 0; x < (std::int64_t) lines[0].size() * 2; ++x) {
      delete tiles[y][x];
    }

    delete [] tiles[y];
  }

  delete [] tiles;
}
