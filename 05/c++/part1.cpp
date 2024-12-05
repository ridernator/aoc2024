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

int main() {
  const auto& data = readFileToVector();

  std::size_t index;
  std::uint64_t before;
  std::uint64_t after;

  std::vector<Rule> rules;
  std::vector<std::vector<std::uint64_t>> manuals;

  for (index = 0; index < data.size(); ++index) {
    if (data[index].empty()) {
      break;
    } else {
      sscanf(data[index].c_str(), "%lu|%lu", &before, &after);

      rules.emplace_back(before, after);
    }
  }

  for (index = index + 1; index < data.size(); ++index) {
    std::stringstream ss(data[index]);
    std::string temp;
    std::vector<std::uint64_t> manual;

    while (std::getline(ss, temp, ',')) {
      manual.push_back(std::strtoul(temp.c_str(), nullptr, 10));
    }

    manuals.push_back(manual);
  }

  std::uint64_t wellOrderedSum = 0;

  for (const auto& manual : manuals) {
    bool wellOrdered = true;

    for (std::size_t page = 0; page < manual.size(); ++page) {
      for (const auto& rule : rules) {
        if (rule.before == manual[page]) {
          for (std::size_t index = 0; index < page; ++index) {
            if (manual[index] == rule.after) {
              wellOrdered = false;

              break;
            }
          }

          if (!wellOrdered) {
            break;
          }
        }
      }

      if (!wellOrdered) {
        break;
      }
    }

    if (wellOrdered) {
      wellOrderedSum += manual[manual.size() / 2];
    }
  }

  std::cout << "Sum of centre pages of well-ordered manuals = " << wellOrderedSum << std::endl;
}
