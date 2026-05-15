/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** RenderedShader
*/

#include "shading/rendered/RenderedShader.hpp"

#include <utility>

#include "integrator/IIntegrator.hpp"
#include "shading/ShadingException.hpp"

namespace raytracer::shading {

RenderedShader::RenderedShader(std::shared_ptr<core::IIntegrator> integrator)
    : integrator_(std::move(integrator)) {
  if (!integrator_) {
    throw ShadingException("RenderedShader requires a non-null IIntegrator");
  }
}

math::Color RenderedShader::shade(const math::Ray& ray,
                                  const scene::Scene& scene, int depth) {
  return integrator_->computeRadiance(ray, scene, depth);
}

}  // namespace raytracer::shading
