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

}
