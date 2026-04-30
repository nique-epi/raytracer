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
class ITransformation;
class ICamera;

namespace libconfig {
class Setting;
}  // namespace libconfig

namespace raytracer::core::registry {

template <typename T>
struct PluginSymbolTraits;

template <>
struct PluginSymbolTraits<IObject> {
  static constexpr const char* createSymbol = "createPrimitive";
  static constexpr const char* destroySymbol = "destroyPrimitive";
};

template <>
struct PluginSymbolTraits<ILight> {
  static constexpr const char* createSymbol = "createLights";
  static constexpr const char* destroySymbol = "destroyLights";
};

template <>
struct PluginSymbolTraits<IMaterial> {
  static constexpr const char* createSymbol = "createMaterial";
  static constexpr const char* destroySymbol = "destroyMaterial";
};

template <>
struct PluginSymbolTraits<ITransformation> {
  static constexpr const char* createSymbol = "createTransformations";
  static constexpr const char* destroySymbol = "destroyTransformations";
};

template <>
struct PluginSymbolTraits<ICamera> {
  static constexpr const char* createSymbol = "createCamera";
  static constexpr const char* destroySymbol = "destroyCamera";
};

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
using TransformRegistry = Registry<ITransformation>;
using CameraRegistry = Registry<ICamera>;

}  // namespace raytracer::core::registry
