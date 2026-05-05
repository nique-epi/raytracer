/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** main
*/

#include <exception>
#include <filesystem>
#include <iostream>
#include <string_view>
#include <variant>
#include "core/Application.hpp"
#include "core/ArgsParser.hpp"
#include "core/Exceptions.hpp"
#include "output/ppm/ppm.hpp"
#include "plugin/PluginLoader.hpp"

namespace {
constexpr std::string_view usage =
    "USAGE: ./raytracer <SCENE_FILE>\n"
    "SCENE_FILE: scene configuration\n";
constexpr int error_exit_code = 84;
}  // namespace

int main(int argc, char** argv) {
  try {
    const auto config = raytracer::core::ArgsParser::parse(
        argc, const_cast<const char**>(argv));
    if (!config) {
      throw raytracer::core::RaytracerException(std::string(usage));
    }
    if (std::holds_alternative<raytracer::core::HelpRequest>(*config)) {
      std::cout << usage;
      return 0;
    }
    return raytracer::core::Application{}.run(
        std::get<raytracer::core::SceneRequest>(*config).scenePath);
  } catch (const raytracer::core::RaytracerException& e) {
    std::cerr << "Error: " << e.what() << '\n';
    return error_exit_code;
  } catch (const std::exception& e) {
    std::cerr << "Unhandled exception: " << e.what() << '\n';
    return error_exit_code;
  }
}
