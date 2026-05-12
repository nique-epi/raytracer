/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PathIntegrator
*/

#include "integrator/pathIntegrator/PathIntegrator.hpp"
#include <stdexcept>

namespace raytracer::core {

// TODO(nolanfrb): implement path tracing (background sampling + material
// scatter recursion). Throwing keeps accidental wiring loud — a silent
// return-black would regress renderer output to a blank image.
math::Color PathIntegrator::computeRadiance(const math::Ray& /*ray*/,
                                            const scene::Scene& /*scene*/,
                                            int /*depth*/) {
  throw std::logic_error(
      "PathIntegrator::computeRadiance is not implemented yet");
}

}  // namespace raytracer::core
