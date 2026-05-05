/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Translation
*/

#pragma once

#include "components/Transformations/ITransformation.hpp"

namespace raytracer::components::transformation {

class Translation : public ITransformation {
 public:
  Translation() = default;
  explicit Translation(const raytracer::math::Vector3D& offset);
  ~Translation() override;

    Translation(const Translation& other) = default;
    Translation& operator=(const Translation& other) = default;
    Translation(Translation&& other) noexcept = default;
    Translation& operator=(Translation&& other) noexcept = default;

  void setOffset(const raytracer::math::Vector3D& offset);

  [[nodiscard]] raytracer::math::Vector3D apply(
      const raytracer::math::Vector3D& point) const override;
  [[nodiscard]] raytracer::math::Vector3D applyToNormal(
      const raytracer::math::Vector3D& normal) const override;
  [[nodiscard]] std::shared_ptr<ITransformation> inverse() const override;

 private:
  raytracer::math::Vector3D _offset;
};

}  // namespace raytracer::components::transformation
