/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** NodeParser
*/

#pragma once

#include <assimp/scene.h>
#include <memory>
#include <vector>
#include "components/material/IMaterial.hpp"
#include "scene/SceneBuilder.hpp"

namespace raytracer::scene {

class NodeParser {
 public:
  static void parse(aiNode* rootNode, const aiScene* scene,
                    SceneBuilder& builder, aiMatrix4x4 initialTransform);

 private:
  static void processNode(
      aiNode* node, const aiScene* scene, SceneBuilder& builder,
      const aiMatrix4x4& currentTransform,
      const std::vector<std::shared_ptr<IMaterial>>& materials);
  static void processMesh(
      aiMesh* mesh, SceneBuilder& builder, const aiMatrix4x4& transform,
      const std::vector<std::shared_ptr<IMaterial>>& materials);
};

}  // namespace raytracer::scene
