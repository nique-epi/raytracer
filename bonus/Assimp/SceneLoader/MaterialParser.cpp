/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MaterialParser
*/

#include "MaterialParser.hpp"
#include <algorithm>
#include <cmath>
#include "factory/material/MaterialFactory.hpp"
#include "utils/math/Color.hpp"

namespace {

constexpr float opacityThreshold = 0.9F;
constexpr float shininessThreshold = 32.0F;
constexpr float maxShininessValue = 128.0F;

}  // namespace

namespace raytracer::scene {

std::vector<std::shared_ptr<IMaterial>> MaterialParser::loadMaterials(
    const aiScene* scene) {
  std::vector<std::shared_ptr<IMaterial>> materials;

  for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
    aiMaterial* mat = scene->mMaterials[i];
    materials.push_back(parseMaterial(mat));
  }
  return materials;
}

std::shared_ptr<IMaterial> MaterialParser::parseMaterial(aiMaterial* mat) {
  aiColor3D diffuseColor(1.0F, 1.0F, 1.0F);
  mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
  const math::Color albedo(diffuseColor.r, diffuseColor.g, diffuseColor.b);

  float opacity = 1.0F;
  mat->Get(AI_MATKEY_OPACITY, opacity);

  float refractionIndex = 1.0F;
  mat->Get(AI_MATKEY_REFRACTI, refractionIndex);

  float shininess = 0.0F;
  mat->Get(AI_MATKEY_SHININESS, shininess);

  std::shared_ptr<IMaterial> material;

  if (opacity < opacityThreshold && refractionIndex > 1.0F) {
    const math::Color glassTint(opacity, opacity, opacity);
    material =
        core::factory::MaterialFactory::createGlass(refractionIndex, glassTint);
  } else if (shininess > shininessThreshold) {
    double fuzz = std::log(1.0 + shininess) / std::log(1.0 + maxShininessValue);
    fuzz = 1.0 - fuzz;
    fuzz = std::clamp(fuzz, 0.0, 1.0);
    material = core::factory::MaterialFactory::createGlossy(fuzz, albedo);
  } else {
    material = core::factory::MaterialFactory::createDiffuse(albedo);
  }

  return material;
}

}  // namespace raytracer::scene
