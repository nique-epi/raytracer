/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** perspectiveCamera
*/

#include "component/camera/perspective/PerspectiveCamera.hpp"
#include <cmath>
#include <numbers>
#include <random>
#include "utils/math/Ray.hpp"

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

namespace {
constexpr double kApertureRadiusFactor = 0.5;
}  // namespace

namespace raytracer::component::camera {

Perspective::Perspective() { updateCamera(); }

Perspective::Perspective(const Math::Vector3D& lookFrom,
                         const Math::Vector3D& lookAt,
                         const Math::Vector3D& vup, double vfov,
                         double aspectRatio, double aperture, double focusDist)
    : position(lookFrom),
      target(lookAt),
      up(vup),
      fov(vfov),
      aspectRatio(aspectRatio),
      aperture(aperture),
      focusDist(focusDist) {
  updateCamera();
}

Math::Ray Perspective::getRay(double u, double v) const {
  Math::Vector3D rd = (aperture * kApertureRadiusFactor) * randomInUnitDisk();
  Math::Vector3D offset = this->u * rd.x + this->v * rd.y;
  Math::Ray ray(position + offset, lowerLeftCorner + u * horizontal +
                                       v * vertical - position - offset);
  return ray;
}

void Perspective::setResolution(int width, int height) {
  aspectRatio = static_cast<double>(width) / static_cast<double>(height);
  updateCamera();
}

void Perspective::updateCamera() {
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
  double theta = fov * std::numbers::pi / 180.0;
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
  double halfHeight = std::tan(theta / 2.0);
  double halfWidth = aspectRatio * halfHeight;

  w = (position - target).normalize();
  u = up.cross(w).normalize();
  v = w.cross(u);

  lowerLeftCorner = position - halfWidth * focusDist * u -
                    halfHeight * focusDist * v - focusDist * w;
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
  horizontal = 2.0 * halfWidth * focusDist * u;
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
  vertical = 2.0 * halfHeight * focusDist * v;
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
Math::Vector3D Perspective::randomInUnitDisk() const {
  thread_local std::mt19937 gen(std::random_device{}());
  thread_local std::uniform_real_distribution<double> dist(-1.0, 1.0);

  while (true) {
    Math::Vector3D p(dist(gen), dist(gen), 0.0);
    if (p.dot(p) < 1.0) {
      return p;
    }
  }
}

}  // namespace raytracer::component::camera

extern "C" gsl::owner<ICamera*> createCamera() {
  return new raytracer::component::camera::Perspective();
}

extern "C" void DestroyCamera(gsl::owner<ICamera*> camera) { delete camera; }
