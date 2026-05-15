/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MaterialPreviewShader
*/

/**
 * @file MaterialPreviewShader.hpp
 * @brief Env-light-only material preview shader.
 *
 * Implements Blender's "Material Preview" semantics: every scene
 * `ILight` is ignored, and the scene background acts as an infinite
 * environment light sampled on **both** primary and secondary rays.
 * Materials still scatter and recurse, so the shader respects the
 * material's BRDF and emission terms.
 *
 * Recursion is bounded by `depth`: scattered rays are followed until
 * `depth` reaches zero or the ray escapes into the background.
 */

#pragma once

#include "rendering/shading/IShadingMode.hpp"

namespace raytracer::shading {

class MaterialPreviewShader : public IShadingMode {
 public:
  MaterialPreviewShader() = default;
  ~MaterialPreviewShader() override = default;

  MaterialPreviewShader(const MaterialPreviewShader&) = delete;
  MaterialPreviewShader& operator=(const MaterialPreviewShader&) = delete;
  MaterialPreviewShader(MaterialPreviewShader&&) = delete;
  MaterialPreviewShader& operator=(MaterialPreviewShader&&) = delete;

  math::Color shade(const math::Ray& ray, const scene::Scene& scene,
                    int depth) override;
};

}  // namespace raytracer::shading
