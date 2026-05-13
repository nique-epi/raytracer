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

}  // namespace raytracer::scene
