/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Scene
*/

#pragma once

#include <memory>
#include <vector>
#include "components/Primitives/Collection.hpp"
#include "components/Primitives/IObject.hpp"
#include "components/camera/ICamera.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"

class ILight;

namespace raytracer::scene {

class Scene {
 public:
  Scene() = default;
  ~Scene() = default;

  Scene(const Scene&) = delete;
  Scene& operator=(const Scene&) = delete;
  Scene(Scene&&) = delete;
  Scene& operator=(Scene&&) = delete;

  void add(const std::shared_ptr<IObject>& object);
  void addLight(std::shared_ptr<ILight> light);
  void setCamera(std::shared_ptr<ICamera> camera);

  bool hit(const raytracer::math::Ray& ray, double tMin, double tMax,
           raytracer::math::HitRecord& rec) const;

  [[nodiscard]] std::shared_ptr<ICamera> getCamera() const;
  [[nodiscard]] const std::vector<std::shared_ptr<ILight>>& getLights() const;

 private:
  raytracer::components::Collection _rootCollection;
  std::vector<std::shared_ptr<ILight>> _lights;
  std::shared_ptr<ICamera> _camera;
};

}  // namespace raytracer::scene
