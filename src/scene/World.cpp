/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** World
*/

#include "World.hpp"

#include "exceptions/Exceptions.hpp"

namespace raytracer::scene {

World::World() : viewportMode_(ViewportMode::Rendered) {}

ViewportMode World::viewportMode() const { return viewportMode_; }

void World::setViewportMode(ViewportMode mode) { viewportMode_ = mode; }

ViewportMode nextViewportMode(ViewportMode mode) {
  switch (mode) {
    case ViewportMode::Wireframe:
      return ViewportMode::MaterialPreview;
    case ViewportMode::MaterialPreview:
      return ViewportMode::Rendered;
    case ViewportMode::Rendered:
      return ViewportMode::Wireframe;
  }
  throw core::RaytracerException("nextViewportMode: unknown ViewportMode");
}

const char* viewportModeName(ViewportMode mode) {
  switch (mode) {
    case ViewportMode::Wireframe:
      return "Wireframe";
    case ViewportMode::MaterialPreview:
      return "Material Preview";
    case ViewportMode::Rendered:
      return "Rendered";
  }
  throw core::RaytracerException("viewportModeName: unknown ViewportMode");
}

}  // namespace raytracer::scene
