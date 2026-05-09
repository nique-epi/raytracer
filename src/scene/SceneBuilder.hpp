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
