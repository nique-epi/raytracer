/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ICamera
*/

#pragma once

#include "utils/math/Ray.hpp"

class ICamera {
 public:
  virtual ~ICamera() = default;

  [[nodiscard]] virtual raytracer::math::Ray getRay(double u,
                                                    double v) const = 0;
  virtual void setResolution(int width, int height) = 0;
};
