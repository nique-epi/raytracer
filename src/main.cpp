#include <iostream>
#include <string_view>

namespace {
constexpr std::string_view usage =
    "USAGE: ./raytracer <SCENE_FILE>\n"
    "SCENE_FILE: scene configuration\n";
constexpr int error_exit_code = 84;
}  // namespace

int main(int argc, char** argv) {
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
