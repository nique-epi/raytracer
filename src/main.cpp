/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** main
*/

#include <exception>
#include <iostream>
#include <string_view>
#include <variant>
#include "Application.hpp"
#include "ArgsParser.hpp"
#include "core/Exceptions.hpp"

namespace {
constexpr std::string_view usage =
    "USAGE: ./raytracer <SCENE_FILE>\n"
    "SCENE_FILE: scene configuration\n";
constexpr int error_exit_code = 84;
}  // namespace

int main(int argc, char** argv) {
  try {
    const auto config = raytracer::ArgsParser::parse(argc, argv);
    if (!config) {
      std::cerr << usage;
      return error_exit_code;
    }
    if (std::holds_alternative<raytracer::HelpRequest>(*config)) {
      std::cout << usage;
      return 0;
    }
    return raytracer::Application{}.run(
        std::get<raytracer::SceneRequest>(*config).scenePath);
  } catch (const raytracer::core::RaytracerException& e) {
    std::cerr << "Error: " << e.what() << '\n';
    return error_exit_code;
  } catch (const std::exception& e) {
    std::cerr << "Unhandled exception: " << e.what() << '\n';
    return error_exit_code;
  }
}
