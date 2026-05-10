/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PerspectiveCameraPlugin
*/

#include "PerspectiveCamera.hpp"
#include "components/camera/ICamera.hpp"

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

extern "C" gsl::owner<ICamera*> createCamera() {
  return new raytracer::components::camera::perspective::Perspective();
}

extern "C" void DestroyCamera(gsl::owner<ICamera*> camera) { delete camera; }
