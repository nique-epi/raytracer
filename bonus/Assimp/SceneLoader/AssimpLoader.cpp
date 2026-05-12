/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** AssimpLoader
*/

#include "AssimpLoader.hpp"
#include <memory>
#include <stack>
#include "components/Primitives/mesh/mesh.hpp"
#include "components/Primitives/triangle/Triangle.hpp"
#include "components/material/IMaterial.hpp"
#include "scene/SceneBuilder.hpp"
#include "utils/math/Vector3D.hpp"

namespace raytracer::scene {

struct NodeData {
  aiNode* node;
  aiMatrix4x4 transform;
};

void AssimpLoader::processNode(
    aiNode* rootNode, const aiScene* scene, SceneBuilder& builder,
    aiMatrix4x4 initialTransform,
    const std::vector<std::shared_ptr<IMaterial>>& materials) {
  std::stack<NodeData> stack;
  stack.push({rootNode, initialTransform});

  while (!stack.empty()) {
    NodeData current = stack.top();
    stack.pop();
    aiNode* node = current.node;
    aiMatrix4x4 currentTransform = current.transform * node->mTransformation;
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      processMesh(mesh, builder, currentTransform, materials);
    }
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
      stack.push({node->mChildren[i], currentTransform});
    }
  }
}

void AssimpLoader::processMesh(
    aiMesh* mesh, SceneBuilder& builder, aiMatrix4x4 transform,
    const std::vector<std::shared_ptr<IMaterial>>& materials) {
  auto meshMaterial = (mesh->mMaterialIndex < materials.size())
                          ? materials[mesh->mMaterialIndex]
                          : nullptr;

  auto myMesh = std::make_shared<raytracer::components::primitives::Mesh>();

  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    const aiFace& face = mesh->mFaces[i];
    if (face.mNumIndices != 3) {
      continue;
    }

    aiVector3D v0 = transform * mesh->mVertices[face.mIndices[0]];
    aiVector3D v1 = transform * mesh->mVertices[face.mIndices[1]];
    aiVector3D v2 = transform * mesh->mVertices[face.mIndices[2]];

    auto tri = std::make_shared<raytracer::components::primitives::Triangle>(
        math::Point3D(v0.x, v0.y, v0.z), math::Point3D(v1.x, v1.y, v1.z),
        math::Point3D(v2.x, v2.y, v2.z), meshMaterial);

    myMesh->addTriangle(std::move(tri));
  }
  builder.addObject(std::move(myMesh));
}

std::vector<std::shared_ptr<IMaterial>> AssimpLoader::loadMaterials(
    const aiScene* scene) {}

void AssimpLoader::processLights(const aiScene* scene, SceneBuilder& builder) {}

void AssimpLoader::processCamera(const aiScene* scene, SceneBuilder& builder,
                                 math::RenderSettings& settings) {}

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

  std::vector<std::shared_ptr<IMaterial>> materials = loadMaterials(scene);
  processNode(scene->mRootNode, scene, builder, aiMatrix4x4(), materials);
  processLights(scene, builder);
  processCamera(scene, builder, settings);

  return true;
}

[[nodiscard]] bool AssimpLoader::supports(const std::string& ext) const {
  return ext == "obj" || ext == "fbx" || ext == "dae" || ext == "glb" ||
         ext == "gltf";
}
}  // namespace raytracer::scene
