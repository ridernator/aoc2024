#include <algorithm>
#include <cstddef>
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

struct Plot {
 public:
  Plot(const char value,
       const bool isCollected,
       const std::int64_t x,
       const std::int64_t y) : value(value),
                               isCollected(isCollected),
                               x(x),
                               y(y) {}
  char value;
  bool isCollected;
  std::vector<Plot*> neighbours;
  std::int64_t x;
  std::int64_t y;
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

std::int64_t countGroups(const std::vector<std::int64_t>& values) {
  if (values.empty()) {
    return 0;
  }

  std::int64_t groups = 1;

  for (std::size_t index = 1; index < values.size(); ++index) {
    if (values[index] != values[index - 1] + 1) {
      ++groups;
    }
  }

  return groups;
}

std::int64_t getNumSides(const std::vector<Plot*>& region) {
  std::int64_t minX = std::numeric_limits<std::int64_t>::max();
  std::int64_t maxX = std::numeric_limits<std::int64_t>::min();
  std::int64_t minY = std::numeric_limits<std::int64_t>::max();
  std::int64_t maxY = std::numeric_limits<std::int64_t>::min();
  std::vector<std::int64_t> edges;
  std::int64_t sides = 0;

  for (const auto& plot : region) {
    minX = std::min(minX, plot->x);
    maxX = std::max(maxX, plot->x);
    minY = std::min(minY, plot->y);
    maxY = std::max(maxY, plot->y);
  }

  for (std::int64_t x = minX; x <= maxX; ++x) {
    edges.clear();

    for (std::int64_t y = minY; y <= maxY; ++y) {
      if (std::find_if(region.begin(), region.end(), [&x, &y] (const auto& plot) {
                                                       return ((plot->x == x) && (plot->y == y));
                                                     }) != region.end()) {
        if (std::find_if(region.begin(), region.end(), [&x, &y] (const auto& plot) {
                                                         return ((plot->x == x - 1) && (plot->y == y));
                                                       }) == region.end()) {
          edges.push_back(y);
        }
      }
    }

    sides += countGroups(edges);

    edges.clear();

    for (std::int64_t y = minY; y <= maxY; ++y) {
      if (std::find_if(region.begin(), region.end(), [&x, &y] (const auto& plot) {
                                                       return ((plot->x == x) && (plot->y == y));
                                                     }) != region.end()) {
        if (std::find_if(region.begin(), region.end(), [&x, &y] (const auto& plot) {
                                                         return ((plot->x == x + 1) && (plot->y == y));
                                                       }) == region.end()) {
          edges.push_back(y);
        }
      }
    }

    sides += countGroups(edges);
  }

  for (std::int64_t y = minY; y <= maxY; ++y) {
    edges.clear();

    for (std::int64_t x = minX; x <= maxX; ++x) {
      if (std::find_if(region.begin(), region.end(), [&x, &y] (const auto& plot) {
                                                       return ((plot->x == x) && (plot->y == y));
                                                     }) != region.end()) {
        if (std::find_if(region.begin(), region.end(), [&x, &y] (const auto& plot) {
                                                         return ((plot->x == x) && (plot->y == y - 1));
                                                       }) == region.end()) {
          edges.push_back(x);
        }
      }
    }

    sides += countGroups(edges);

    edges.clear();

    for (std::int64_t x = minX; x <= maxX; ++x) {
      if (std::find_if(region.begin(), region.end(), [&x, &y] (const auto& plot) {
                                                       return ((plot->x == x) && (plot->y == y));
                                                     }) != region.end()) {
        if (std::find_if(region.begin(), region.end(), [&x, &y] (const auto& plot) {
                                                         return ((plot->x == x) && (plot->y == y + 1));
                                                       }) == region.end()) {
          edges.push_back(x);
        }
      }
    }

    sides += countGroups(edges);
  }

  return sides;
}

int main() {
  const auto& lines = readFileToVector();

  Plot*** plots = new Plot**[lines.size()];
  std::vector<std::vector<Plot*>> regions;

  for (std::int64_t y = 0; y < (std::int64_t) lines.size(); ++y) {
    plots[y] = new Plot*[lines[0].size()];

    for (std::int64_t x = 0; x < (std::int64_t) lines[0].size(); ++x) {
      plots[y][x] = new Plot(lines[y][x], false, x, y);
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
  std::int64_t numSides;

  for (const auto& region : regions) {
    numSides = getNumSides(region);
    fencingCost += numSides * region.size();

    // std::cout << "Region " << region[0]->value << ", area " << region.size() << ", sides " << numSides << std::endl;
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
