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

// AXa + BXb = Xp
// AYa + BYb = Yp
//
// AXa = Xp - BXb
// AYa = Yp - BYb
//
// A = (Xp - BXb) / Xa
// A = (Yp - BYb) / Ya
//
// (Xp - BXb) / Xa = (Yp - BYb) / Ya
// Xp - BXb = Xa * (Yp - BYb) / Ya
// (Xp - BXb) * Ya = (Yp - BYb) * Xa
// YaXp - YaBXb = XaYp - XaBYb
// XaBYb - YaBXb = XaYp - YaXp
// B(XaYb - YaXb) = XaYp - YaXp
// ####### B = (XaYp - YaXp) / (XaYb - YaXb)
//
// AXa + BXb = Xp
// AYa + BYb = Yp
//
// BXb = Xp - AXa
// BYb = Yp - AYa
//
// B = (Xp - AXa) / Xb
// B = (Yp - AYa) / Yb
//
// Yb * (Xp - AXa) = Xb * (Yp - AYa)
// YbXp - YbAXa = XbYp - XbAYa
// XbAYa - YbAXa = XbYp - YbXp
// A(XbYa - YbXa) = XbYp - YbXp
// ###### A = (XbYp - YbXp) / (XbYa - YbXa)

int main() {
  const auto& lines = readFileToVector();
  std::int64_t Xa;
  std::int64_t Ya;
  std::int64_t Xb;
  std::int64_t Yb;
  std::int64_t Xp;
  std::int64_t Yp;
  std::int64_t aCost = 3;
  std::int64_t bCost = 1;
  std::int64_t sum = 0;
  std::int64_t a;
  std::int64_t b;

  for (std::size_t index = 0; index < lines.size(); index += 4) {
    sscanf(lines[index].c_str(), "Button A: X+%li, Y+%li", &Xa, &Ya);
    sscanf(lines[index + 1].c_str(), "Button B: X+%li, Y+%li", &Xb, &Yb);
    sscanf(lines[index + 2].c_str(), "Prize: X=%li, Y=%li", &Xp, &Yp);

    // A = (XbYp - YbXp) / (XbYa - YbXa)
    a = (Xb * Yp - Yb * Xp) / (Xb * Ya - Yb * Xa);

    // B = (XaYp - YaXp) / (XaYb - YaXb)
    b = (Xa * Yp - Ya * Xp) / (Xa * Yb - Ya * Xb);

    if ((a * Xa + b * Xb == Xp) && (a * Ya + b * Yb == Yp)) {
      sum += a * aCost + b * bCost;
    }
  }

  std::cout << "Fewest tokens needed = " << sum << std::endl;
}
