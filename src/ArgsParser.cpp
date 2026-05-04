/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ArgsParser
*/

#include "ArgsParser.hpp"
#include <string_view>

namespace raytracer {

std::optional<AppConfig> ArgsParser::parse(int argc, const char** argv) {
  if (argc == 2 && (std::string_view(argv[1]) == "-h" ||
                    std::string_view(argv[1]) == "--help")) {
    return HelpRequest{};
  }
  if (argc != 2) {
    return std::nullopt;
  }
  return SceneRequest{argv[1]};
}

}  // namespace raytracer
