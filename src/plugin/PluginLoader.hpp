/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PluginLoader
*/

#pragma once

#include <map>
#include <string>

namespace raytracer::plugin {

class PluginLoader {
 public:
  using HandleMap = std::map<std::string, void*>;

  PluginLoader() = default;
  PluginLoader(const PluginLoader&) = delete;
  PluginLoader& operator=(const PluginLoader&) = delete;
  PluginLoader(PluginLoader&&) = delete;
  PluginLoader& operator=(PluginLoader&&) = delete;
  ~PluginLoader();

  void* load(const std::string& path);
  static void* getSymbol(void* handle, const std::string& name);
  void unloadAll();

  [[nodiscard]] const HandleMap& getHandles() const noexcept;

 private:
  HandleMap handles;
};

}  // namespace raytracer::plugin
