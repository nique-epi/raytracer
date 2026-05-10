/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SceneBuilder
*/

#pragma once
#include <functional>
#include <libconfig.h++>
#include <map>
#include <memory>
#include <string>
#include "registry/registry.hpp"

namespace raytracer::scene {

class Scene;

/**
 * @brief Assembles a @ref Scene from parsed configuration entries.
 *
 * Scene loaders call @ref addObject, @ref addLight and @ref addCamera for
 * each entity they parse. The builder resolves each type name through the
 * matching Registry and accumulates the resulting components into an
 * internal Scene instance. Call @ref build() to retrieve the finished scene
 * once all entries have been registered.
 */
class SceneBuilder {
 public:
  /**
   * @brief Construct a builder backed by the four component registries.
   *
   * @param [in] objects   Registry used to instantiate primitive objects.
   * @param [in] lights    Registry used to instantiate light sources.
   * @param [in] cameras   Registry used to instantiate cameras.
   * @param [in] materials Registry used to instantiate materials.
   */
  SceneBuilder(raytracer::core::registry::ObjectRegistry& objects,
               raytracer::core::registry::LightRegistry& lights,
               raytracer::core::registry::CameraRegistry& cameras,
               raytracer::core::registry::MaterialRegistry& materials);

  /**
   * @brief Resolve @p type in the object registry and add it to the scene.
   *
   * @param [in] type Registry key identifying the primitive type.
   * @param [in] cfg  Configuration block passed to the factory.
   *
   * @throws raytracer::core::RaytracerException If @p type is not registered.
   */
  void addObject(const std::string& type, const libconfig::Setting& cfg);

  /**
   * @brief Resolve @p type in the light registry and add it to the scene.
   *
   * @param [in] type Registry key identifying the light type.
   * @param [in] cfg  Configuration block passed to the factory.
   *
   * @throws raytracer::core::RaytracerException If @p type is not registered.
   */
  void addLight(const std::string& type, const libconfig::Setting& cfg);

  /**
   * @brief Resolve the camera type in the camera registry and set it on the scene.
   *
   * The type is read from @p cfg under the key @c "type"; it defaults to
   * @c "perspective" when the key is absent.
   *
   * @param [in] cfg Configuration block passed to the factory.
   *
   * @throws raytracer::core::RaytracerException If the resolved type is not registered.
   */
  void addCamera(const libconfig::Setting& cfg);

  /**
   * @brief Finalise and return the assembled scene.
   *
   * @returns A shared pointer to the constructed @ref Scene.
   *
   * @throws raytracer::core::RaytracerException If no camera or no light
   *         source has been registered.
   */
  std::shared_ptr<Scene> build();

  /**
   * @brief Count how many times @p type has been attempted via add calls.
   *
   * The counter is incremented before registry resolution, so it reflects
   * the number of parser-level attempts regardless of success or failure.
   *
   * @param [in] type Type discriminant to look up.
   * @returns Number of recorded attempts for @p type, or 0 if none.
   */
  [[nodiscard]] std::size_t count(const std::string& type) const;

 private:
  std::shared_ptr<Scene> scene_;
  std::reference_wrapper<raytracer::core::registry::ObjectRegistry>
      objectRegistry_;
  std::reference_wrapper<raytracer::core::registry::LightRegistry>
      lightRegistry_;
  std::reference_wrapper<raytracer::core::registry::CameraRegistry>
      cameraRegistry_;
  [[maybe_unused]] std::reference_wrapper<
      raytracer::core::registry::MaterialRegistry>
      materialRegistry_;
  std::map<std::string, std::size_t> typeCounts_;
};

}  // namespace raytracer::scene
