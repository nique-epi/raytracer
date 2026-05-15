/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** WireframeShader
*/

/**
 * @file WireframeShader.hpp
 * @brief Geometry-only viewport shader.
 *
 * Renders the surface normal of the first scene hit as an RGB colour,
 * mapping the normal components from `[-1, 1]` to `[0, 1]`. No
 * material, no lighting, no recursion, no background sampling: rays
 * that miss the scene produce black.
 *
 * This shader is the cheapest of the three: useful for verifying
 * primitive placement and transforms without paying for any shading
 * work.
 */

#pragma once

#include "rendering/shading/IShadingMode.hpp"

namespace raytracer::shading {

class WireframeShader : public IShadingMode {
 public:
  WireframeShader() = default;
  ~WireframeShader() override = default;

  WireframeShader(const WireframeShader&) = delete;
  WireframeShader& operator=(const WireframeShader&) = delete;
  WireframeShader(WireframeShader&&) = delete;
  WireframeShader& operator=(WireframeShader&&) = delete;

  math::Color shade(const math::Ray& ray, const scene::Scene& scene,
                    int depth) override;
};

}  // namespace raytracer::shading
