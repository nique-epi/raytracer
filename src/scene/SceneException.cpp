/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SceneException
*/

#include "SceneException.hpp"
#include <string>
#include "constants/Errors.hpp"

namespace raytracer::scene {

SceneFileNotFoundException::SceneFileNotFoundException(const std::string& path)
    : SceneLoaderException(
          std::string(raytracer::constants::errors::SCENE_FILE_NOT_FOUND) +
          ": '" + path + "'") {}

SceneParseException::SceneParseException(const std::string& path,
                                         const std::string& reason)
    : SceneLoaderException(
          std::string(raytracer::constants::errors::SCENE_PARSE_ERROR) +
          " in '" + path + "': " + reason) {}

SceneParseException::SceneParseException(const std::string& path, int line,
                                         const std::string& reason)
    : SceneLoaderException(
          std::string(raytracer::constants::errors::SCENE_PARSE_ERROR) +
          " in '" + path + "':" + std::to_string(line) + ": " + reason) {}

}  // namespace raytracer::scene
