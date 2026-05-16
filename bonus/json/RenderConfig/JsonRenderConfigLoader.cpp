/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** JsonRenderConfigLoader
*/

#include "JsonRenderConfigLoader.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include "scene/SceneFileNotFoundException.hpp"
#include "scene/SceneParseException.hpp"

namespace raytracer::bonus::json {

namespace {

raytracer::scene::ViewportMode viewportModeFromString(const std::string& mode) {
  if (mode == "wireframe") return raytracer::scene::ViewportMode::Wireframe;
  if (mode == "materialPreview")
    return raytracer::scene::ViewportMode::MaterialPreview;
  return raytracer::scene::ViewportMode::Rendered;
}

}  // namespace

GlobalRenderConfig JsonRenderConfigLoader::load(
    const std::string& path, const math::RenderSettings& base) {
  std::ifstream file(path);
  if (!file.is_open()) {
    throw raytracer::scene::SceneFileNotFoundException(path);
  }

  nlohmann::json j;
  try {
    file >> j;
  } catch (const nlohmann::json::parse_error& error) {
    throw raytracer::scene::SceneParseException(path, error.what());
  }

  GlobalRenderConfig config;
  config.settings = base;
  math::RenderSettings& settings = config.settings;

  settings.imageWidth = j.value("imageWidth", settings.imageWidth);
  settings.imageHeight = j.value("imageHeight", settings.imageHeight);
  settings.tileWidth = j.value("tileWidth", settings.tileWidth);
  settings.tileHeight = j.value("tileHeight", settings.tileHeight);
  settings.numThreads = j.value("numThreads", settings.numThreads);
  settings.samplesPerPixel =
      j.value("samplesPerPixel", settings.samplesPerPixel);
  settings.maxDepth = j.value("maxDepth", settings.maxDepth);

  if (j.contains("viewportMode") && j.at("viewportMode").is_string()) {
    config.viewportMode =
        viewportModeFromString(j.at("viewportMode").get<std::string>());
  }

  return config;
}

}  // namespace raytracer::bonus::json
