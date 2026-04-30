/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** CFGSceneLoader
*/

#include "CFGSceneLoader.hpp"
#include <iostream>
#include <libconfig.h++>
#include "SceneBuilder.hpp"

namespace {

void addList(const libconfig::Setting& parent, const char* key,
             const char* type, raytracer::scene::SceneBuilder& builder) {
  if (!parent.exists(key)) {
    return;
  }
  const auto& list = parent[key];
  for (int i = 0; i < list.getLength(); ++i) {
    builder.addObject(type, list[i]);
  }
}

}  // namespace

namespace raytracer::scene {

bool CFGSceneLoader::supports(const std::string& ext) const {
  return ext == "cfg";
}

bool CFGSceneLoader::load(const std::string& path, SceneBuilder& builder,
                          math::RenderSettings& settings) {
  libconfig::Config cfg;

  try {
    cfg.readFile(path.c_str());
  } catch (const libconfig::FileIOException&) {
    std::cerr << "I/O error while reading file: " << path << "\n";
    return false;
  } catch (const libconfig::ParseException& pex) {
    std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
              << " - " << pex.getError() << "\n";
    return false;
  }

  const libconfig::Setting& root = cfg.getRoot();
  parsePrimitives(root, builder);
  parseLights(root, builder);
  parseCamera(root, builder);
  parseSettings(root, settings);
  return true;
}

void CFGSceneLoader::parsePrimitives(const libconfig::Setting& root,
                                     SceneBuilder& builder) {
  if (!root.exists("primitives")) {
    return;
  }
  const auto& p = root["primitives"];
  addList(p, "spheres", "sphere", builder);
  addList(p, "planes", "plane", builder);
}

void CFGSceneLoader::parseLights(const libconfig::Setting& root,
                                 SceneBuilder& builder) {
  if (!root.exists("lights")) {
    return;
  }
  const auto& l = root["lights"];
  addList(l, "ambient", "ambient", builder);
  addList(l, "directional", "directional", builder);
}

void CFGSceneLoader::parseCamera(const libconfig::Setting& root,
                                 SceneBuilder& builder) {
  if (!root.exists("camera")) {
    return;
  }
  builder.addObject("camera", root["camera"]);
}

void CFGSceneLoader::parseSettings(const libconfig::Setting& root,
                                   math::RenderSettings& settings) {
  if (!root.exists("settings")) {
    return;
  }
  const auto& s = root["settings"];
  s.lookupValue("imageWidth", settings.imageWidth);
  s.lookupValue("imageHeight", settings.imageHeight);
  s.lookupValue("tileWidth", settings.tileWidth);
  s.lookupValue("tileHeight", settings.tileHeight);
  s.lookupValue("numThreads", settings.numThreads);
  s.lookupValue("samplesPerPixel", settings.samplesPerPixel);
  s.lookupValue("maxDepth", settings.maxDepth);
}

}  // namespace raytracer::scene
