#include "utils.h"

#include <string>

namespace mm {

std::string_view left_trim(std::string_view str)
{
  auto i = 0;
  while(i < str.size() && std::isspace(str.at(i))) { ++i; }

  return str.substr(i);
}

std::pair<bool, std::string_view> starts_with(std::string_view str, std::string_view prefix)
{
  if(prefix.size() > str.size()) {
    return { false, str };
  }

  auto eq = std::equal(prefix.begin(), prefix.end(), str.begin());
  return { eq, eq ? left_trim(str.substr(prefix.length())) : str };
}

line_iterator::line_iterator(std::string_view s, std::string_view::size_type begin)
  : data_(s), line_begin_(begin), line_end_(begin)
{
  advance();
}

line_iterator::value_type line_iterator::operator*() const
{
  if(line_begin_ >= data_.size()) { return ""; }
  return data_.substr(line_begin_, line_end_ - line_begin_);
}

bool line_iterator::operator==(line_iterator const& other) const
{
  return (line_begin_ >= data_.size() && other.line_begin_ >= data_.size()) ||
         (line_begin_ == other.line_begin_ && line_end_ == other.line_end_);
}

bool line_iterator::operator!=(line_iterator const& other) const
{
  return !(*this == other);
}

line_iterator& line_iterator::operator++()
{
  while(line_begin_ < data_.size() && data_.at(line_begin_) != '\n') {
    ++line_begin_;
  }
  advance();
  return *this;
}

line_iterator line_iterator::operator++(int)
{
  auto ret = *this;
  this->operator++();
  return ret;
}

proxy<std::string_view> line_iterator::operator->() const
{
  return proxy<std::string_view>(**this);
}

void line_iterator::advance()
{
  while(line_begin_ < data_.size() && data_.at(line_begin_) == '\n') {
    ++line_begin_;
  }

  line_end_ = data_.find('\n', line_begin_);
}

}
