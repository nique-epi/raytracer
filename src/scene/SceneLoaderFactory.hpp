/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SceneLoaderFactory
*/

#pragma once
#include <memory>
#include <string>
#include <vector>
#include "ISceneLoader.hpp"

namespace raytracer::scene {

class SceneLoaderFactory {
 public:
  void registerLoader(std::shared_ptr<ISceneLoader> loader);
  [[nodiscard]] std::shared_ptr<ISceneLoader> getLoader(
      const std::string& path) const;

 private:
  std::vector<std::shared_ptr<ISceneLoader>> _loaders;
};

}  // namespace raytracer::scene
