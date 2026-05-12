/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** NodeParser
*/

#include "NodeParser.hpp"
#include <memory>
#include <stack>
#include "MaterialParser.hpp"
#include "components/Primitives/mesh/mesh.hpp"
#include "components/Primitives/triangle/Triangle.hpp"
#include "utils/math/Vector3D.hpp"

namespace raytracer::scene {

void NodeParser::parse(aiNode* rootNode, const aiScene* scene,
                       SceneBuilder& builder, aiMatrix4x4 initialTransform) {
  std::vector<std::shared_ptr<IMaterial>> materials =
      MaterialParser::loadMaterials(scene);
  processNode(rootNode, scene, builder, initialTransform, materials);
}

void NodeParser::processNode(
    aiNode* node, const aiScene* scene, SceneBuilder& builder,
    const aiMatrix4x4& currentTransform,
    const std::vector<std::shared_ptr<IMaterial>>& materials) {
  struct NodeData {
    aiNode* node;
    aiMatrix4x4 transform;
  };

  std::stack<NodeData> stack;
  stack.push({node, currentTransform});

  while (!stack.empty()) {
    NodeData current = stack.top();
    stack.pop();
    aiNode* currentNode = current.node;
    aiMatrix4x4 nodeTransform =
        current.transform * currentNode->mTransformation;

    for (unsigned int i = 0; i < currentNode->mNumMeshes; ++i) {
      aiMesh* mesh = scene->mMeshes[currentNode->mMeshes[i]];
      processMesh(mesh, builder, nodeTransform, materials);
    }

    for (unsigned int i = 0; i < currentNode->mNumChildren; ++i) {
      stack.push({currentNode->mChildren[i], nodeTransform});
    }
  }
}

void NodeParser::processMesh(
    aiMesh* mesh, SceneBuilder& builder, const aiMatrix4x4& transform,
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

}  // namespace raytracer::scene
