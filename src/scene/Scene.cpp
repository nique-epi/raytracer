/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Scene
*/

#include "Scene.hpp"
#include <utility>
#include <vector>
#include "background/Solid/SolidBackground.hpp"
#include "components/Primitives/BVH/BVHNode.hpp"

namespace raytracer::scene {

void Scene::add(const std::shared_ptr<IObject>& object) {
  rootCollection_.add(object);
}

void Scene::addLight(std::shared_ptr<ILight> light) {
  lights_.push_back(std::move(light));
}

void Scene::setCamera(std::shared_ptr<ICamera> camera) {
  camera_ = std::move(camera);
}

bool Scene::hit(const raytracer::math::Ray& ray, double tMin, double tMax,
                raytracer::math::HitRecord& rec) const {
  if (accelerator_) {
    return accelerator_->hits(ray, tMin, tMax, rec);
  }
  return rootCollection_.hits(ray, tMin, tMax, rec);
}

void Scene::buildBVH() {
  const std::vector<std::shared_ptr<IObject>>& objects =
      rootCollection_.getObjects();
  if (objects.empty()) {
    accelerator_ = nullptr;
    return;
  }

  for (const auto& obj : objects) {
    if (obj) {
      obj->buildAccelerationStructure();
    }
  }
  accelerator_ = std::make_shared<raytracer::components::BVHNode>(objects);
}

std::shared_ptr<ICamera> Scene::getCamera() const { return camera_; }

const std::vector<std::shared_ptr<ILight>>& Scene::getLights() const {
  return lights_;
}

std::shared_ptr<raytracer::scene::background::IBackground>
Scene::getBackground() const {
  return background_;
}

void Scene::setBackground(
    std::shared_ptr<raytracer::scene::background::IBackground> background) {
  background_ = std::move(background);
}

const World& Scene::getWorld() const { return world_; }

World& Scene::getWorld() { return world_; }

Scene::Scene()
    : background_(
          std::make_shared<raytracer::scene::background::SolidBackground>(
              raytracer::math::Color{0, 0, 0})) {}

}  // namespace raytracer::scene
