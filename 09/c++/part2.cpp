#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <limits>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#define INPUT "../data/input"

const std::uint64_t EMPTY = std::numeric_limits<std::uint64_t>::max();

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

void print(std::vector<std::uint64_t>& disk) {
  for (std::size_t index = 0; index < disk.size(); ++index) {
    if (disk[index] == EMPTY) {
      std::cout << '.';
    } else {
      std::cout << disk[index];
    }
  }
  std::cout << std::endl;
}

const std::vector<std::uint64_t> GAPS = { EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY };

std::size_t findGap(std::vector<std::uint64_t>& disk,
                    const std::int64_t gap) {
  auto iter = std::search(disk.begin(), disk.end(), GAPS.end() - gap, GAPS.end());

  if (iter == disk.end()) {
    return 0;
  } else {
    return std::distance(disk.begin(), iter);
  }
}

int main() {
  const auto& lines = readFileToVector();

  std::vector<std::uint64_t> disk;

  bool isGap = false;
  std::uint8_t size;
  std::uint64_t id = 0;

  for (char fileData : lines[0]) {
    size = fileData - 48;

    for (std::uint8_t index = 0; index < size; ++index) {
      if (isGap) {
        disk.emplace_back(EMPTY);
      } else {
        disk.emplace_back(id);
      }
    }

    if (isGap) {
      isGap = false;
    } else {
      ++id;

      isGap = true;
    }
  }

  for (std::int64_t index = disk.size() - 1; index >= 0; --index) {
    // print(disk);

    if (disk[index] == EMPTY) {
      continue;
    }

    std::int64_t startIndex = index;

    while ((startIndex >= 0) && (disk[startIndex] == disk[index])) {
      --startIndex;
    }
    ++startIndex;

    if (startIndex < 0) {
      break;
    }

    std::size_t gap = findGap(disk, index + 1 - startIndex);

    if ((gap != 0) && (gap < (std::size_t) startIndex)) {
      for (std::size_t copyIndex = gap; copyIndex < gap + (index + 1 - (std::size_t) startIndex); ++copyIndex) {
        disk[copyIndex] = disk[startIndex + (copyIndex - gap)];
        disk[startIndex + (copyIndex - gap)] = EMPTY;
      }
    }

    index = startIndex;
  }

  // print(disk);

  std::uint64_t checksum = 0;
  for (std::size_t index = 0; index < disk.size(); ++index) {
    if (disk[index] != EMPTY) {
      checksum += index * disk[index];
    }
  }

  std::cout << "Checksum = " << checksum << std::endl;
}
