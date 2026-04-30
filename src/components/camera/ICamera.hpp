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
  ICamera() = default;
  virtual ~ICamera() = default;

  ICamera(const ICamera&) = default;
  ICamera& operator=(const ICamera&) = default;
  ICamera(ICamera&&) = default;
  ICamera& operator=(ICamera&&) = default;

  [[nodiscard]] virtual raytracer::math::Ray getRay(double u,
                                                    double v) const = 0;
  virtual void setResolution(int width, int height) = 0;
};
