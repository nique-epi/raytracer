/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** registry
*/

#include "registry.hpp"
#include <dlfcn.h>
#include "plugin/PluginLoader.hpp"

namespace raytracer::core::registry {

template <typename T>
void Registry<T>::registerType(const std::string& name, CreatorFn fn) {
  creators[name] = std::move(fn);
}

template <typename T>
std::shared_ptr<T> Registry<T>::create(const std::string& type,
                                       const libconfig::Setting& cfg) const {
  auto it = creators.find(type);
  return it->second(cfg);
}

template <typename T>
void Registry<T>::loadPlugin(const std::string& path) {
  raytracer::plugin::PluginLoader loader;
  void* handle = loader.load(path);
  if (handle == nullptr) {
    return;
  }
  using RegisterFn = void (*)(Registry<T>&);
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  auto registerFn = reinterpret_cast<RegisterFn>(
      raytracer::plugin::PluginLoader::getSymbol(handle, "registerTypes"));
  if (registerFn) {
    registerFn(*this);
  }
}

template class Registry<IObject>;
template class Registry<ILight>;
template class Registry<IMaterial>;
template class Registry<ITransform>;

}  // namespace raytracer::core::registry
