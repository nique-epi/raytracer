#include <cassert>
#include <filesystem>
#include "plugin/PluginLoader.hpp"

int main() {
  raytracer::plugin::PluginLoader loader;

  const std::filesystem::path pluginPath =
      std::filesystem::current_path() / "fake_plugin.so";

  void* handle = loader.load(pluginPath.string());
  assert(handle != nullptr);

  auto* symbol =
      raytracer::plugin::PluginLoader::getSymbol(handle, "fake_plugin_value");
  assert(symbol != nullptr);

  auto* value = static_cast<int*>(symbol);
  assert(*value == 42);

  loader.unloadAll();
  assert(loader.getHandles().empty());

  return 0;
}
