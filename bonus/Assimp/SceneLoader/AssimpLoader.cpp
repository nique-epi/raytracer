/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** AssimpLoader
*/

#include "AssimpLoader.hpp"
#include <iostream>
#include <memory>
#include <numbers>
#include <stack>
#include "background/Solid/SolidBackground.hpp"
#include "components/Primitives/mesh/mesh.hpp"
#include "components/Primitives/triangle/Triangle.hpp"
#include "components/camera/perspective/PerspectiveCamera.hpp"
#include "components/light/ambient/AmbientLight.hpp"
#include "components/light/directional/Directional.hpp"
#include "components/material/IMaterial.hpp"
#include "factory/material/MaterialFactory.hpp"
#include "scene/SceneBuilder.hpp"
#include "utils/math/Vector3D.hpp"

namespace {
namespace components = raytracer::components;
namespace math = raytracer::math;
namespace scene = raytracer::scene;

constexpr double halfTurn = 2.0;
constexpr double degreesPerRadian = 180.0 / std::numbers::pi;
constexpr float cameraForwardAxis = -1.F;
constexpr float cameraUpAxis = 1.F;
constexpr float fallbackLightDirectionComponent = 0.5F;

math::Color toColor(const aiColor3D& color) {
  return {color.r, color.g, color.b};
}

math::Vector3D toVector3D(const aiVector3D& vector) {
  return {vector.x, vector.y, vector.z};
}

void logLight(const aiLight* light, const math::Color& color,
              unsigned int index) {
  std::cerr << "  light[" << index << "] name='" << light->mName.C_Str()
            << "' type=" << light->mType << " diffuse=(" << color.r << ", "
            << color.g << ", " << color.b << ") ambient=("
            << light->mColorAmbient.r << ", " << light->mColorAmbient.g << ", "
            << light->mColorAmbient.b << ")" << '\n';
}

void addDirectionalLight(scene::SceneBuilder& builder, const aiLight* light,
                         const math::Color& color) {
  auto directionalLight =
      std::make_shared<components::light::directional::Directional>(
          math::Vector3D(light->mDirection.x, light->mDirection.y,
                         light->mDirection.z),
          color, 1.0);
  builder.addLight(std::move(directionalLight));
}

void addAmbientLight(scene::SceneBuilder& builder, const aiLight* light) {
  const math::Color ambient = toColor(light->mColorAmbient);
  auto ambientLight =
      std::make_shared<raytracer::components::light::ambient::Ambient>(ambient,
                                                                       1.0);
  builder.addLight(std::move(ambientLight));
}

void addFallbackLight(scene::SceneBuilder& builder) {
  auto fallback = std::make_shared<components::light::directional::Directional>(
      math::Vector3D(-fallbackLightDirectionComponent, -1.0,
                     -fallbackLightDirectionComponent),
      math::Color(1.0, 1.0, 1.0), 1.0);
  builder.addLight(std::move(fallback));
}

aiMatrix4x4 buildCameraTransform(const aiScene* scene, const aiCamera* cam) {
  aiMatrix4x4 camTransform;
  aiNode* camNode = scene->mRootNode->FindNode(cam->mName);

  if (camNode != nullptr) {
    aiNode* current = camNode;
    while (current != nullptr) {
      camTransform = current->mTransformation * camTransform;
      current = current->mParent;
    }
  }

  return camTransform;
}

double computeVerticalFov(const aiCamera* cam, double aspectRatio) {
  const double hfov = cam->mHorizontalFOV;
  const double vfovRad =
      halfTurn * std::atan(std::tan(hfov / halfTurn) / aspectRatio);
  return vfovRad * degreesPerRadian;
}
}  // namespace

namespace raytracer::scene {

void AssimpLoader::processNode(
    aiNode* rootNode, const aiScene* scene, SceneBuilder& builder,
    aiMatrix4x4 initialTransform,
    const std::vector<std::shared_ptr<IMaterial>>& materials) {
  struct NodeData {
    aiNode* node;
    aiMatrix4x4 transform;
  };

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
    if (scene->mCameras != nullptr && scene->mNumCameras > 0) {
      for (unsigned int j = 0; j < scene->mNumCameras; ++j) {
        if (scene->mCameras[j]->mName == node->mName) {
          std::cerr << "Found camera node: " << node->mName.C_Str() << '\n';
        }
      }
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

    if (mesh->mNumFaces > 0 && i == 0) {
      std::cerr << "  First triangle: (" << v0.x << "," << v0.y << "," << v0.z
                << ") (" << v1.x << "," << v1.y << "," << v1.z << ") (" << v2.x
                << "," << v2.y << "," << v2.z << ")" << '\n';
    }

    auto tri = std::make_shared<raytracer::components::primitives::Triangle>(
        math::Point3D(v0.x, v0.y, v0.z), math::Point3D(v1.x, v1.y, v1.z),
        math::Point3D(v2.x, v2.y, v2.z), meshMaterial);

    myMesh->addTriangle(std::move(tri));
  }
  builder.addObject(std::move(myMesh));
}

std::vector<std::shared_ptr<IMaterial>> AssimpLoader::loadMaterials(
    const aiScene* scene) {
  std::vector<std::shared_ptr<IMaterial>> materials;
  for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
    aiMaterial* mat = scene->mMaterials[i];
    aiColor3D color(1.0, 1.0, 1.0);
    mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);

