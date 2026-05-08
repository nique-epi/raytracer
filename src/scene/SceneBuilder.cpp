/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SceneBuilder
*/

#include "SceneBuilder.hpp"
#include <iostream>
#include "core/Exceptions.hpp"
#include "scene/Scene.hpp"

namespace raytracer::scene {

SceneBuilder::SceneBuilder(
    raytracer::core::registry::ObjectRegistry& objects,
    raytracer::core::registry::LightRegistry& lights,
    raytracer::core::registry::CameraRegistry& cameras,
    raytracer::core::registry::MaterialRegistry& materials)
    : _scene(std::make_shared<Scene>()),
      _objectRegistry(&objects),
      _lightRegistry(&lights),
      _cameraRegistry(&cameras),
      _materialRegistry(&materials) {}

void SceneBuilder::addObject(const std::string& type,
                             const libconfig::Setting& cfg) {
  _typeCounts[type]++;
  try {
    auto obj = _objectRegistry->create(type, cfg);
    if (obj) {
      _scene->add(obj);
    }
  } catch (const raytracer::core::RaytracerException& e) {
    std::cerr << "[SceneBuilder] skipping object \"" << type
              << "\": " << e.what() << "\n";
  }
}

void SceneBuilder::addLight(const std::string& type,
                            const libconfig::Setting& cfg) {
  _typeCounts[type]++;
  try {
    auto light = _lightRegistry->create(type, cfg);
    if (light) {
      _scene->addLight(light);
    }
  } catch (const raytracer::core::RaytracerException& e) {
    std::cerr << "[SceneBuilder] skipping light \"" << type
              << "\": " << e.what() << "\n";
  }
}

void SceneBuilder::addCamera(const libconfig::Setting& cfg) {
  _typeCounts["camera"]++;
  std::string type = "perspective";
  cfg.lookupValue("type", type);
  try {
    auto camera = _cameraRegistry->create(type, cfg);
    if (camera) {
      _scene->setCamera(camera);
    }
  } catch (const raytracer::core::RaytracerException& e) {
    std::cerr << "[SceneBuilder] skipping camera \"" << type
              << "\": " << e.what() << "\n";
  }
}

std::shared_ptr<Scene> SceneBuilder::build() {
  if (!_scene->getCamera()) {
    throw raytracer::core::RaytracerException(
        "SceneBuilder::build: scene has no camera defined");
  }
  if (_scene->getLights().empty()) {
    throw raytracer::core::RaytracerException(
        "SceneBuilder::build: scene has no light sources defined");
  }
  return _scene;
}

std::size_t SceneBuilder::count(const std::string& type) const {
  const auto it = _typeCounts.find(type);
  return it != _typeCounts.end() ? it->second : 0;
}

}  // namespace raytracer::scene
