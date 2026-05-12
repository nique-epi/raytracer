/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** CameraParser
*/

#pragma once

#include <assimp/scene.h>
#include "scene/SceneBuilder.hpp"
#include "utils/math/RenderSettings.hpp"

namespace raytracer::scene {

class CameraParser {
 public:
  static void parse(const aiScene* scene, SceneBuilder& builder,
                    math::RenderSettings& settings);

 private:
  static aiMatrix4x4 buildCameraTransform(const aiScene* scene,
                                          const aiCamera* cam);
  static double computeVerticalFov(const aiCamera* cam, double aspectRatio);
  static void addCamera(SceneBuilder& builder, const aiCamera* cam,
                        const aiMatrix4x4& camTransform,
                        math::RenderSettings& settings);
};

}  // namespace raytracer::scene
