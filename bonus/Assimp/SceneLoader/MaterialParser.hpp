/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MaterialParser
*/

#pragma once

#include <assimp/scene.h>
#include <memory>
#include <vector>
#include "components/material/IMaterial.hpp"
#include "utils/math/Color.hpp"

namespace raytracer::scene {

class MaterialParser {
 public:
  static std::vector<std::shared_ptr<IMaterial>> loadMaterials(
      const aiScene* scene);

 private:
  struct MaterialProperties {
    math::Color albedo;
    float opacity;
    float shininess;
    float metallicFactor;
    float refractionIndex;
  };

  static std::shared_ptr<IMaterial> parseMaterial(aiMaterial* mat);
  static MaterialProperties extractMaterialProperties(aiMaterial* mat);
  static std::shared_ptr<IMaterial> createMaterialFromProperties(
      const MaterialProperties& properties);
};

}  // namespace raytracer::scene
