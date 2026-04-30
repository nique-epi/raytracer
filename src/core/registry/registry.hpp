/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** registry
*/

#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include "components/Primitive/IObject.hpp"

class ILight;
class IMaterial;
class ITransform;

namespace libconfig {
class Setting;
}  // namespace libconfig

namespace raytracer::core::registry {

template <typename T>
class Registry {
 public:
  using CreatorFn =
      std::function<std::shared_ptr<T>(const libconfig::Setting&)>;

  void registerType(const std::string& name, CreatorFn fn);

  std::shared_ptr<T> create(const std::string& type,
                            const libconfig::Setting& cfg) const;

  void loadPlugin(const std::string& path);

 private:
  std::map<std::string, CreatorFn> creators;
};

using ObjectRegistry = Registry<IObject>;
using LightRegistry = Registry<ILight>;
using MaterialRegistry = Registry<IMaterial>;
using TransformRegistry = Registry<ITransform>;

}  // namespace raytracer::core::registry
