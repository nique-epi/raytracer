#include <exception>
#include <iostream>
#include <string_view>
#include "core/Exceptions.hpp"
namespace {
constexpr std::string_view usage =
    "USAGE: ./raytracer <SCENE_FILE>\n"
    "SCENE_FILE: scene configuration\n";
constexpr int error_exit_code = 84;

int run(int argc, char** argv) {
  if (argc == 2 && (std::string_view(argv[1]) == "-h" ||
                    std::string_view(argv[1]) == "--help")) {
    std::cout << usage;
    return 0;
  }

  if (argc != 2) {
    std::cerr << usage;
    return error_exit_code;
  }

  return 0;
}
}  // namespace

int main(int argc, char** argv) {
  try {
    return run(argc, argv);
  } catch (const Core::RaytracerException& e) {
    std::cerr << "Error: " << e.what() << '\n';
    return error_exit_code;
  } catch (const std::exception& e) {
    std::cerr << "Unhandled exception: " << e.what() << '\n';
    return error_exit_code;
  }
}
