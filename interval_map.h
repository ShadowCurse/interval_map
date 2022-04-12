#ifndef INTERVAL_MAP_H_
#define INTERVAL_MAP_H_ 

#include <ios>
#include <iostream>
#include <map>

class Key {
  int a;
 public:
  Key(int a) : a{a} {}
  bool operator<(const Key &other) const { return a < other.a; }
  friend std::ostream &operator<<(std::ostream &os, const Key &key) {
      os << key.a;
      return os;
  }
};

class Value {
  char v;
 public:
  Value(char v) : v{v} {}
  bool operator==(const Value &other) const { return v == other.v; }
  friend std::ostream &operator<<(std::ostream &os, const Value &val) {
      os << val.v;
      return os;
  }
};

template<typename K, typename V>
class interval_map {
 public:
  V m_valBegin;
  std::map<K, V> m_map;

  explicit interval_map(V const &val)
      : m_valBegin{val} {}

  void assign(K const &keyBegin, K const &keyEnd, V const &val) {
      if (!(keyBegin < keyEnd))
          return;

      const auto is_default_value{val == m_valBegin};

      using c_iter = typename std::map<K, V>::const_iterator;
      c_iter upper{};

      const auto upper_bound = m_map.upper_bound(keyEnd);
      // if trying to insert range in front(also true when map is empty)
      if (upper_bound == std::cbegin(m_map)) { 
          // must be no ranges with default value in front
          if (is_default_value) return; 
          upper = m_map.insert({keyEnd, m_valBegin}).first;
      } else {
          const auto prev = std::prev(upper_bound);
          // checks if next range has the same value
          if (prev->second == val)  
              upper = upper_bound;
          else {
              upper = m_map.insert({keyEnd, prev->second}).first;
          }
      }

      const auto lower = m_map.lower_bound(keyBegin);
      m_map.erase(lower, upper);

      const auto new_lower = m_map.lower_bound(keyBegin);
      const auto last{new_lower == std::begin(m_map)};
      const auto eq_prev{std::prev(new_lower)->second == val};
      if (last && !is_default_value || !last && !eq_prev)
          m_map.insert({keyBegin, val});
  }

  V const &operator[](K const &key) const {
      auto it = m_map.upper_bound(key);
      if (it == m_map.begin()) {
          return m_valBegin;
      } else {
          return (--it)->second;
      }
  }
};

#endif 
