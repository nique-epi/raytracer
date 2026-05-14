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
constexpr float maxShininessValue = 1000.0F;

raytracer::math::Color readMaterialColor(aiMaterial* mat, bool& hasBaseColor) {
  aiColor4D baseColor(1.0F, 1.0F, 1.0F, 1.0F);
  if (mat->Get(AI_MATKEY_BASE_COLOR, baseColor) == AI_SUCCESS) {
    hasBaseColor = true;
    return {baseColor.r, baseColor.g, baseColor.b};
  }

  aiColor3D diffuseColor(1.0F, 1.0F, 1.0F);
  if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) == AI_SUCCESS) {
    return {diffuseColor.r, diffuseColor.g, diffuseColor.b};
  }

  return {1.0, 1.0, 1.0};
}

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

MaterialParser::MaterialProperties MaterialParser::extractMaterialProperties(
    aiMaterial* mat) {
  MaterialProperties properties;

  bool hasBaseColor = false;
  properties.albedo = readMaterialColor(mat, hasBaseColor);

  properties.opacity = 1.0F;
  mat->Get(AI_MATKEY_OPACITY, properties.opacity);

  properties.shininess = 0.0F;
  mat->Get(AI_MATKEY_SHININESS, properties.shininess);

  properties.metallicFactor = 0.0F;
  mat->Get(AI_MATKEY_METALLIC_FACTOR, properties.metallicFactor);

  properties.refractionIndex = 1.0F;
  mat->Get(AI_MATKEY_REFRACTI, properties.refractionIndex);

  return properties;
}

std::shared_ptr<IMaterial> MaterialParser::createMaterialFromProperties(
    const MaterialProperties& properties) {
  if (properties.opacity < opacityThreshold &&
      properties.refractionIndex > 1.0F) {
    const math::Color glassTint(properties.opacity, properties.opacity,
                                properties.opacity);
    return core::factory::MaterialFactory::createGlass(
        properties.refractionIndex, glassTint);
  }

  double roughness = 0.5;
  if (properties.shininess > 0) {
    roughness = 1.0 - std::log(1.0 + properties.shininess) /
                          std::log(1.0 + maxShininessValue);
  }
  roughness = std::clamp(roughness, 0.0, 1.0);

  return core::factory::MaterialFactory::createPrincipled(
      properties.albedo, static_cast<double>(properties.metallicFactor),
      roughness, static_cast<double>(properties.refractionIndex));
}

std::shared_ptr<IMaterial> MaterialParser::parseMaterial(aiMaterial* mat) {
  const MaterialProperties properties = extractMaterialProperties(mat);
  return createMaterialFromProperties(properties);
}

}  // namespace raytracer::scene
