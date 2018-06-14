#pragma once

#include <iostream>
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

}
