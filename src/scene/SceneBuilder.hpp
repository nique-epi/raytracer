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
#include "core/registry/registry.hpp"

namespace raytracer::scene {

class Scene;

class SceneBuilder {
 public:
  SceneBuilder(raytracer::core::registry::ObjectRegistry& objects,
               raytracer::core::registry::LightRegistry& lights,
               raytracer::core::registry::CameraRegistry& cameras,
               raytracer::core::registry::MaterialRegistry& materials);

  void addObject(const std::string& type, const libconfig::Setting& cfg);
  void addLight(const std::string& type, const libconfig::Setting& cfg);
  void addCamera(const libconfig::Setting& cfg);

  std::shared_ptr<Scene> build();

  [[nodiscard]] std::size_t count(const std::string& type) const;

 private:
  std::shared_ptr<Scene> _scene;
  std::reference_wrapper<raytracer::core::registry::ObjectRegistry>
      _objectRegistry;
  std::reference_wrapper<raytracer::core::registry::LightRegistry>
      _lightRegistry;
  std::reference_wrapper<raytracer::core::registry::CameraRegistry>
      _cameraRegistry;
  [[maybe_unused]] std::reference_wrapper<
      raytracer::core::registry::MaterialRegistry>
      _materialRegistry;
  std::map<std::string, std::size_t> _typeCounts;
};

}  // namespace raytracer::scene
