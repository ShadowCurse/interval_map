#include <array>
#include <gtest/gtest.h>
#include "interval_map.h"

void test_map(const std::map<Key, Value> &test, const std::map<Key, Value> &ref) {
  EXPECT_EQ(test.size(), ref.size());
  for (const auto &r: ref) {
    EXPECT_EQ(test.at(r.first), r.second);
  }
}

void test_range(const interval_map<Key, Value>& im, const std::array<char, 10>& vals) {
  for (auto i{0}; i < 10; ++i) {
    EXPECT_EQ(im[i], vals[i]);
  }
}

TEST(test, empty) {
  auto im = interval_map<Key, Value>{'A'};
  test_map(im.m_map, {});
  test_range(im, {'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A'});
}

TEST(test, one_interval) {
  auto im = interval_map<Key, Value>{'A'};
  im.assign(4, 6, 'C');
  test_map(im.m_map, {{4, 'C'}, {6, 'A'}});
  test_range(im, {'A', 'A', 'A', 'A', 'C', 'C', 'A', 'A', 'A', 'A'});
}

TEST(test, invalid_range) {
  auto im = interval_map<Key, Value>{'A'};
  im.assign(7, 4, 'C');
  test_map(im.m_map, {});
  test_range(im, {'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A'});
}

TEST(test, invalid_range_default_value) {
  auto im = interval_map<Key, Value>{'A'};
  im.assign(4, 6, 'A');
  test_map(im.m_map, {});
  test_range(im, {'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A'});
}

TEST(test, invalid_range_default_value_before) {
  auto im = interval_map<Key, Value>{'A'};
  im.assign(7, 8, 'B');
  im.assign(4, 6, 'A');
  test_map(im.m_map, {{7, 'B'}, {8, 'A'}});
  test_range(im, {'A', 'A', 'A', 'A', 'A', 'A', 'A', 'B', 'A', 'A'});
}

TEST(test, invalid_range_default_value_after) {
  auto im = interval_map<Key, Value>{'A'};
  im.assign(4, 6, 'B');
  im.assign(7, 8, 'A');
  test_map(im.m_map, {{4, 'B'}, {6, 'A'}});
  test_range(im, {'A', 'A', 'A', 'A', 'B', 'B', 'A', 'A', 'A', 'A'});
}

TEST(test, valid_range_overwrite_with_default_before) {
  auto im = interval_map<Key, Value>{'A'};
  im.assign(4, 8, 'B');
  im.assign(2, 6, 'A');
  test_map(im.m_map, {{6, 'B'}, {8, 'A'}});
  test_range(im, {'A', 'A', 'A', 'A', 'A', 'A', 'B', 'B', 'A', 'A'});
}

TEST(test, one_range_after_another) {
  auto im = interval_map<Key, Value>{'A'};
  im.assign(4, 6, 'B');
  im.assign(7, 8, 'C');
  test_map(im.m_map, {{4, 'B'}, {6, 'A'}, {7, 'C'}, {8, 'A'}});
  test_range(im, {'A', 'A', 'A', 'A', 'B', 'B', 'A', 'C', 'A', 'A'});
}

TEST(test, multiple_ranges_concat_back) {
  auto im = interval_map<Key, Value>{'A'};
  im.assign(2, 4, 'B');
  im.assign(4, 6, 'B');
  im.assign(6, 8, 'B');
  test_map(im.m_map, {{2, 'B'}, {8, 'A'}});
  test_range(im, {'A', 'A', 'B', 'B', 'B', 'B', 'B', 'B', 'A', 'A'});
}

TEST(test, multiple_ranges_overwrite_concat) {
  auto im = interval_map<Key, Value>{'A'};
  im.assign(2, 4, 'B');
  im.assign(4, 6, 'C');
  im.assign(6, 8, 'B');
  im.assign(4, 6, 'B');
  test_map(im.m_map, {{2, 'B'}, {8, 'A'}});
  test_range(im, {'A', 'A', 'B', 'B', 'B', 'B', 'B', 'B', 'A', 'A'});
}

TEST(test, multiple_ranges_concat_front) {
  auto im = interval_map<Key, Value>{'A'};
  im.assign(4, 6, 'B');
  im.assign(2, 4, 'B');
  test_map(im.m_map, {{2, 'B'}, {6, 'A'}});
  test_range(im, {'A', 'A', 'B', 'B', 'B', 'B', 'A', 'A', 'A', 'A'});
}

TEST(test, multiple_ranges_concat_intersect) {
  auto im = interval_map<Key, Value>{'A'};
  im.assign(2, 5, 'B');
  im.assign(4, 6, 'B');
  test_map(im.m_map, {{2, 'B'}, {6, 'A'}});
  test_range(im, {'A', 'A', 'B', 'B', 'B', 'B', 'A', 'A', 'A', 'A'});
}

TEST(test, multiple_ranges_concat_intersect_front) {
  auto im = interval_map<Key, Value>{'A'};
  im.assign(4, 6, 'B');
  im.assign(2, 5, 'B');
  test_map(im.m_map, {{2, 'B'}, {6, 'A'}});
  test_range(im, {'A', 'A', 'B', 'B', 'B', 'B', 'A', 'A', 'A', 'A'});
}

