/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** LightParser
*/

#include "LightsParser.hpp"
#include <assimp/matrix3x3.h>
#include <iostream>
#include <memory>
#include <stack>
#include "components/light/ambient/AmbientLight.hpp"
#include "components/light/directional/Directional.hpp"
#include "components/light/point/Point.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Vector3D.hpp"

namespace {
namespace math = raytracer::math;

constexpr float fallbackDir = 0.5F;

math::Color toColor(const aiColor3D& c) { return {c.r, c.g, c.b}; }
}  // namespace

namespace raytracer::scene {

void LightParser::parse(const aiScene* scene, SceneBuilder& builder) {
  bool hasLight = false;

  for (unsigned int i = 0; i < scene->mNumLights; ++i) {
    const aiLight* light = scene->mLights[i];
    const math::Color color = toColor(light->mColorDiffuse);
    const float intensity = extractIntensity(scene, light->mName);
    const aiMatrix4x4 worldTransform = findWorldTransform(scene, light->mName);
    std::cerr << "[AssimpLightParser] Parsing Assimp light '"
              << light->mName.C_Str() << "' (type=" << light->mType
              << ", intensity=" << intensity << ")" << '\n';

    switch (light->mType) {
      case aiLightSource_DIRECTIONAL:
        addDirectional(builder, light, worldTransform, intensity);
        break;
      case aiLightSource_POINT:
        addPoint(builder, worldTransform, color, intensity);
        break;
      case aiLightSource_AMBIENT:
        addAmbient(builder, light);
        break;
      default:
        addFallback(builder);
        break;
    }

    hasLight = true;
  }

  if (!hasLight) {
    builder.addLight(std::make_shared<components::light::ambient::Ambient>(
        math::Color(1.0, 1.0, 1.0), 1.0));
  }
}

float LightParser::extractIntensity(const aiScene* scene,
                                    const aiString& name) {
  struct Entry {
    aiNode* node;
  };
  std::stack<Entry> stack;
  stack.push({scene->mRootNode});

  while (!stack.empty()) {
    aiNode* node = stack.top().node;
    stack.pop();

    if (node->mName == name && node->mMetaData != nullptr) {
      float intensity = 1.0F;
      if (node->mMetaData->Get("intensity", intensity)) {
        return intensity;
      }

      double intensityD = 1.0;
      if (node->mMetaData->Get("intensity", intensityD)) {
        return static_cast<float>(intensityD);
      }
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
      stack.push({node->mChildren[i]});
    }
  }
  return 1.0F;
}

aiMatrix4x4 LightParser::findWorldTransform(const aiScene* scene,
                                            const aiString& name) {
  struct Entry {
    aiNode* node;
    aiMatrix4x4 transform;
  };
  std::stack<Entry> stack;
  stack.push({scene->mRootNode, aiMatrix4x4()});

  while (!stack.empty()) {
    auto [node, parentTransform] = stack.top();
    stack.pop();
    aiMatrix4x4 worldTransform = parentTransform * node->mTransformation;

    if (node->mName == name) {
      return worldTransform;
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
      stack.push({node->mChildren[i], worldTransform});
    }
  }
  // NOLINTNEXTLINE(modernize-return-braced-init-list)
  return aiMatrix4x4();
}

void LightParser::addDirectional(SceneBuilder& builder, const aiLight* light,
                                 const aiMatrix4x4& worldTransform,
                                 float intensity) {
  aiMatrix3x3 rotMat(worldTransform);
  aiVector3D worldDir = rotMat * light->mDirection;
  worldDir.Normalize();
  std::cerr << "[AssimpLightParser] Created Assimp directional light '"
            << light->mName.C_Str() << "' direction=(" << worldDir.x << ", "
            << worldDir.y << ", " << worldDir.z << ")"
            << ", color=(" << light->mColorDiffuse.r << ", "
            << light->mColorDiffuse.g << ", " << light->mColorDiffuse.b << ")"
            << ", intensity=" << intensity << '\n';

  builder.addLight(
      std::make_shared<components::light::directional::Directional>(
          math::Vector3D(worldDir.x, worldDir.y, worldDir.z),
          toColor(light->mColorDiffuse), static_cast<double>(intensity)));
}

void LightParser::addPoint(SceneBuilder& builder,
                           const aiMatrix4x4& worldTransform,
                           const math::Color& color, float intensity) {
  aiVector3D pos;
  aiVector3D scale;
  aiQuaternion rot;
  worldTransform.Decompose(scale, rot, pos);
  std::cerr << "[AssimpLightParser] Created Assimp point light at (" << pos.x
            << ", " << pos.y << ", " << pos.z << ")"
            << ", color=(" << color.r << ", " << color.g << ", " << color.b
            << ")" << ", intensity=" << intensity << '\n';

  builder.addLight(std::make_shared<components::light::point::PointLight>(
      math::Point3D(pos.x, pos.y, pos.z), color,
      static_cast<double>(intensity)));
}

void LightParser::addAmbient(SceneBuilder& builder, const aiLight* light) {
  std::cerr << "[AssimpLightParser] Created Assimp ambient light '"
            << light->mName.C_Str() << "' color=(" << light->mColorAmbient.r
            << ", " << light->mColorAmbient.g << ", " << light->mColorAmbient.b
            << ")" << '\n';
  builder.addLight(std::make_shared<components::light::ambient::Ambient>(
      toColor(light->mColorAmbient), 1.0));
}

void LightParser::addFallback(SceneBuilder& builder) {
  std::cerr << "[AssimpLightParser] Created Assimp fallback directional light"
            << " direction=(" << -fallbackDir << ", -1, " << -fallbackDir << ")"
            << '\n';
  builder.addLight(
      std::make_shared<components::light::directional::Directional>(
          math::Vector3D(-fallbackDir, -1.0, -fallbackDir),
          math::Color(1.0, 1.0, 1.0), 1.0));
}

}  // namespace raytracer::scene
