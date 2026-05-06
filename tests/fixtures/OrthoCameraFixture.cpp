/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** OrthoCameraFixture
*/

#include "OrthoCameraFixture.hpp"

raytracer::math::Ray OrthoCameraFixture::getRay(double u, double v) const {
  double x = (u - 0.5) * 2.0;
  double y = (v - 0.5) * 2.0;
  return raytracer::math::Ray(raytracer::math::Vector3D(x, y, 1.0),
                              raytracer::math::Vector3D(0, 0, -1.0));
}

void OrthoCameraFixture::setResolution(int, int) {}
