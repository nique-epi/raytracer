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
#include "components/Primitives/IObject.hpp"
#include "plugin/PluginLoader.hpp"

class ILight;
class IMaterial;
class ITransformation;
class ICamera;

namespace libconfig {
class Setting;
}  // namespace libconfig

/**
 * @brief Registry utilities for dynamically created components.
 */
namespace raytracer::core::registry {

template <typename T>
struct PluginSymbolTraits;

/**
 * @brief Map primitive registry types to plugin entry points.
 */
template <>
struct PluginSymbolTraits<IObject> {
  static constexpr const char* createSymbol = "createPrimitive";
  static constexpr const char* destroySymbol = "destroyPrimitive";
};

/**
 * @brief Map light registry types to plugin entry points.
 */
template <>
struct PluginSymbolTraits<ILight> {
  static constexpr const char* createSymbol = "createLights";
  static constexpr const char* destroySymbol = "destroyLights";
};

/**
 * @brief Map material registry types to plugin entry points.
 */
template <>
struct PluginSymbolTraits<IMaterial> {
  static constexpr const char* createSymbol = "createMaterial";
  static constexpr const char* destroySymbol = "destroyMaterial";
};

/**
 * @brief Map transformation registry types to plugin entry points.
 */
template <>
struct PluginSymbolTraits<ITransformation> {
  static constexpr const char* createSymbol = "createTransformations";
  static constexpr const char* destroySymbol = "destroyTransformations";
};

/**
 * @brief Map camera registry types to plugin entry points.
 */
template <>
struct PluginSymbolTraits<ICamera> {
  static constexpr const char* createSymbol = "createCamera";
  static constexpr const char* destroySymbol = "destroyCamera";
};

/**
 * @brief Register and construct dynamically loaded component types.
 *
 * @tparam T Interface type managed by the registry.
 */
template <typename T>
class Registry {
 public:
  /**
   * @brief Creator function signature used by the registry.
   */
  using CreatorFn =
      std::function<std::shared_ptr<T>(const libconfig::Setting&)>;

  /**
   * @brief Register a named creator function.
   *
   * @param [in] name Registry key used to create the type.
   * @param [in] fn   Factory function associated with @p name.
   *
   * @throws raytracer::core::RaytracerException If @p name is already
   *         registered or if @p fn is empty.
   */
  void registerType(const std::string& name, CreatorFn fn);

  /**
   * @brief Create an instance from a registered type name.
   *
   * @param [in] type Registry key to resolve.
   * @param [in] cfg  Configuration used by the creator function.
   * @returns A shared pointer to the constructed component.
   *
   * @throws raytracer::core::RaytracerException If @p type is not registered.
   */
  std::shared_ptr<T> create(const std::string& type,
                            const libconfig::Setting& cfg) const;

  /**
   * @brief Load a plugin and register its exported creators.
   *
   * @param [in] path Path to the shared library to load.
   *
   * @note If the plugin cannot be loaded, this function returns without
   *       registering anything.
   */
  void loadPlugin(const std::string& path);

 private:
  std::map<std::string, CreatorFn> creators;
  raytracer::plugin::PluginLoader pluginLoader;
};

/**
 * @brief Registry of primitive component factories.
 */
using ObjectRegistry = Registry<IObject>;
/**
 * @brief Registry of light component factories.
 */
using LightRegistry = Registry<ILight>;
/**
 * @brief Registry of material component factories.
 */
using MaterialRegistry = Registry<IMaterial>;
/**
 * @brief Registry of transformation component factories.
 */
using TransformRegistry = Registry<ITransformation>;
/**
 * @brief Registry of camera component factories.
 */
using CameraRegistry = Registry<ICamera>;

}  // namespace raytracer::core::registry
