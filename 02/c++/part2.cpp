#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
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

bool isGood(const std::vector<std::int64_t>& nums) {
  for (std::size_t ignoredIndex = 0; ignoredIndex < nums.size(); ++ignoredIndex) {
    bool good = true;

    for (std::size_t index = (ignoredIndex < 2 ? 2 : 1); index < nums.size(); index = (ignoredIndex == index + 1 ? index + 2 : index + 1)) {
      if ((nums[index] <= nums[(ignoredIndex == index - 1 ? index - 2 : index - 1)]) ||
          (nums[index] - nums[(ignoredIndex == index - 1 ? index - 2 : index - 1)] > 3)) {
        good = false;

        break;
      }
    }

    if (good) {
      return true;
    }

    good = true;
    for (std::size_t index = (ignoredIndex < 2 ? 2 : 1); index < nums.size(); index = (ignoredIndex == index + 1 ? index + 2 : index + 1)) {
      if ((nums[index] >= nums[(ignoredIndex == index - 1 ? index - 2 : index - 1)]) ||
          (nums[index] - nums[(ignoredIndex == index - 1 ? index - 2 : index - 1)] < -3)) {
        good = false;

        break;
      }
    }

    if (good) {
      return good;
    }
  }

  return false;
}

int main() {
  const auto& lines = readFileToVector();

  std::uint64_t safeCount = 0;
  std::string tempString;

  for (const auto& line : lines) {
    std::vector<std::int64_t> data;
    std::stringstream ss(line);

    while (std::getline(ss, tempString, ' ')) {
      data.push_back(std::strtoll(tempString.c_str(), nullptr, 10));
    }

    if (isGood(data)) {
      ++safeCount;
    }
  }

  std::cout << "Number of safe levels = " << safeCount << std::endl;
}
