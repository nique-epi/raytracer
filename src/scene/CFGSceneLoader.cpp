/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** CFGSceneLoader
*/

#include "CFGSceneLoader.hpp"
#include <libconfig.h++>
#include "SceneBuilder.hpp"
#include "SceneFileNotFoundException.hpp"
#include "SceneParseException.hpp"
#include "background/Gradient/GradientBackground.hpp"  // NOLINT(misc-include-cleaner)
#include "background/Solid/SolidBackground.hpp"  // NOLINT(misc-include-cleaner)
#include "factory/material/MaterialFactory.hpp"

namespace {

using AddFn = void (raytracer::scene::SceneBuilder::*)(
    const std::string&, const libconfig::Setting&);

void addList(const libconfig::Setting& parent, const char* key,
             const char* type, raytracer::scene::SceneBuilder& builder,
             AddFn adder) {
  if (!parent.exists(key)) {
    return;
  }
  const auto& list = parent[key];
  for (int i = 0; i < list.getLength(); ++i) {
    (builder.*adder)(type, list[i]);
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
    throw SceneFileNotFoundException(path);
  } catch (const libconfig::ParseException& pex) {
    const std::string file =
        ((pex.getFile() != nullptr) && pex.getFile()[0] != '\0') ? pex.getFile()
                                                                 : path;
    throw SceneParseException(file, pex.getLine(), pex.getError());
  }

  const libconfig::Setting& root = cfg.getRoot();
  parsePrimitives(root, builder);
  parseLights(root, builder);
  parseCamera(root, builder);
  parseSettings(root, settings);
  parseBackground(root, builder);
  return true;
}

void CFGSceneLoader::parsePrimitives(const libconfig::Setting& root,
                                     SceneBuilder& builder) {
  if (!root.exists("primitives")) {
    return;
  }
  const auto& p = root["primitives"];
  addList(p, "spheres", "sphere", builder, &SceneBuilder::addObject);
  addList(p, "planes", "plane", builder, &SceneBuilder::addObject);
  addList(p, "cylinders", "cylinder", builder, &SceneBuilder::addObject);
  addList(p, "cones", "cone", builder, &SceneBuilder::addObject);
}

void CFGSceneLoader::parseLights(const libconfig::Setting& root,
                                 SceneBuilder& builder) {
  if (!root.exists("lights")) {
    return;
  }
  const auto& l = root["lights"];
  addList(l, "ambient", "ambient", builder, &SceneBuilder::addLight);
  addList(l, "directional", "directional", builder, &SceneBuilder::addLight);
  addList(l, "point", "point", builder, &SceneBuilder::addLight);
}

void CFGSceneLoader::parseCamera(const libconfig::Setting& root,
                                 SceneBuilder& builder) {
  if (!root.exists("camera")) {
    return;
  }
  builder.addCamera(root["camera"]);
}

void CFGSceneLoader::parseBackground(const libconfig::Setting& root,
                                     SceneBuilder& builder) {
  using core::factory::MaterialFactory;

  if (!root.exists("background")) {
    builder.setBackground(
        std::make_shared<background::SolidBackground>(math::Color{0, 0, 0}));
    return;
  }

  const auto& bg = root["background"];
  std::string type = "solid";
  bg.lookupValue("type", type);

  if (type == "gradient") {
    const math::Color topColor =
        bg.exists("topColor")
            ? MaterialFactory::parseColor(bg["topColor"], {1, 1, 1})
            : math::Color{1, 1, 1};
    const math::Color bottomColor =
        bg.exists("bottomColor")
            ? MaterialFactory::parseColor(bg["bottomColor"], {0, 0, 0})
            : math::Color{0, 0, 0};
    builder.setBackground(std::make_shared<background::GradientBackground>(
        topColor, bottomColor));
    return;
  }

  const math::Color color =
      bg.exists("color") ? MaterialFactory::parseColor(bg["color"], {0, 0, 0})
                         : math::Color{0, 0, 0};
  builder.setBackground(std::make_shared<background::SolidBackground>(color));
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
