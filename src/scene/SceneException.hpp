/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SceneException
*/

#ifndef SCENE_SCENEEXCEPTION_HPP_
#define SCENE_SCENEEXCEPTION_HPP_

#include <string>
#include "exceptions/Exceptions.hpp"

namespace raytracer::scene {

class SceneLoaderException : public raytracer::core::RaytracerException {
 public:
  using raytracer::core::RaytracerException::RaytracerException;
};

class SceneFileNotFoundException : public SceneLoaderException {
 public:
  explicit SceneFileNotFoundException(const std::string& path);
};

class SceneParseException : public SceneLoaderException {
 public:
  SceneParseException(const std::string& path, const std::string& reason);
  SceneParseException(const std::string& path, int line,
                      const std::string& reason);
};

class SceneBuildException : public raytracer::core::RaytracerException {
 public:
  using raytracer::core::RaytracerException::RaytracerException;
};

}  // namespace raytracer::scene

#endif  // SCENE_SCENEEXCEPTION_HPP_
