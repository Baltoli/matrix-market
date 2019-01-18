#pragma once

#include <iostream>
#include <iterator>

namespace mm {

std::string left_trim(std::string const& str);

std::pair<bool, std::string> starts_with(std::string const& str, std::string const& prefix);

template <typename F>
void for_each_line(std::string const& str, F&& f)
{
  auto line_begin = 0;
  auto line_end = 0;

  do {
    line_end = str.find('\n', line_begin);
    if(line_end == std::string::npos) {
      break;
    }

    if(line_begin == line_end) {
      ++line_begin;
    } else {
      auto line = str.substr(line_begin, line_end - line_begin);
      std::forward<decltype(f)>(f)(line);
      line_begin = line_end + 1;
    }
  } while(true);
}

template <typename T>
class proxy {
public:
  proxy(T const& t) : t_(t) {}
  T* operator->() { return &t_; };

private:
  T t_;
};

class split_iterator {
public:
  using difference_type = void;
  using value_type = std::string;
  using pointer = std::string*;
  using reference = std::string&;
  using category = std::input_iterator_tag;

  split_iterator(char d_, std::string, std::string::size_type);

  value_type operator*() const;
  bool operator==(split_iterator const& other) const;
  bool operator!=(split_iterator const& other) const;
  split_iterator& operator++();
  split_iterator operator++(int);
  proxy<std::string> operator->() const;

private:
  void advance();

  char delim_;
  std::string data_;
  std::string::size_type line_begin_;
  std::string::size_type line_end_;
};

class lines {
public:
  lines(std::string str) : str_(str) {}

  split_iterator begin() const { return split_iterator('\n', str_, 0); }
  split_iterator end() const { return split_iterator('\n', str_, std::string::npos); }

private:
  std::string str_;
};

class columns {
public:
  columns(std::string str) : str_(str) {}

  split_iterator begin() const { return split_iterator(' ', str_, 0); }
  split_iterator end() const { return split_iterator(' ', str_, std::string::npos); }

private:
  std::string str_;
};

}
