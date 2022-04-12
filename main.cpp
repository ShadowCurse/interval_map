#include "interval_map.h"
#include <array>
#include <cstdlib>
#include <iostream>

void print_data(int begin, int end, char val, const interval_map<Key, Value>& im, const std::array<char, 10>& vals) {
  std::cout << "begin: " << begin << " end: " << end << " char: " << val << '\n';
  std::cout << "map values: \n";
  for (auto i{0}; i < 10; i++) {
      std::cout << im[i] << " ";
  }       
  std::cout << "\n";
  std::cout << "ref values: \n";
  for (auto i{0}; i < 10; i++) {
      std::cout << vals[i] << " ";
  }       
  std::cout << "\n";
}

auto test_range(int begin, int end, char val, const interval_map<Key, Value>& im, const std::array<char, 10>& vals) -> bool {
  for (auto i{0}; i < 10; ++i) {
    if (!(im[i] == vals[i])) {
        std::cout << "ERROR\n";
        return false;
    }
  }
  return true;
}

int main() {
  srand(time(nullptr));

  auto im = interval_map<Key, Value>{'A'};
  auto interval = std::array{'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A'};
  auto chars = std::array{'A', 'B', 'C', 'D', 'E', 'F', 'G'};
  for (auto i{0}; i < 100000000; i++) {
    auto begin = rand() % interval.size();
    auto end = rand() % interval.size();
    auto val = rand() % chars.size();
    im.assign(begin, end, chars[val]);
    for (auto j{begin}; j < end; j++)
      interval[j] = chars[val];
    // print_data(begin, end, val, im, interval);
    if (!test_range(begin, end, chars[val], im, interval))
        return -1;
  }

  return 0;
}
