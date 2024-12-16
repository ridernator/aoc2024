#include <algorithm>
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
};

void print(Tile*** tiles,
           Tile* reindeer,
           const std::int64_t maxX,
           const std::int64_t maxY) {
  for (std::int64_t y = 0; y < maxY; ++y) {
    for (std::int64_t x = 0; x < maxX; ++x) {
      if (tiles[y][x]->isWall) {
        std::cout << '#';
      } else if (tiles[y][x]->isEnd) {
        std::cout << 'E';
      } else if (tiles[y][x] == reindeer) {
        std::cout << '@';
      } else {
        std::cout << '.';
      }
    }

    std::cout << std::endl;
  }

  std::cout << std::endl;
}

std::int64_t minScore = std::numeric_limits<std::int64_t>::max();

void solve(Tile*** tiles,
           const std::int64_t reindeerX,
           const std::int64_t reindeerY,
           const std::int64_t reindeerVelocityX,
           const std::int64_t reindeerVelocityY,
           const std::int64_t currentScore,
           std::vector<Tile*>& usedTiles) {
  if (currentScore < minScore) {
    if (tiles[reindeerY][reindeerX]->isEnd) {
      if (currentScore < minScore) {
        minScore = currentScore;

        std::cout << "minScore is now " << minScore << std::endl;
      }
    } else {
      usedTiles.push_back(tiles[reindeerY][reindeerX]);

      if (reindeerVelocityX == 0) {
        if ((!tiles[reindeerY + reindeerVelocityY][reindeerX]->isWall) &&
            (std::find(usedTiles.begin(), usedTiles.end(), tiles[reindeerY + reindeerVelocityY][reindeerX]) == usedTiles.end())) {
          solve(tiles, reindeerX, reindeerY + reindeerVelocityY, reindeerVelocityX, reindeerVelocityY, currentScore + 1, usedTiles);
        }

        if ((!tiles[reindeerY][reindeerX + 1]->isWall) &&
            (std::find(usedTiles.begin(), usedTiles.end(), tiles[reindeerY][reindeerX + 1]) == usedTiles.end())) {
          solve(tiles, reindeerX + 1, reindeerY, 1, 0, currentScore + 1001, usedTiles);
        }

        if ((!tiles[reindeerY][reindeerX - 1]->isWall) &&
            (std::find(usedTiles.begin(), usedTiles.end(), tiles[reindeerY][reindeerX - 1]) == usedTiles.end())) {
          solve(tiles, reindeerX - 1, reindeerY, -1, 0, currentScore + 1001, usedTiles);
        }
      } else {
        if ((!tiles[reindeerY][reindeerX + reindeerVelocityX]->isWall) &&
            (std::find(usedTiles.begin(), usedTiles.end(), tiles[reindeerY][reindeerX + reindeerVelocityX]) == usedTiles.end())) {
          solve(tiles, reindeerX + reindeerVelocityX, reindeerY, reindeerVelocityX, reindeerVelocityY, currentScore + 1, usedTiles);
        }

        if ((!tiles[reindeerY + 1][reindeerX]->isWall) &&
            (std::find(usedTiles.begin(), usedTiles.end(), tiles[reindeerY + 1][reindeerX]) == usedTiles.end())) {
          solve(tiles, reindeerX, reindeerY + 1, 0, 1, currentScore + 1001, usedTiles);
        }

        if ((!tiles[reindeerY - 1][reindeerX]->isWall) &&
            (std::find(usedTiles.begin(), usedTiles.end(), tiles[reindeerY - 1][reindeerX]) == usedTiles.end())) {
          solve(tiles, reindeerX, reindeerY - 1, 0, -1, currentScore + 1001, usedTiles);
        }
      }

      usedTiles.pop_back();
    }
  }
}

int main() {
  const auto& lines = readFileToVector();

  Tile*** tiles = new Tile**[lines.size()];
  std::int64_t reindeerX = 1;
  std::int64_t reindeerY = 1;

  for (std::int64_t y = 0; y < (std::int64_t) lines.size(); ++y) {
    tiles[y] = new Tile*[lines[0].size()];

    for (std::int64_t x = 0; x < (std::int64_t) lines[0].size(); ++x) {
      tiles[y][x] = new Tile((lines[y][x] == '#'), (lines[y][x] == 'E'));

      if (lines[y][x] == 'S') {
        reindeerX = x;
        reindeerY = y;
      }
    }
  }

  print(tiles, tiles[reindeerY][reindeerX], lines[0].size(), lines.size());

  std::vector<Tile*> usedTiles;
  solve(tiles, reindeerX, reindeerY, 1, 0, 0, usedTiles);

  for (std::int64_t y = 0; y < (std::int64_t) lines.size(); ++y) {
    for (std::int64_t x = 0; x < (std::int64_t) lines[0].size(); ++x) {
      delete tiles[y][x];
    }

    delete [] tiles[y];
  }

  delete [] tiles;
}
