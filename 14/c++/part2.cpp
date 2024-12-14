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

class Robot {
 public:
  Robot(const std::int64_t px,
        const std::int64_t py,
        const std::int64_t vx,
        const std::int64_t vy) : px(px),
                                 py(py),
                                 vx(vx),
                                 vy(vy) {}

  void step(const std::int64_t maxX,
            const std::int64_t maxY) {
    px += vx;
    py += vy;

    if (px < 0) {
      px += maxX;
    } else if (px >= maxX) {
      px -= maxX;
    }

    if (py < 0) {
      py += maxY;
    } else if (py >= maxY) {
      py -= maxY;
    }
  }

  std::int64_t getQuadrant(const std::int64_t maxX,
                           const std::int64_t maxY) {
    if (px < maxX / 2) {
      if (py < maxY / 2) {
        return 0;
      } else if (py > maxY / 2) {
        return 1;
      } else {
        return 4;
      }
    } else if (px > maxX / 2) {
      if (py < maxY / 2) {
        return 2;
      } else if (py > maxY / 2) {
        return 3;
      } else {
        return 4;
      }
    } else {
      return 4;
    }
  }

  std::int64_t px;
  std::int64_t py;

  const std::int64_t vx;
  const std::int64_t vy;
};

int main() {
  const auto& lines = readFileToVector();
  std::vector<Robot> robots;
  std::int64_t px;
  std::int64_t py;
  std::int64_t vx;
  std::int64_t vy;
  std::int64_t maxX = 101;
  std::int64_t maxY = 103;

  for (const auto& line : lines) {
    sscanf(line.c_str(), "p=%li,%li v=%li,%li", &px, &py, &vx, &vy);

    robots.emplace_back(px, py, vx, vy);
  }

  std::uint64_t quadrants[5];
  std::uint64_t safetyFactor;
  std::uint64_t bestSafetyFactor = std::numeric_limits<std::uint64_t>::max();

  for (std::uint64_t index = 1; index < 1000000; ++index) {
    quadrants[0] = 0;
    quadrants[1] = 0;
    quadrants[2] = 0;
    quadrants[3] = 0;

    for (auto& robot : robots) {
      robot.step(maxX, maxY);
      ++quadrants[robot.getQuadrant(maxX, maxY)];
    }

    safetyFactor = (quadrants[0] * quadrants[1] * quadrants[2] * quadrants[3]);

    if (safetyFactor < bestSafetyFactor) {
      bestSafetyFactor = safetyFactor;

      std::cout << "Picture at " << index << std::endl;

      for (std::int64_t y = 0; y < maxY; ++y) {
        for (std::int64_t x = 0; x < maxX; ++x) {
          if (std::find_if(robots.begin(), robots.end(), [&x, &y] (const auto& robot) {
                                                           return ((robot.px == x) && (robot.py == y));
                                                         }) == robots.end()) {
            std::cout << '.';
          } else {
            std::cout << 'X';
          }
        }

        std::cout << std::endl << std::endl;
      }
    }
  }
}
