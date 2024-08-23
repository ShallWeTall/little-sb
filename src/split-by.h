#pragma once

#include <ranges>
#include <string_view>
#include <vector>

inline auto split_by(std::string_view const str,
                     char delim = ' ') -> std::vector<std::string>
{
  std::vector<std::string> parts;
  for (auto const &part : str | std::views::split(delim)) {
    parts.emplace_back(std::string_view{part});
  }
  return parts;
}
