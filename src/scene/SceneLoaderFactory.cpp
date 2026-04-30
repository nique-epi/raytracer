/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SceneLoaderFactory
*/

#include "SceneLoaderFactory.hpp"

namespace raytracer::scene {

void SceneLoaderFactory::registerLoader(std::shared_ptr<ISceneLoader> loader) {
  _loaders.push_back(std::move(loader));
}

std::shared_ptr<ISceneLoader> SceneLoaderFactory::getLoader(
    const std::string& path) const {
  const auto dot = path.rfind('.');
  if (dot == std::string::npos) {
    return nullptr;
  }

  const std::string ext = path.substr(dot + 1);

  for (const auto& loader : _loaders) {
    if (loader->supports(ext)) {
      return loader;
    }
  }
  return nullptr;
}

}  // namespace raytracer::scene
