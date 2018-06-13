#pragma once

#include <string_view>

namespace mm {

std::string_view left_trim(std::string_view str);

std::pair<bool, std::string_view> starts_with(std::string_view str, std::string_view prefix);

}
