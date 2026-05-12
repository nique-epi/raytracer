/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MaterialFactory
*/

#include "MaterialFactory.hpp"
#include <libconfig.h++>
#include <memory>
#include <string>
#include "components/material/Textures/ITexture.hpp"
#include "components/material/Textures/checker/CheckerTexture.hpp"
#include "components/material/Textures/noise/NoiseTexture.hpp"
#include "components/material/Textures/solid/SolidColor.hpp"
#include "components/material/diffuse/DiffuseMaterial.hpp"
#include "components/material/glass/Glass.hpp"
#include "components/material/glossy/Glossy.hpp"
#include "components/material/textured/TexturedMaterial.hpp"
#include "exceptions/Exceptions.hpp"
#include "utils/math/Color.hpp"

namespace raytracer::core::factory {

namespace {

using raytracer::components::material::DiffuseMaterial;
using raytracer::components::material::Glass;
using raytracer::components::material::Glossy;
using raytracer::components::material::TexturedMaterial;
using raytracer::materials::ITexture;
using raytracer::materials::textures::CheckerTexture;
using raytracer::materials::textures::NoiseTexture;
using raytracer::materials::textures::SolidColor;
using raytracer::math::Color;

void overrideColorIfPresent(const libconfig::Setting& cfg, const char* key,
                            Color& target) {
  if (cfg.exists(key)) {
    target = MaterialFactory::parseColor(cfg.lookup(key), target);
  }
}

std::shared_ptr<IMaterial> createDiffuseFromCfg(const libconfig::Setting& cfg) {
  Color albedo(1.0, 1.0, 1.0);
  overrideColorIfPresent(cfg, "albedo", albedo);
  overrideColorIfPresent(cfg, "color", albedo);
  return MaterialFactory::createDiffuse(albedo);
}

std::shared_ptr<IMaterial> createGlossyFromCfg(const libconfig::Setting& cfg) {
  double fuzz = 0.0;
  Color albedo(1.0, 1.0, 1.0);
  cfg.lookupValue("fuzz", fuzz);
  overrideColorIfPresent(cfg, "albedo", albedo);
  overrideColorIfPresent(cfg, "color", albedo);
  return MaterialFactory::createGlossy(fuzz, albedo);
}

std::shared_ptr<IMaterial> createGlassFromCfg(const libconfig::Setting& cfg) {
  double refractionIndex = defaultGlassRefractionIndex;
  cfg.lookupValue("refractionIndex", refractionIndex);
  return MaterialFactory::createGlass(refractionIndex);
}

}  // namespace

std::shared_ptr<IMaterial> MaterialFactory::createDiffuse(
    const math::Color& albedo) {
  return std::make_shared<DiffuseMaterial>(albedo);
}

std::shared_ptr<IMaterial> MaterialFactory::createGlossy(
    double fuzz, const math::Color& albedo) {
  return std::make_shared<Glossy>(fuzz, albedo);
}

std::shared_ptr<IMaterial> MaterialFactory::createGlass(
    double refractionIndex) {
  return std::make_shared<Glass>(refractionIndex);
}

// NOLINTNEXTLINE(misc-use-internal-linkage)
std::shared_ptr<IMaterial> MaterialFactory::createTextured(
    std::shared_ptr<raytracer::materials::ITexture> texture,
    const math::Color& albedo) {
  return std::make_shared<TexturedMaterial>(std::move(texture), albedo);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
std::shared_ptr<raytracer::materials::ITexture> MaterialFactory::parseTexture(
    const libconfig::Setting& cfg) {
  std::string type;
  if (!cfg.lookupValue("type", type)) {
    throw RaytracerException("MaterialFactory: missing required texture field 'type'");
  }

  if (type == "solid") {
    Color color(1.0, 1.0, 1.0);
    overrideColorIfPresent(cfg, "color", color);
    return std::make_shared<SolidColor>(color);
  }
  if (type == "checker") {
    Color odd(0.0, 0.0, 0.0);
    Color even(1.0, 1.0, 1.0);
    double scale = 1.0;
    overrideColorIfPresent(cfg, "odd", odd);
    overrideColorIfPresent(cfg, "even", even);
    cfg.lookupValue("scale", scale);
    return std::make_shared<CheckerTexture>(odd, even, scale);
  }
  if (type == "noise") {
    return std::make_shared<NoiseTexture>();
  }
  throw RaytracerException("MaterialFactory: unknown texture type '" + type +
                           "'");
}

math::Color MaterialFactory::parseColor(const libconfig::Setting& s,
                                        const math::Color& fallback) {
  constexpr double colorChannelMaximum = 255.0;
  int rRaw = static_cast<int>(fallback.r * colorChannelMaximum);
  int gRaw = static_cast<int>(fallback.g * colorChannelMaximum);
  int bRaw = static_cast<int>(fallback.b * colorChannelMaximum);
  s.lookupValue("r", rRaw);
  s.lookupValue("g", gRaw);
  s.lookupValue("b", bRaw);
  return {static_cast<double>(rRaw) / colorChannelMaximum,
          static_cast<double>(gRaw) / colorChannelMaximum,
          static_cast<double>(bRaw) / colorChannelMaximum};
}

std::shared_ptr<IMaterial> MaterialFactory::create(
    const std::string& type, const libconfig::Setting& cfg) {
  if (type == "diffuse") {
    return createDiffuseFromCfg(cfg);
  }
  if (type == "glossy") {
    return createGlossyFromCfg(cfg);
  }
  if (type == "glass") {
    return createGlassFromCfg(cfg);
  }
  if (type == "textured") {
    Color albedo(1.0, 1.0, 1.0);
    overrideColorIfPresent(cfg, "albedo", albedo);
    const std::shared_ptr<ITexture> texture =
        cfg.exists("texture") ? parseTexture(cfg.lookup("texture"))
                              : std::make_shared<SolidColor>(albedo);
    return createTextured(texture, albedo);
  }
  throw RaytracerException("MaterialFactory: unknown material type '" + type +
                           "'");
}

}  // namespace raytracer::core::factory
