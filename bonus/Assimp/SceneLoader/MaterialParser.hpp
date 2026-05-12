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

namespace raytracer::scene {

class MaterialParser {
 public:
  static std::vector<std::shared_ptr<IMaterial>> loadMaterials(
      const aiScene* scene);

 private:
  static std::shared_ptr<IMaterial> parseMaterial(aiMaterial* mat);
};

}  // namespace raytracer::scene
