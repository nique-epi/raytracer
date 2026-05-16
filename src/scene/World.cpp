/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** World
*/

#include "World.hpp"

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
  return ViewportMode::Rendered;
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
  return "Unknown";
}

}  // namespace raytracer::scene