    materials.push_back(core::factory::MaterialFactory::createDiffuse(
        math::Color(color.r, color.g, color.b)));
  }
  return materials;
}

void AssimpLoader::processLights(const aiScene* scene, SceneBuilder& builder) {
  bool hasLight = false;
  std::cerr << "AssimpLoader::processLights: " << scene->mNumLights
            << " light(s) found" << '\n';

  for (unsigned int i = 0; i < scene->mNumLights; ++i) {
    aiLight* light = scene->mLights[i];
    const math::Color color = toColor(light->mColorDiffuse);
    logLight(light, color, i);

    if (light->mType == aiLightSource_DIRECTIONAL) {
      addDirectionalLight(builder, light, color);
      std::cerr << "    -> added directional light" << '\n';
    } else if (light->mType == aiLightSource_AMBIENT) {
      addAmbientLight(builder, light);
      std::cerr << "    -> added ambient light" << '\n';
    } else {
      addFallbackLight(builder);
      std::cerr << "    -> added fallback directional light" << '\n';
    }

    hasLight = true;
  }

  if (!hasLight) {
    std::cerr << "  no supported light found -> adding default ambient" << '\n';
    builder.addLight(
        std::make_shared<raytracer::components::light::ambient::Ambient>(
            math::Color(1.0, 1.0, 1.0), 1.0));
  }
}

void AssimpLoader::processCamera(const aiScene* scene, SceneBuilder& builder,
                                 math::RenderSettings& settings) {
  if (scene->mNumCameras == 0) {
    return;
  }

  aiCamera* cam = scene->mCameras[0];
  aiMatrix4x4 camTransform = buildCameraTransform(scene, cam);

  aiVector3D pos;
  aiVector3D scale;
  aiQuaternion rot;
  camTransform.Decompose(scale, rot, pos);

  aiMatrix3x3 rotMat(rot.GetMatrix());
  aiVector3D forward = rotMat * aiVector3D(0.F, 0.F, cameraForwardAxis);
  aiVector3D up = rotMat * aiVector3D(0.F, cameraUpAxis, 0.F);

  math::Vector3D position = toVector3D(pos);
  math::Vector3D target(pos.x + forward.x, pos.y + forward.y,
                        pos.z + forward.z);
  math::Vector3D upVec = toVector3D(up);

  double aspectRatio = static_cast<double>(settings.imageWidth) /
                       static_cast<double>(settings.imageHeight);

  double vfov = computeVerticalFov(cam, aspectRatio);

  std::cerr << "Camera: pos=(" << pos.x << "," << pos.y << "," << pos.z
            << ") target=(" << target.x << "," << target.y << "," << target.z
            << ") vfov=" << vfov << "\n";

  auto cameraObj =
      std::make_shared<raytracer::components::camera::perspective::Perspective>(
          position, target, upVec, vfov, aspectRatio);
  builder.addCamera(std::move(cameraObj));
}

bool AssimpLoader::load(const std::string& path, SceneBuilder& builder,
                        math::RenderSettings& settings) {
  Assimp::Importer importer;
  const aiScene* scene =
      importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |
                                  aiProcess_GenSmoothNormals);

  if (scene == nullptr || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0 ||
      scene->mRootNode == nullptr) {
    std::cerr << "Failed to load GLTF: " << importer.GetErrorString() << '\n';
    return false;
  }

  std::cerr << "Loaded GLTF: " << scene->mNumMeshes << " meshes, "
            << scene->mNumMaterials << " materials, " << scene->mNumCameras
            << " cameras, " << scene->mNumLights << " lights" << '\n';

  std::vector<std::shared_ptr<IMaterial>> materials = loadMaterials(scene);
  processNode(scene->mRootNode, scene, builder, aiMatrix4x4(), materials);
  processLights(scene, builder);
  processCamera(scene, builder, settings);
  builder.setBackground(
      std::make_shared<raytracer::scene::background::SolidBackground>(
          math::Color{1, 1, 1}));
  return true;
}

[[nodiscard]] bool AssimpLoader::supports(const std::string& ext) const {
  return ext == "obj" || ext == "fbx" || ext == "dae" || ext == "glb" ||
         ext == "gltf";
}
}  // namespace raytracer::scene
