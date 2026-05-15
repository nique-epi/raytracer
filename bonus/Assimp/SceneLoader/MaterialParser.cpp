/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MaterialParser
*/

#include "MaterialParser.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
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
    std::cerr << "[AssimpMaterialParser] Parsing Assimp material #" << i
              << " / " << scene->mNumMaterials << '\n';
    materials.push_back(parseMaterial(mat));
  }
  return materials;
}

std::shared_ptr<IMaterial> MaterialParser::parseMaterial(aiMaterial* mat) {
  aiColor3D diffuseColor(1.0F, 1.0F, 1.0F);
  mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
  math::Color albedo(diffuseColor.r, diffuseColor.g, diffuseColor.b);

  float opacity = 1.0F;
  mat->Get(AI_MATKEY_OPACITY, opacity);

  float shininess = 0.0F;
  mat->Get(AI_MATKEY_SHININESS, shininess);

  float refractionIndex = 1.0F;
  mat->Get(AI_MATKEY_REFRACTI, refractionIndex);

  std::shared_ptr<IMaterial> material;

  if (opacity < opacityThreshold && refractionIndex > 1.0F) {
    const math::Color glassTint(opacity, opacity, opacity);
    std::cerr << "[AssimpMaterialParser] Created Assimp glass material with"
              << " opacity=" << opacity
              << ", refractionIndex=" << refractionIndex << ", tint=("
              << glassTint.r << ", " << glassTint.g << ", " << glassTint.b
              << ")" << '\n';
    material =
        core::factory::MaterialFactory::createGlass(refractionIndex, glassTint);
  } else if (shininess > shininessThreshold) {
    double fuzz = std::log(1.0 + shininess) / std::log(1.0 + maxShininessValue);
    fuzz = 1.0 - fuzz;
    fuzz = std::clamp(fuzz, 0.0, 1.0);
    std::cerr << "[AssimpMaterialParser] Created Assimp glossy material with"
              << " albedo=(" << albedo.r << ", " << albedo.g << ", " << albedo.b
              << ")"
              << ", shininess=" << shininess << ", fuzz=" << fuzz << '\n';
    material = core::factory::MaterialFactory::createGlossy(fuzz, albedo);
  } else {
    std::cerr << "[AssimpMaterialParser] Created Assimp diffuse material with"
              << " albedo=(" << albedo.r << ", " << albedo.g << ", " << albedo.b
              << ")"
              << ", opacity=" << opacity << ", shininess=" << shininess
              << ", refractionIndex=" << refractionIndex << '\n';
    material = core::factory::MaterialFactory::createDiffuse(albedo);
  }

  return material;
}

}  // namespace raytracer::scene
