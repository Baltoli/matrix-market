#pragma once

#include <iostream>
#include <iterator>
#include <string_view>

namespace mm {

std::string_view left_trim(std::string_view str);

std::pair<bool, std::string_view> starts_with(std::string_view str, std::string_view prefix);

template <typename F>
void for_each_line(std::string_view str, F&& f)
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
  using value_type = std::string_view;
  using pointer = std::string_view*;
  using reference = std::string_view&;
  using category = std::input_iterator_tag;

  split_iterator(char d_, std::string_view, std::string_view::size_type);

  value_type operator*() const;
  bool operator==(split_iterator const& other) const;
  bool operator!=(split_iterator const& other) const;
  split_iterator& operator++();
  split_iterator operator++(int);
  proxy<std::string_view> operator->() const;

private:
  void advance();

  char delim_;
  std::string_view::size_type line_begin_;
  std::string_view::size_type line_end_;
  std::string_view data_;
};

class lines {
public:
  lines(std::string_view str) : str_(str) {}

  split_iterator begin() const { return split_iterator('\n', str_, 0); }
  split_iterator end() const { return split_iterator('\n', str_, std::string_view::npos); }

private:
  std::string_view str_;
};

class columns {
public:
  columns(std::string_view str) : str_(str) {}

  split_iterator begin() const { return split_iterator(' ', str_, 0); }
  split_iterator end() const { return split_iterator(' ', str_, std::string_view::npos); }

private:
  std::string_view str_;
};

}
