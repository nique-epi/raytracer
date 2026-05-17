/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** WireframeShader
*/

/**
 * @file WireframeShader.hpp
 * @brief Geometry-only viewport shader rendered as Blender-style grey.
 *
 * Shades the first scene hit with a flat grey whose brightness follows
 * the facing ratio @c max(0, N·V) — surfaces facing the camera are
 * light grey, silhouette edges fall back to a dark grey floor. This
 * matches Blender's "Solid" viewport look: the geometry reads as a
 * three-dimensional clay model, with no material colour, no lighting,
 * no recursion, and no background sampling. Rays that miss the scene
 * produce black.
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
