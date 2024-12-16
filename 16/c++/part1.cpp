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
  Tile(const bool isWall,
       const bool isEnd) : isWall(isWall),
                           isEnd(isEnd) {}

  const bool isWall;
  const bool isEnd;
  std::int64_t distance = std::numeric_limits<std::int64_t>::max();
  std::int64_t xVelocity;
  std::int64_t yVelocity;
};

void print(Tile*** tiles,
           const std::int64_t maxX,
           const std::int64_t maxY) {
  for (std::int64_t y = 0; y < maxY; ++y) {
    for (std::int64_t x = 0; x < maxX; ++x) {
      if (tiles[y][x]->isWall) {
        std::cout << '#';
      } else if (tiles[y][x]->isEnd) {
        std::cout << 'E';
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

  Tile*** tiles = new Tile**[lines.size()];
  Tile* endTile = nullptr;

  for (std::int64_t y = 0; y < (std::int64_t) lines.size(); ++y) {
    tiles[y] = new Tile*[lines[0].size()];

    for (std::int64_t x = 0; x < (std::int64_t) lines[0].size(); ++x) {
      tiles[y][x] = new Tile((lines[y][x] == '#'), (lines[y][x] == 'E'));

      if (lines[y][x] == 'S') {
        tiles[y][x]->distance = 0;
        tiles[y][x]->xVelocity = 1;
        tiles[y][x]->yVelocity = 0;
      }

      if (lines[y][x] == 'E') {
        endTile = tiles[y][x];
      }
    }
  }

  print(tiles, lines[0].size(), lines.size());

  std::int64_t distance = 0;

  while (endTile->distance == std::numeric_limits<std::int64_t>::max()) {
    for (std::int64_t y = 0; y < (std::int64_t) lines.size(); ++y) {
      for (std::int64_t x = 0; x < (std::int64_t) lines[0].size(); ++x) {
        if (tiles[y][x]->distance == distance) {
          if (tiles[y][x]->xVelocity == 0) {
            if ((!tiles[y + tiles[y][x]->yVelocity][x]->isWall) &&
                (tiles[y + tiles[y][x]->yVelocity][x]->distance > distance + 1)) {
              tiles[y + tiles[y][x]->yVelocity][x]->distance = distance + 1;
              tiles[y + tiles[y][x]->yVelocity][x]->yVelocity = tiles[y][x]->yVelocity;
              tiles[y + tiles[y][x]->yVelocity][x]->xVelocity = 0;
            }

            if ((!tiles[y][x + 1]->isWall) &&
                (tiles[y][x + 1]->distance > distance + 1001)) {
              tiles[y][x + 1]->distance = distance + 1001;
              tiles[y][x + 1]->xVelocity = 1;
              tiles[y][x + 1]->yVelocity = 0;
            }

            if ((!tiles[y][x - 1]->isWall) &&
                (tiles[y][x - 1]->distance > distance + 1001)) {
              tiles[y][x - 1]->distance = distance + 1001;
              tiles[y][x - 1]->xVelocity = -1;
              tiles[y][x - 1]->yVelocity = 0;
            }
          } else {
            if ((!tiles[y][x + tiles[y][x]->xVelocity]->isWall) &&
                (tiles[y][x + tiles[y][x]->xVelocity]->distance > distance + 1)) {
              tiles[y][x + tiles[y][x]->xVelocity]->distance = distance + 1;
              tiles[y][x + tiles[y][x]->xVelocity]->xVelocity = tiles[y][x]->xVelocity;
              tiles[y][x + tiles[y][x]->xVelocity]->yVelocity = 0;
            }

            if ((!tiles[y + 1][x]->isWall) &&
                (tiles[y + 1][x]->distance > distance + 1001)) {
              tiles[y + 1][x]->distance = distance + 1001;
              tiles[y + 1][x]->yVelocity = 1;
              tiles[y + 1][x]->xVelocity = 0;
            }

            if ((!tiles[y - 1][x]->isWall) &&
                (tiles[y - 1][x]->distance > distance + 1001)) {
              tiles[y - 1][x]->distance = distance + 1001;
              tiles[y - 1][x]->yVelocity = -1;
              tiles[y - 1][x]->xVelocity = 0;
            }
          }
        }
      }
    }

    ++distance;
  }

  std::cout << "Best score is " << endTile->distance << std::endl;

  for (std::int64_t y = 0; y < (std::int64_t) lines.size(); ++y) {
    for (std::int64_t x = 0; x < (std::int64_t) lines[0].size(); ++x) {
      delete tiles[y][x];
    }

    delete [] tiles[y];
  }

  delete [] tiles;
}
