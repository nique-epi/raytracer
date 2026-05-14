/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ArgsParser
*/

#pragma once
#include <optional>
#include <string>
#include <variant>

namespace raytracer::core {

struct HelpRequest {};

struct SceneRequest {
  std::string scenePath;
  bool useBVH{false};
};

using AppConfig = std::variant<HelpRequest, SceneRequest>;

class ArgsParser {
 public:
  [[nodiscard]] static std::optional<AppConfig> parse(int argc,
                                                      const char** argv);
};

}  // namespace raytracer::core
