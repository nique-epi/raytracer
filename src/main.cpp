#include <exception>
#include <filesystem>
#include <iostream>
#include <string_view>
#include "core/Exceptions.hpp"
#include "output/ppm/ppm.hpp"
#include "plugin/PluginLoader.hpp"

namespace {
constexpr std::string_view usage =
    "USAGE: ./raytracer <SCENE_FILE>\n"
    "SCENE_FILE: scene configuration\n";
constexpr int error_exit_code = 84;

int checkArguments(int argc, char** argv) {
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

std::map<std::string, void*> loadPlugins() {
  raytracer::plugin::PluginLoader loader;

  for (const std::filesystem::directory_entry& pluginEntry :
       std::filesystem::directory_iterator("./plugins")) {
    const std::filesystem::path& pluginPath = pluginEntry.path();
    if (pluginEntry.is_regular_file() && pluginPath.extension() == ".so") {
      loader.load(pluginPath.string());
    }
  }
  return loader.getHandles();
}

int run(int argc, char** argv) {
  if (checkArguments(argc, argv) != 0) {
    return error_exit_code;
  }
  const std::map<std::string, void*> pluginHandles = loadPlugins();
  // scene loader call
  // raytracer renderer call
  raytracer::components::Image finalImage =
      /* raytracer::renderer::render(scene) */;
  raytracer::output::ppm::write(finalImage, "output.ppm");
  return 0;
}
}  // namespace

int main(int argc, char** argv) {
  try {
    return run(argc, argv);
  } catch (const raytracer::core::RaytracerException& e) {
    std::cerr << "Error: " << e.what() << '\n';
    return error_exit_code;
  } catch (const std::exception& e) {
    std::cerr << "Unhandled exception: " << e.what() << '\n';
    return error_exit_code;
  }
}
