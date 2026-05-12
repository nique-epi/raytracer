/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SceneParseException
*/

#include "SceneParseException.hpp"
#include <string>
#include "constants/Errors.hpp"

namespace raytracer::scene {

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
