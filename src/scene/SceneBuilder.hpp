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
#include "World.hpp"
#include "background/IBackground.hpp"
#include "factory/IComponentFactory.hpp"

namespace raytracer::scene {

class Scene;

/**
 * @brief Assembles a @ref Scene from parsed configuration entries.
 *
 * Scene loaders call @ref addObject, @ref addLight and @ref addCamera for
 * each entity they parse. The builder resolves each type name through the
 * injected @ref IComponentFactory and accumulates the resulting components
 * into an internal Scene instance. Call @ref build() to retrieve the
 * finished scene once all entries have been registered.
 */
class SceneBuilder {
 public:
  /**
   * @brief Construct a builder backed by a component factory.
   *
   * @param [in] factory Factory used to instantiate every component type.
   */
  explicit SceneBuilder(raytracer::core::factory::IComponentFactory& factory);

  /**
   * @brief Resolve @p type via the factory and add the primitive to the scene.
   *
   * @param [in] type Type discriminant identifying the primitive.
   * @param [in] cfg  Configuration block passed to the factory.
   *
   * @throws raytracer::core::RaytracerException If @p type is not supported.
   */
  void addObject(const std::string& type, const libconfig::Setting& cfg);

  /**
   * @brief Resolve @p type via the factory and add the light to the scene.
   *
   * @param [in] type Type discriminant identifying the light.
   * @param [in] cfg  Configuration block passed to the factory.
   *
   * @throws raytracer::core::RaytracerException If @p type is not supported.
   */
  void addLight(const std::string& type, const libconfig::Setting& cfg);

  /**
   * @brief Resolve the camera type via the factory and set it on the scene.
   *
   * The type is read from @p cfg under the key @c "type"; it defaults to
   * @c "perspective" when the key is absent.
   *
   * @param [in] cfg Configuration block passed to the factory.
   *
   * @throws raytracer::core::RaytracerException If the resolved type is not supported.
   */
  void addCamera(const libconfig::Setting& cfg);

  /**
   * @brief Set the background on the scene.
   *
   * @param [in] background Background implementation to attach.
   */
  void setBackground(
      std::shared_ptr<raytracer::scene::background::IBackground> background);

  /**
   * @brief Set the viewport (shading) mode on the scene's World.
   *
   * @param [in] mode Shading strategy used by the renderer.
   */
  void setViewportMode(ViewportMode mode);

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
  std::reference_wrapper<raytracer::core::factory::IComponentFactory> factory_;
  std::map<std::string, std::size_t> typeCounts_;
};

}  // namespace raytracer::scene
