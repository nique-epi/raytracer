/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MaterialParser
*/

#include "MaterialParser.hpp"
#include <assimp/pbrmaterial.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include "factory/material/MaterialFactory.hpp"
#include "utils/math/Color.hpp"

namespace {

constexpr float opacityThreshold = 0.9F;
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
  aiColor4D baseColor4(1.0F, 1.0F, 1.0F, 1.0F);
  mat->Get(AI_MATKEY_BASE_COLOR, baseColor4);
  aiColor3D diffuseColor(1.0F, 1.0F, 1.0F);
  mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
  const math::Color baseColor(
      (baseColor4.r != 1.0F || baseColor4.g != 1.0F || baseColor4.b != 1.0F)
          ? math::Color(baseColor4.r, baseColor4.g, baseColor4.b)
          : math::Color(diffuseColor.r, diffuseColor.g, diffuseColor.b));

  float opacity = 1.0F;
  mat->Get(AI_MATKEY_OPACITY, opacity);

  float refractionIndex = 1.0F;
  mat->Get(AI_MATKEY_REFRACTI, refractionIndex);

  float roughness = -1.0F;
  mat->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness);

  float metallic = 0.0F;
  mat->Get(AI_MATKEY_METALLIC_FACTOR, metallic);

  float shininess = 0.0F;
  mat->Get(AI_MATKEY_SHININESS, shininess);

  std::shared_ptr<IMaterial> material;

  if (opacity < opacityThreshold && refractionIndex > 1.0F) {
    const math::Color glassTint(opacity, opacity, opacity);
    std::cerr << "[AssimpMaterialParser] Created glass material with"
              << " opacity=" << opacity
              << ", refractionIndex=" << refractionIndex << ", tint=("
              << glassTint.r << ", " << glassTint.g << ", " << glassTint.b
              << ")" << '\n';
    material =
        core::factory::MaterialFactory::createGlass(refractionIndex, glassTint);
  } else {
    double finalMetallic = metallic;
    double finalRoughness = roughness >= 0.0F ? roughness : 0.5;
    if (roughness < 0.0F && shininess > 0.0F) {
      finalRoughness =
          std::log(1.0 + shininess) / std::log(1.0 + maxShininessValue);
      finalRoughness = std::clamp(finalRoughness, 0.0, 1.0);
    }
    std::cerr << "[AssimpMaterialParser] Created principledBSDF material with"
              << " baseColor=(" << baseColor.r << ", " << baseColor.g << ", "
              << baseColor.b << ")"
              << ", metallic=" << finalMetallic
              << ", roughness=" << finalRoughness << ", ior=" << refractionIndex
              << '\n';
    material = core::factory::MaterialFactory::createPrincipled(
        baseColor, finalMetallic, finalRoughness, refractionIndex);
  }

  return material;
}

}  // namespace raytracer::scene
