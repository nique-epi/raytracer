/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** CFGSceneLoader
*/

#pragma once
#include <libconfig.h++>
#include "ISceneLoader.hpp"

namespace raytracer::scene {

class CFGSceneLoader : public ISceneLoader {
 public:
  bool load(const std::string& path, SceneBuilder& builder,
            math::RenderSettings& settings) override;
  [[nodiscard]] bool supports(const std::string& ext) const override;

 private:
  static void parsePrimitives(const libconfig::Setting& root,
                              SceneBuilder& builder);
  static void parseLights(const libconfig::Setting& root,
                          SceneBuilder& builder);
  static void parseCamera(const libconfig::Setting& root,
                          SceneBuilder& builder);
  static void parseSettings(const libconfig::Setting& root,
                            math::RenderSettings& settings);
};

}  // namespace raytracer::scene
