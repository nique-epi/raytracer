/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SceneFileNotFoundException
*/

#include "SceneFileNotFoundException.hpp"
#include <string>
#include "constants/Errors.hpp"

namespace raytracer::scene {

SceneFileNotFoundException::SceneFileNotFoundException(const std::string& path)
    : SceneLoaderException(
          std::string(raytracer::constants::errors::SCENE_FILE_NOT_FOUND) +
          ": '" + path + "'") {}

}  // namespace raytracer::scene
