#include <cctype>
#include <cstddef>
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

bool isPossible(const std::string& design,
                const std::vector<std::string>& towels) {
  for (const auto& towel : towels) {
    if (design.size() >= towel.size()) {
      if (design.compare(0, towel.size(), towel) == 0) {
        if (design.size() == towel.size()) {
          return true;
        } else if (isPossible(design.substr(towel.size()), towels)) {
          return true;
        }
      }
    }
  }

  return false;
}

int main() {
  const auto& lines = readFileToVector();

  std::stringstream ss(lines[0]);
  std::string tempString;
  std::vector<std::string> towels;

  while (std::getline(ss, tempString, ',')) {
    while (tempString[0] == ' ') {
      tempString.erase(tempString.begin());
    }

    towels.push_back(tempString);
  }

  for (auto iterator = towels.begin(); iterator != towels.end();) {
    std::vector<std::string> towelsCopy;

    for (std::size_t index = 0; index < towels.size(); ++index) {
      if (towels[index] != *iterator) {
        towelsCopy.push_back(towels[index]);
      }
    }

    if (isPossible(*iterator, towelsCopy)) {
      iterator = towels.erase(iterator);
    } else {
      ++iterator;
    }
  }

  std::uint64_t possibles = 0;

  for (std::size_t index = 2; index < lines.size(); ++index) {
    if (isPossible(lines[index], towels)) {
      ++possibles;
    }
  }

  std::cout << "Possible designs = " << possibles << std::endl;
}
