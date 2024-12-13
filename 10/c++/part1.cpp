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

bool canReach(      std::int32_t** data,
              const std::int32_t   x,
              const std::int32_t   y,
              const std::int32_t   targetX,
              const std::int32_t   targetY,
              const std::int32_t   maxX,
              const std::int32_t   maxY) {
  if ((x == targetX) && (y == targetY)) {
    return true;
  }

  for (std::int32_t step = -1; step <= 1; step += 2) {
    if ((x + step >= 0) && (x + step < maxX) &&
        (*(*(data + y) + x + step) == *(*(data + y) + x) + 1)) {
      if (canReach(data, x + step, y, targetX, targetY, maxX, maxY)) {
        return true;
      }
    }

    if ((y + step >= 0) && (y + step < maxY) &&
        (*(*(data + y + step) + x) == *(*(data + y) + x) + 1)) {
      if (canReach(data, x, y + step, targetX, targetY, maxX, maxY)) {
        return true;
      }
    }
  }

  return false;
}

int main() {
  const auto& lines = readFileToVector();

  std::int32_t** data = new std::int32_t*[lines.size()];
  std::int32_t maxX = lines[0].size();
  std::int32_t maxY = lines.size();

  for (std::int32_t y = 0; y < maxY; ++y) {
    *(data + y) = new std::int32_t[maxX];
  }

  for (std::int32_t y = 0; y < maxY; ++y) {
    for (std::int32_t x = 0; x < maxX; ++x) {
      if (lines[y][x] == '.') {
        *(*(data + y) + x) = -1;
      } else {
        *(*(data + y) + x) = lines[y][x] - 48;
      }
    }
  }

  std::uint64_t sum = 0;

  for (std::int32_t y = 0; y < maxY; ++y) {
    for (std::int32_t x = 0; x < maxX; ++x) {
      if (*(*(data + y) + x) == 0) {
        for (std::int32_t targetY = 0; targetY < maxY; ++targetY) {
          for (std::int32_t targetX = 0; targetX < maxX; ++targetX) {
            if (*(*(data + targetY) + targetX) == 9) {
              if (canReach(data, x, y, targetX, targetY, maxX, maxY)) {
                ++sum;
              }
            }
          }
        }
      }
    }
  }

  std::cout << "Sum of scores = " << sum << std::endl;

  for (std::int32_t y = 0; y < maxY; ++y) {
    delete [] *(data + y);
  }

  delete [] data;
}
