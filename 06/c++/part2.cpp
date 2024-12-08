#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <set>
#include <string>
#include <sstream>
#include <fstream>
#include <tuple>
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

struct Rule {
  std::uint64_t before;
  std::uint64_t after;
};

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
};

enum Direction {
  N,
  S,
  E,
  W
};

void increment(Direction& direction) {
  switch (direction) {
    case Direction::N: {
      direction = Direction::E;

      break;
    }

    case Direction::E: {
      direction = Direction::S;

      break;
    }

    case Direction::S: {
      direction = Direction::W;

      break;
    }

    case Direction::W: {
      direction = Direction::N;

      break;
    }
  }
}

void step(Position& position, const Direction direction) {
  switch (direction) {
    case Direction::N: {
      --position.y;

      break;
    }

    case Direction::E: {
      ++position.x;

      break;
    }

    case Direction::S: {
      ++position.y;

      break;
    }

    case Direction::W: {
      --position.x;

      break;
    }
  }
}

bool doesLoop(const std::vector<Position>& walls,
              const Position& guardStart,
              const std::int64_t maxX,
              const std::int64_t maxY) {
  Position guard = guardStart;
  Direction guardDirection = Direction::N;
  std::vector<std::tuple<Position, Direction>> steps;
  Position nextStep;
  bool returnVal = false;

  while ((guard.x >= 0) && (guard.x < maxX) &&
         (guard.y >= 0) && (guard.y < maxY)) {
    if (std::find(steps.begin(), steps.end(), std::make_tuple(guard, guardDirection)) == steps.end()) {
      steps.emplace_back(guard, guardDirection);
    } else {
      returnVal = true;

      break;
    }

    nextStep = guard;
    step(nextStep, guardDirection);

    while (std::find(walls.begin(), walls.end(), nextStep) != walls.end()) {
      increment(guardDirection);

      nextStep = guard;
      step(nextStep, guardDirection);
    }

    guard = nextStep;
  }

  return returnVal;
}

int main() {
  const auto& data = readFileToVector();

  std::vector<Position> walls;

  Position guard;

  for (std::size_t rowIndex = 0; rowIndex < data.size(); ++rowIndex) {
    for (std::size_t colIndex = 0; colIndex < data[0].size(); ++colIndex) {
      if (data[rowIndex][colIndex] == '#') {
        walls.emplace_back(colIndex, rowIndex);
      } else if (data[rowIndex][colIndex] == '^') {
        guard.x = colIndex;
        guard.y = rowIndex;
      }
    }
  }

  Position nextStep;
  Direction guardDirection = Direction::N;
  std::set<Position> firstRunSteps;
  Position guardCopy = guard;

  while ((guardCopy.x >= 0) && (guardCopy.x < (std::int64_t) data[0].size()) &&
         (guardCopy.y >= 0) && (guardCopy.y < (std::int64_t) data.size())) {
    nextStep = guardCopy;
    step(nextStep, guardDirection);

    while (std::find(walls.begin(), walls.end(), nextStep) != walls.end()) {
      increment(guardDirection);

      nextStep = guardCopy;
      step(nextStep, guardDirection);
    }

    firstRunSteps.insert(guardCopy);
    guardCopy = nextStep;
  }

  std::uint64_t loopCount = 0;
  Position testPos;

  for (std::size_t rowIndex = 0; rowIndex < data.size(); ++rowIndex) {
    for (std::size_t colIndex = 0; colIndex < data[0].size(); ++colIndex) {
      testPos.x = colIndex;
      testPos.y = rowIndex;

      if ((std::find(firstRunSteps.begin(), firstRunSteps.end(), testPos) != firstRunSteps.end()) &&
          (std::find(walls.begin(), walls.end(), testPos) == walls.end())) {
        walls.push_back(testPos);

        if (doesLoop(walls, guard, data[0].size(), data.size())) {
          ++loopCount;
        }

        walls.pop_back();
      }
    }
  }

  std::cout << "Number of places which would cause loops = " << loopCount << std::endl;
}
