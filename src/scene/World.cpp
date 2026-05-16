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

const AmbientOcclusionSettings& World::ambientOcclusion() const {
  return ambientOcclusion_;
}

void World::setAmbientOcclusion(const AmbientOcclusionSettings& settings) {
  ambientOcclusion_ = settings;
}

}  // namespace raytracer::scene
