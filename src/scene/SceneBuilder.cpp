/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SceneBuilder
*/

#include "SceneBuilder.hpp"
#include "SceneConfigurator.hpp"
#include "core/Exceptions.hpp"
#include "scene/Scene.hpp"

namespace raytracer::scene {

SceneBuilder::SceneBuilder(
    raytracer::core::registry::ObjectRegistry& objects,
    raytracer::core::registry::LightRegistry& lights,
    raytracer::core::registry::CameraRegistry& cameras,
    raytracer::core::registry::MaterialRegistry& materials)
    : scene_(std::make_shared<Scene>()),
      objectRegistry_(objects),
      lightRegistry_(lights),
      cameraRegistry_(cameras),
      materialRegistry_(materials) {}

void SceneBuilder::addObject(const std::string& type,
                             const libconfig::Setting& cfg) {
  typeCounts_[type]++;
  try {
    auto obj = objectRegistry_.get().create(type, cfg);
    if (!obj) {
      return;
    }
    configureObject(type, obj, cfg);
    scene_->add(obj);
  } catch (const raytracer::core::RaytracerException& e) {
    throw raytracer::core::RaytracerException(e.what());
  }
}

void SceneBuilder::addLight(const std::string& type,
                            const libconfig::Setting& cfg) {
  typeCounts_[type]++;
  try {
    auto light = lightRegistry_.get().create(type, cfg);
    if (!light) {
      return;
    }
    configureLight(type, light, cfg);
    scene_->addLight(light);
  } catch (const raytracer::core::RaytracerException& e) {
    throw raytracer::core::RaytracerException(e.what());
  }
}

void SceneBuilder::addCamera(const libconfig::Setting& cfg) {
  typeCounts_["camera"]++;
  std::string type = "perspective";
  cfg.lookupValue("type", type);
  try {
    auto camera = cameraRegistry_.get().create(type, cfg);
    if (!camera) {
      return;
    }
    if (type == "perspective") {
      configurePerspectiveCamera(camera, cfg);
    }
    scene_->setCamera(camera);
  } catch (const raytracer::core::RaytracerException& e) {
    throw raytracer::core::RaytracerException(e.what());
  }
}

std::shared_ptr<Scene> SceneBuilder::build() {
  if (!scene_->getCamera()) {
    throw raytracer::core::RaytracerException(
        "SceneBuilder::build: scene has no camera defined");
  }
  if (scene_->getLights().empty()) {
    throw raytracer::core::RaytracerException(
        "SceneBuilder::build: scene has no light sources defined");
  }
  return scene_;
}

std::size_t SceneBuilder::count(const std::string& type) const {
  const auto it = typeCounts_.find(type);
  return it != typeCounts_.end() ? it->second : 0;
}

}  // namespace raytracer::scene
