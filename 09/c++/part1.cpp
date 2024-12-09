#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
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

  while (std::find(disk.begin(), disk.end(), EMPTY) != disk.end()) {
    // print(disk);

    *(std::find(disk.begin(), disk.end(), EMPTY)) = disk.back();
    disk.pop_back();

    while (disk.back() == EMPTY) {
      disk.pop_back();
    }
  }

  // print(disk);

  std::uint64_t checksum = 0;
  for (std::size_t index = 0; index < disk.size(); ++index) {
    checksum += index * disk[index];
  }

  std::cout << "Checksum = " << checksum << std::endl;
}
