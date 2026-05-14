/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** AssimpLoader
*/

#include "AssimpLoader.hpp"
#include <iostream>
#include "CameraParser.hpp"
#include "LightsParser.hpp"
#include "NodeParser.hpp"
#include "background/Solid/SolidBackground.hpp"
#include "scene/SceneBuilder.hpp"

namespace {
unsigned int countTriangles(const aiScene* scene) {
  unsigned int triangleCount = 0;

  for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
    const aiMesh* mesh = scene->mMeshes[i];

    for (unsigned int j = 0; j < mesh->mNumFaces; ++j) {
      if (mesh->mFaces[j].mNumIndices == 3) {
        ++triangleCount;
      }
    }
  }

  return triangleCount;
}

}  // namespace

namespace raytracer::scene {

bool AssimpLoader::load(const std::string& path, SceneBuilder& builder,
                        math::RenderSettings& settings) {
  Assimp::Importer importer;
  const aiScene* scene =
      importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |
                                  aiProcess_GenSmoothNormals);

  if (scene == nullptr || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0 ||
      scene->mRootNode == nullptr) {
    return false;
  }

  std::cerr << "Loaded GLTF: " << scene->mNumMeshes << " meshes, "
            << scene->mNumMaterials << " materials, " << scene->mNumCameras
            << " cameras, " << scene->mNumLights << " lights, "
            << countTriangles(scene) << " triangles" << '\n';

  NodeParser::parse(scene->mRootNode, scene, builder, aiMatrix4x4());
  LightParser::parse(scene, builder);
  CameraParser::parse(scene, builder, settings);
  builder.setBackground(
      std::make_shared<raytracer::scene::background::SolidBackground>(
          math::Color{0.5, 0.5, 0.5}));
  return true;
}

[[nodiscard]] bool AssimpLoader::supports(const std::string& ext) const {
  return ext == "obj" || ext == "fbx" || ext == "dae" || ext == "glb" ||
         ext == "gltf";
}
}  // namespace raytracer::scene
