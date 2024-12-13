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

struct Plot {
 public:
  Plot(const char value,
       const bool isCollected) : value(value),
                                 isCollected(isCollected) {}
  char value;
  bool isCollected;
  std::vector<Plot*> neighbours;
};


void growRegion(Plot* plot,
                std::vector<Plot*>& region) {
  for (const auto& neighbour : plot->neighbours) {
    if ((!neighbour->isCollected) && (neighbour->value == plot->value)) {
      region.push_back(neighbour);
      neighbour->isCollected = true;

      growRegion(neighbour, region);
    }
  }
}

std::int64_t getPerimeter(const std::vector<Plot*>& region) {
  std::int64_t perimeter = 0;

  for (const auto& plot : region) {
    perimeter += 4;

    for (const auto& neighbour : plot->neighbours) {
      if (neighbour->value == plot->value) {
        --perimeter;
      }
    }
  }

  return perimeter;
}

int main() {
  const auto& lines = readFileToVector();

  Plot*** plots = new Plot**[lines.size()];
  std::vector<std::vector<Plot*>> regions;

  for (std::int64_t y = 0; y < (std::int64_t) lines.size(); ++y) {
    plots[y] = new Plot*[lines[0].size()];

    for (std::int64_t x = 0; x < (std::int64_t) lines[0].size(); ++x) {
      plots[y][x] = new Plot(lines[y][x], false);
    }
  }

  for (std::int64_t y = 0; y < (std::int64_t) lines.size(); ++y) {
    for (std::int64_t x = 0; x < (std::int64_t) lines[0].size(); ++x) {
      for (std::int64_t step = -1; step <= 1; step += 2) {
        if ((x + step >= 0) && (x + step < (std::int64_t) lines[0].size())) {
          plots[y][x]->neighbours.push_back(plots[y][x + step]);
        }

        if ((y + step >= 0) && (y + step < (std::int64_t) lines.size())) {
          plots[y][x]->neighbours.push_back(plots[y + step][x]);
        }
      }
    }
  }

  for (std::int64_t y = 0; y < (std::int64_t) lines.size(); ++y) {
    for (std::int64_t x = 0; x < (std::int64_t) lines[0].size(); ++x) {
      if (!plots[y][x]->isCollected) {
        std::vector<Plot*> region;

        region.push_back(plots[y][x]);
        plots[y][x]->isCollected = true;

        growRegion(plots[y][x], region);

        regions.push_back(region);
      }
    }
  }

  std::int64_t fencingCost = 0;
  std::int64_t perimeter;

  for (const auto& region : regions) {
    perimeter = getPerimeter(region);
    fencingCost += perimeter * region.size();

    // std::cout << "Region " << region[0]->value << ", area " << region.size() << ", perimeter " << perimeter << std::endl;
  }

  std::cout << "Total fencing cost = " << fencingCost << std::endl;

  for (std::int64_t y = 0; y < (std::int64_t) lines.size(); ++y) {
    for (std::int64_t x = 0; x < (std::int64_t) lines[0].size(); ++x) {
      delete plots[y][x];
    }

    delete [] plots[y];
  }

  delete [] plots;
}
