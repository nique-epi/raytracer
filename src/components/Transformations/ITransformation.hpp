/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ITransformation
*/

#pragma once

#include <memory>
#include "utils/math/Vector3D.hpp"

class ITransformation {
 public:
  virtual ~ITransformation() = default;
  ITransformation() = default;

  ITransformation(const ITransformation& other) = default;
  ITransformation& operator=(const ITransformation& other) = default;
  ITransformation(ITransformation&& other) noexcept = default;
  ITransformation& operator=(ITransformation&& other) noexcept = default;

  [[nodiscard]] virtual raytracer::math::Vector3D apply(
      const raytracer::math::Vector3D& point) const = 0;
  [[nodiscard]] virtual raytracer::math::Vector3D applyToNormal(
      const raytracer::math::Vector3D& normal) const = 0;
  [[nodiscard]] virtual std::shared_ptr<ITransformation> inverse() const = 0;
};
