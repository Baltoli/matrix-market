#include "utils.h"

#include <string>

namespace mm {

std::string left_trim(std::string const& str)
{
  auto i = 0u;
  while(i < str.size() && std::isspace(str.at(i))) { ++i; }

  return str.substr(i);
}

std::pair<bool, std::string> starts_with(std::string const& str, std::string const& prefix)
{
  if(prefix.size() > str.size()) {
    return { false, str };
  }

  auto eq = std::equal(prefix.begin(), prefix.end(), str.begin());
  return { eq, eq ? left_trim(str.substr(prefix.length())) : str };
}

split_iterator::split_iterator(char d, std::string s, std::string::size_type begin)
  : delim_(d), data_(s), line_begin_(begin), line_end_(begin)
{
  advance();
}

split_iterator::value_type split_iterator::operator*() const
{
  if(line_begin_ >= data_.size()) { return ""; }
  return data_.substr(line_begin_, line_end_ - line_begin_);
}

bool split_iterator::operator==(split_iterator const& other) const
{
  return (line_begin_ >= data_.size() && other.line_begin_ >= data_.size()) ||
         (line_begin_ == other.line_begin_ && line_end_ == other.line_end_);
}

bool split_iterator::operator!=(split_iterator const& other) const
{
  return !(*this == other);
}

split_iterator& split_iterator::operator++()
{
  while(line_begin_ < data_.size() && data_.at(line_begin_) != delim_) {
    ++line_begin_;
  }
  advance();
  return *this;
}

split_iterator split_iterator::operator++(int)
{
  auto ret = *this;
  this->operator++();
  return ret;
}

proxy<std::string> split_iterator::operator->() const
{
  return proxy<std::string>(**this);
}

void split_iterator::advance()
{
  while(line_begin_ < data_.size() && data_.at(line_begin_) == delim_) {
    ++line_begin_;
  }

  line_end_ = data_.find(delim_, line_begin_);
}

}
