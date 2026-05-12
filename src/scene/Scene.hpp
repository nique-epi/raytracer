/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Scene
*/

#pragma once

#include <memory>
#include <vector>
#include "World.hpp"
#include "background/IBackground.hpp"
#include "components/Primitives/Collection/Collection.hpp"
#include "components/Primitives/IObject.hpp"
#include "components/camera/ICamera.hpp"
#include "components/light/ILight.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"

namespace raytracer::scene {

class Scene {
 public:
  Scene();
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
  [[nodiscard]] std::shared_ptr<raytracer::scene::background::IBackground>
  getBackground() const;
  void setBackground(
      std::shared_ptr<raytracer::scene::background::IBackground> background);

  [[nodiscard]] const World& getWorld() const;
  [[nodiscard]] World& getWorld();

 private:
  raytracer::components::Collection rootCollection_;
  std::vector<std::shared_ptr<ILight>> lights_;
  std::shared_ptr<ICamera> camera_;
  std::shared_ptr<raytracer::scene::background::IBackground> background_;
  World world_;
};

}  // namespace raytracer::scene
