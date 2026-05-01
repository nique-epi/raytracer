#include <cassert>
#include <filesystem>
#include "plugin/PluginLoader.hpp"

int main() {
  raytracer::plugin::PluginLoader loader;

  const std::filesystem::path pluginPath =
      std::filesystem::current_path() / "fake_plugin.so";

  [[maybe_unused]] void* handle = loader.load(pluginPath.string());
  assert(handle != nullptr);

  [[maybe_unused]] void* symbol =
      raytracer::plugin::PluginLoader::getSymbol(handle, "fake_plugin_value");
  assert(symbol != nullptr);
  assert(*static_cast<int*>(symbol) == 42);

  loader.unloadAll();
  assert(loader.getHandles().empty());

  return 0;
}
