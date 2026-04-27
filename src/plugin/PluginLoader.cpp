/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PluginLoader
*/

#include "PluginLoader.hpp"
#include <dlfcn.h>
#include <iostream>

namespace raytracer::plugin {

PluginLoader::~PluginLoader() { unloadAll(); }

const PluginLoader::HandleMap& PluginLoader::getHandles() const noexcept {
  return handles;
}

void* PluginLoader::load(const std::string& path) {
  // NOLINTNEXTLINE(concurrency-mt-unsafe)
  (void)dlerror();
  void* handle = dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
  if (handle == nullptr) {
    // NOLINTNEXTLINE(concurrency-mt-unsafe)
    const char* error = dlerror();
    std::cerr << "PluginLoader: failed to load '" << path << "'";
    if (error != nullptr) {
      std::cerr << ": " << error;
    }
    std::cerr << '\n';
    return nullptr;
  }
  handles[path] = handle;
  return handle;
}

void* PluginLoader::getSymbol(void* handle, const std::string& name) {
  // NOLINTNEXTLINE(concurrency-mt-unsafe)
  (void)dlerror();
  void* symbol = dlsym(handle, name.c_str());
  // NOLINTNEXTLINE(concurrency-mt-unsafe)
  const char* error = dlerror();
  if (error != nullptr) {
    std::cerr << "PluginLoader: failed to resolve symbol '" << name
              << "': " << error << '\n';
    return nullptr;
  }
  return symbol;
}

void PluginLoader::unloadAll() {
  for (auto& [path, handle] : handles) {
    if (handle == nullptr) {
      continue;
    }
    if (dlclose(handle) != 0) {
      // NOLINTNEXTLINE(concurrency-mt-unsafe)
      (void)dlerror();
      // NOLINTNEXTLINE(concurrency-mt-unsafe)
      const char* error = dlerror();
      std::cerr << "PluginLoader: failed to unload '" << path << "'";
      if (error != nullptr) {
        std::cerr << ": " << error;
      }
      std::cerr << '\n';
    }
  }
  handles.clear();
}
}  // namespace raytracer::plugin
