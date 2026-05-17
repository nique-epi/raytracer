/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** JsonSettingsLoader
*/

#include "JsonSettingsLoader.hpp"
#include <algorithm>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <thread>
#include "scene/Scene.hpp"
#include "scene/SceneFileNotFoundException.hpp"
#include "scene/SceneParseException.hpp"
#include "scene/World.hpp"

namespace raytracer::bonus::json {

namespace {

raytracer::scene::ViewportMode viewportModeFromString(const std::string& mode) {
  if (mode == "wireframe") {
    return raytracer::scene::ViewportMode::Wireframe;
  }
  if (mode == "materialPreview") {
    return raytracer::scene::ViewportMode::MaterialPreview;
  }
  return raytracer::scene::ViewportMode::Rendered;
}

}  // namespace

JsonSettings JsonSettingsLoader::load(const std::string& path,
                                      const math::RenderSettings& base) {
  std::ifstream file(path);
  if (!file.is_open()) {
    throw raytracer::scene::SceneFileNotFoundException(path);
  }

  nlohmann::json j;
  try {
    file >> j;
    if (!j.is_object()) {
      throw raytracer::scene::SceneParseException(path,
                                                  "root must be a JSON object");
    }

    JsonSettings result;
    result.settings = base;
    math::RenderSettings& settings = result.settings;

    settings.imageWidth = j.value("imageWidth", settings.imageWidth);
    settings.imageHeight = j.value("imageHeight", settings.imageHeight);
    settings.tileWidth = j.value("tileWidth", settings.tileWidth);
    settings.tileHeight = j.value("tileHeight", settings.tileHeight);
    settings.numThreads = j.value("numThreads", settings.numThreads);
    if (settings.numThreads <= 0) {
      settings.numThreads =
          static_cast<int>(std::max(1U, std::thread::hardware_concurrency()));
    }
    settings.samplesPerPixel =
        j.value("samplesPerPixel", settings.samplesPerPixel);
    settings.maxDepth = j.value("maxDepth", settings.maxDepth);

    if (j.contains("viewportMode") && j.at("viewportMode").is_string()) {
      result.viewportMode =
          viewportModeFromString(j.at("viewportMode").get<std::string>());
    }
    if (j.contains("name") && j.at("name").is_string()) {
      result.outputFile = j.at("name").get<std::string>();
    }

    return result;
  } catch (const raytracer::scene::SceneParseException&) {
    throw;
  } catch (const nlohmann::json::exception& error) {
    throw raytracer::scene::SceneParseException(path, error.what());
  }
}

void JsonSettingsLoader::applyOverrides(raytracer::scene::Scene& scene,
                                        math::RenderSettings& settings,
                                        const JsonSettings& json,
                                        std::string& outputPath) {
  settings = json.settings;
  if (json.viewportMode) {
    scene.getWorld().setViewportMode(*json.viewportMode);
  }
  if (json.outputFile) {
    outputPath = *json.outputFile;
  }
}

}  // namespace raytracer::bonus::json
