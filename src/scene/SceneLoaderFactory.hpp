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

/**
 * @brief Dispatches scene-loader requests to the right concrete ISceneLoader
 *        based on file extension.
 *
 * Seul Factory légitime du projet — les autres classes similaires (qui
 * stockent des handlers indexés par clé sans en construire de nouveaux) sont
 * des Registry<T>. Le terme "Factory" est assumé ici car, du point de vue de
 * l'appelant, getLoader(path) renvoie un loader prêt à l'emploi à partir
 * d'un discriminant (l'extension), sans que le caller sache s'il provient
 * d'un cache ou d'une nouvelle instance — c'est l'intention sémantique
 * d'une fabrique côté API publique.
 */
class SceneLoaderFactory {
 public:
  /**
   * @brief Register a loader implementation.
   *
   * The loader is appended to the internal list. On lookup, the first
   * registered loader whose supports() matches wins, so registration order
   * also defines priority.
   *
   * @param [in] loader Concrete loader implementation to register.
   */
  void registerLoader(std::shared_ptr<ISceneLoader> loader);

  /**
   * @brief Find a registered loader able to handle the given path.
   *
   * The path's extension (the substring after the last '.') is extracted
   * and tested against each registered loader's supports().
   *
   * @param [in] path Path to a scene file.
   * @returns The first matching loader.
   * @returns nullptr if @p path has no extension or no registered loader
   *                  reports supports() == true.
   */
  [[nodiscard]] std::shared_ptr<ISceneLoader> getLoader(
      const std::string& path) const;

 private:
  std::vector<std::shared_ptr<ISceneLoader>> _loaders;
};

}  // namespace raytracer::scene