TEST(test, multiple_ranges_concat_inside) {
  auto im = interval_map<Key, Value>{'A'};
  im.assign(2, 8, 'B');
  im.assign(5, 7, 'B');
  test_map(im.m_map, {{2, 'B'}, {8, 'A'}});
  test_range(im, {'A', 'A', 'B', 'B', 'B', 'B', 'B', 'B', 'A', 'A'});
}

TEST(test, one_range_right_after_another) {
  auto im = interval_map<Key, Value>{'A'};
  im.assign(2, 6, 'B');
  im.assign(6, 8, 'C');
  test_map(im.m_map, {{2, 'B'}, {6, 'C'}, {8, 'A'}});
  test_range(im, {'A', 'A', 'B', 'B', 'B', 'B', 'C', 'C', 'A', 'A'});
}

TEST(test, one_range_replacing_another) {
  auto im = interval_map<Key, Value>{'A'};
  im.assign(2, 6, 'B');
  im.assign(2, 6, 'C');
  test_map(im.m_map, {{2, 'C'}, {6, 'A'}});
  test_range(im, {'A', 'A', 'C', 'C', 'C', 'C', 'A', 'A', 'A', 'A'});
}

TEST(test, range_fully_inside_other_range) {
  auto im = interval_map<Key, Value>{'A'};
  im.assign(2, 8, 'B');
  im.assign(4, 6, 'C');
  test_map(im.m_map, {{2, 'B'}, {4, 'C'}, {6, 'B'}, {8, 'A'}});
  test_range(im, {'A', 'A', 'B', 'B', 'C', 'C', 'B', 'B', 'A', 'A'});
}

TEST(test, range_fully_over_2_other_range) {
  auto im = interval_map<Key, Value>{'A'};
  im.assign(2, 3, 'B');
  im.assign(4, 5, 'C');
  im.assign(1, 9, 'D');
  test_map(im.m_map, {{1, 'D'}, {9, 'A'}});
  test_range(im, {'A', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'A'});
}

TEST(test, range_in_the_beggining_of_other_range) {
  auto im = interval_map<Key, Value>{'A'};
  im.assign(2, 8, 'B');
  im.assign(2, 6, 'C');
  test_map(im.m_map, {{2, 'C'}, {6, 'B'}, {8, 'A'}});
  test_range(im, {'A', 'A', 'C', 'C', 'C', 'C', 'B', 'B', 'A', 'A'});
}

TEST(test, range_in_the_end_of_other_range) {
  auto im = interval_map<Key, Value>{'A'};
  im.assign(2, 4, 'B');
  im.assign(4, 6, 'C');
  test_map(im.m_map, {{2, 'B'}, {4, 'C'}, {6, 'A'}});
  test_range(im, {'A', 'A', 'B', 'B', 'C', 'C', 'A', 'A', 'A', 'A'});
}

TEST(test, range_intersects_begining_of_other_range) {
  auto im = interval_map<Key, Value>{'A'};
  im.assign(4, 6, 'B');
  im.assign(2, 5, 'C');
  test_map(im.m_map, {{2, 'C'}, {5, 'B'}, {6, 'A'}});
  test_range(im, {'A', 'A', 'C', 'C', 'C', 'B', 'A', 'A', 'A', 'A'});
}

TEST(test, range_intersects_ending_of_other_range) {
  auto im = interval_map<Key, Value>{'A'};
  im.assign(2, 6, 'B');
  im.assign(3, 8, 'C');
  test_map(im.m_map, {{2, 'B'}, {3, 'C'}, {8, 'A'}});
  test_range(im, {'A', 'A', 'B', 'C', 'C', 'C', 'C', 'C', 'A', 'A'});
}

TEST(test, range_intersects_2_ranges) {
  auto im = interval_map<Key, Value>{'A'};
  im.assign(2, 5, 'B');
  im.assign(7, 9, 'C');
  im.assign(3, 9, 'D');
  test_map(im.m_map, {{2, 'B'}, {3, 'D'}, {9, 'A'}});
  test_range(im, {'A', 'A', 'B', 'D', 'D', 'D', 'D', 'D', 'D', 'A'});
}

// TEST(test, set_range_to_default_in_range) {
//   auto im = interval_map<Key, Value>{'A'};
//   im.assign(0, 9, 'B');
//   im.assign(4, 7, 'D');
//   im.assign(1, 7, 'A');
//   test_map(im.m_map, {{0, 'B'}, {1, 'A'}, {7, 'B'}, {9, 'A'}});
//   test_range(im, {'B', 'A', 'A', 'A', 'A', 'A', 'A', 'B', 'B', 'A'});
// }

// TEST(test, set_range_to_default) {
//   auto im = interval_map<Key, Value>{'A'};
//   im.assign(4, 7, 'D');
//   im.assign(1, 7, 'A');
//   test_map(im.m_map, {});
//   test_range(im, {'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A'});
// }

TEST(test, random) {
  auto im = interval_map<Key, Value>{'A'};
  auto interval = std::array{'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A'};
  auto chars = std::array{'A', 'B', 'C', 'D', 'E', 'F', 'G'};
  for (auto i{0}; i < 10; i++) {
    auto begin = rand() % interval.size();
    auto end = rand() % interval.size();
    auto val = rand() % chars.size();
    im.assign(begin, end, chars[val]);
    for (auto j{begin}; j < end; j++)
      interval[j] = chars[val];
    test_range(im, interval);
  }
}
