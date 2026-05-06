/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Scene
*/

#include "Scene.hpp"
#include <utility>

namespace raytracer::scene {

void Scene::add(const std::shared_ptr<IObject>& object) {
  _rootCollection.add(object);
}

void Scene::addLight(std::shared_ptr<ILight> light) {
  _lights.push_back(std::move(light));
}

void Scene::setCamera(std::shared_ptr<ICamera> camera) {
  _camera = std::move(camera);
}

bool Scene::hit(const raytracer::math::Ray& ray, double tMin, double tMax,
                raytracer::math::HitRecord& rec) const {
  return _rootCollection.hits(ray, tMin, tMax, rec);
}

std::shared_ptr<ICamera> Scene::getCamera() const { return _camera; }

const std::vector<std::shared_ptr<ILight>>& Scene::getLights() const {
  return _lights;
}

}  // namespace raytracer::scene
