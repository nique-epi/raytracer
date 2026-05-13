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
#include "application/Application.hpp"
#include "application/ArgsParser.hpp"
#include "exceptions/Exceptions.hpp"

namespace {
constexpr std::string_view usage =
    "USAGE: ./raytracer <SCENE_FILE> [--no-bvh]\n"
    "SCENE_FILE: scene configuration\n"
    "--no-bvh:    disable BVH acceleration structure\n";
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
    const auto& req = std::get<raytracer::core::SceneRequest>(*config);
    return raytracer::core::Application{}.run(req.scenePath, req.useBVH);
  } catch (const raytracer::core::RaytracerException& e) {
    std::cerr << "Error: " << e.what() << '\n';
    return error_exit_code;
  } catch (const std::exception& e) {
    std::cerr << "Unhandled exception: " << e.what() << '\n';
    return error_exit_code;
  }
}
