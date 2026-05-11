/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** AssimpLoader
*/

#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include "components/material/IMaterial.hpp"
#include "scene/ISceneLoader.hpp"

namespace raytracer::scene {

class AssimpLoader : public ISceneLoader {
 public:
  AssimpLoader() = default;
  ~AssimpLoader() override = default;

  AssimpLoader(const AssimpLoader&) = delete;
  AssimpLoader& operator=(const AssimpLoader&) = delete;
  AssimpLoader(AssimpLoader&&) = delete;
  AssimpLoader& operator=(AssimpLoader&&) = delete;

  bool load(const std::string& path, SceneBuilder& builder,
            math::RenderSettings& settings) override;

  [[nodiscard]] bool supports(const std::string& ext) const override;

 private:
  void processNode(aiNode* rootNode, const aiScene* scene,
                   SceneBuilder& builder, aiMatrix4x4 initialTransform,
                   const std::vector<std::shared_ptr<IMaterial>>& materials);
  void processMesh(aiMesh* mesh, SceneBuilder& builder, aiMatrix4x4 transform,
                   const std::vector<std::shared_ptr<IMaterial>>& materials);
  void processLights(const aiScene* scene, SceneBuilder& builder);
  void processCamera(const aiScene* scene, SceneBuilder& builder,
                     math::RenderSettings& settings);
  std::vector<std::shared_ptr<IMaterial>> loadMaterials(const aiScene* scene);
};

}  // namespace raytracer::scene
