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
};

}  // namespace raytracer::scene
