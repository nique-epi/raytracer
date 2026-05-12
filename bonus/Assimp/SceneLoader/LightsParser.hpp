/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** LightParser
*/

#pragma once

#include <assimp/scene.h>
#include "scene/SceneBuilder.hpp"

namespace raytracer::scene {

class LightParser {
 public:
  static void parse(const aiScene* scene, SceneBuilder& builder);

 private:
  static float extractIntensity(const aiScene* scene, const aiString& name);
  static aiMatrix4x4 findWorldTransform(const aiScene* scene,
                                        const aiString& name);

  static void addDirectional(SceneBuilder& builder, const aiLight* light,
                             const aiMatrix4x4& worldTransform,
                             float intensity);
  static void addPoint(SceneBuilder& builder, const aiMatrix4x4& worldTransform,
                       const math::Color& color, float intensity);
  static void addAmbient(SceneBuilder& builder, const aiLight* light);
  static void addFallback(SceneBuilder& builder);
};

}  // namespace raytracer::scene
