/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SceneBuilder
*/

#include "SceneBuilder.hpp"
#include "SceneBuildException.hpp"
#include "constants/Errors.hpp"
#include "scene/Scene.hpp"

namespace raytracer::scene {

SceneBuilder::SceneBuilder(raytracer::core::factory::IComponentFactory& factory)
    : scene_(std::make_shared<Scene>()), factory_(factory) {}

void SceneBuilder::addObject(const std::string& type,
                             const libconfig::Setting& cfg) {
  typeCounts_[type]++;
  try {
    auto obj = factory_.get().createPrimitive(type, cfg);
    if (obj) {
      scene_->add(obj);
    }
  } catch (const raytracer::core::RaytracerException& e) {
    throw SceneBuildException(e.what());
  }
}

void SceneBuilder::addLight(const std::string& type,
                            const libconfig::Setting& cfg) {
  typeCounts_[type]++;
  try {
    auto light = factory_.get().createLight(type, cfg);
    if (light) {
      scene_->addLight(light);
    }
  } catch (const raytracer::core::RaytracerException& e) {
    throw SceneBuildException(e.what());
  }
}

void SceneBuilder::addCamera(const libconfig::Setting& cfg) {
  typeCounts_["camera"]++;
  std::string type = "perspective";
  cfg.lookupValue("type", type);
  try {
    auto camera = factory_.get().createCamera(type, cfg);
    if (camera) {
      scene_->setCamera(camera);
    }
  } catch (const raytracer::core::RaytracerException& e) {
    throw SceneBuildException(e.what());
  }
}

void SceneBuilder::setBackground(
    std::shared_ptr<raytracer::scene::background::IBackground> background) {
  scene_->setBackground(std::move(background));
}

std::shared_ptr<Scene> SceneBuilder::build() {
  if (!scene_->getCamera()) {
    throw SceneBuildException(
        std::string(raytracer::constants::errors::SCENE_BUILD_ERROR) +
        ": scene has no camera defined");
  }
  if (scene_->getLights().empty()) {
    throw SceneBuildException(
        std::string(raytracer::constants::errors::SCENE_BUILD_ERROR) +
        ": scene has no light sources defined");
  }
  return scene_;
}

std::size_t SceneBuilder::count(const std::string& type) const {
  const auto it = typeCounts_.find(type);
  return it != typeCounts_.end() ? it->second : 0;
}

}  // namespace raytracer::scene
