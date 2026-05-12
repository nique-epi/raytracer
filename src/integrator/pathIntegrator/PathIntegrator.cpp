/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PathIntegrator
*/

#include "integrator/pathIntegrator/PathIntegrator.hpp"

namespace raytracer::core {

math::Color PathIntegrator::Li(const math::Ray& /*ray*/,
                               const scene::Scene& /*scene*/, int /*depth*/) {
  return math::Color{0.0, 0.0, 0.0};
}

}  // namespace raytracer::core
