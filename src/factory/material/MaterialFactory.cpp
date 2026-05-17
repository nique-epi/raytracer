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
#include "components/material/Textures/image/ImageTexture.hpp"
#include "components/material/Textures/noise/NoiseTexture.hpp"
#include "components/material/Textures/solid/SolidColor.hpp"
#include "components/material/diffuse/DiffuseMaterial.hpp"
#include "components/material/glass/Glass.hpp"
#include "components/material/glossy/Glossy.hpp"
#include "components/material/principledBSDF/PrincipledBSDF.hpp"
#include "components/material/textured/TexturedMaterial.hpp"
#include "exceptions/Exceptions.hpp"
#include "utils/math/Color.hpp"

namespace raytracer::core::factory {

namespace {

using raytracer::components::material::DiffuseMaterial;
using raytracer::components::material::Glass;
using raytracer::components::material::Glossy;
using raytracer::components::material::PrincipledMaterial;
using raytracer::components::material::TexturedMaterial;
using raytracer::materials::ITexture;
using raytracer::materials::textures::CheckerTexture;
using raytracer::materials::textures::ImageTexture;
using raytracer::materials::textures::NoiseTexture;
using raytracer::materials::textures::SolidColor;
using raytracer::math::Color;

void overrideColorIfPresent(const libconfig::Setting& cfg, const char* key,
                            Color& target) {
  if (cfg.exists(key)) {
    target = MaterialFactory::parseColor(cfg.lookup(key), target);
  }
}

void parsePhongParams(const libconfig::Setting& cfg, Color& specularAlbedo,
                      double& shininess) {
  overrideColorIfPresent(cfg, "specular", specularAlbedo);
  cfg.lookupValue("shininess", shininess);
}

std::shared_ptr<IMaterial> createDiffuseFromCfg(const libconfig::Setting& cfg) {
  Color albedo(1.0, 1.0, 1.0);
  Color specularAlbedo(0.0, 0.0, 0.0);
  double shininess = 1.0;
  overrideColorIfPresent(cfg, "albedo", albedo);
  overrideColorIfPresent(cfg, "color", albedo);
  parsePhongParams(cfg, specularAlbedo, shininess);
  return MaterialFactory::createDiffuse(albedo, specularAlbedo, shininess);
}

std::shared_ptr<IMaterial> createGlossyFromCfg(const libconfig::Setting& cfg) {
  double fuzz = 0.0;
  Color albedo(1.0, 1.0, 1.0);
  Color specularAlbedo(0.0, 0.0, 0.0);
  double shininess = 1.0;
  cfg.lookupValue("fuzz", fuzz);
  overrideColorIfPresent(cfg, "albedo", albedo);
  overrideColorIfPresent(cfg, "color", albedo);
  parsePhongParams(cfg, specularAlbedo, shininess);
  return MaterialFactory::createGlossy(fuzz, albedo, specularAlbedo, shininess);
}

std::shared_ptr<IMaterial> createGlassFromCfg(const libconfig::Setting& cfg) {
  double refractionIndex = defaultGlassRefractionIndex;
  cfg.lookupValue("refractionIndex", refractionIndex);
  Color tint(1.0, 1.0, 1.0);
  Color specularAlbedo(0.0, 0.0, 0.0);
  double shininess = 1.0;
  overrideColorIfPresent(cfg, "tint", tint);
  parsePhongParams(cfg, specularAlbedo, shininess);
  return MaterialFactory::createGlass(refractionIndex, tint, specularAlbedo,
                                       shininess);
}

std::shared_ptr<IMaterial> createPrincipledFromCfg(
    const libconfig::Setting& cfg) {
  Color baseColor(1.0, 1.0, 1.0);
  double metallic = 0.0;
  double roughness = defaultPrincipledRoughness;
  double ior = defaultPrincipledRefractionIndex;
  double alpha = 1.0;
  overrideColorIfPresent(cfg, "baseColor", baseColor);
  cfg.lookupValue("metallic", metallic);
  cfg.lookupValue("roughness", roughness);
  cfg.lookupValue("ior", ior);
  cfg.lookupValue("alpha", alpha);
  return MaterialFactory::createPrincipled(baseColor, metallic, roughness, ior,
                                           alpha);
}

}  // namespace

std::shared_ptr<IMaterial> MaterialFactory::createDiffuse(
    const math::Color& albedo, const math::Color& specularAlbedo,
    double shininess) {
  return std::make_shared<DiffuseMaterial>(albedo, specularAlbedo, shininess);
}

std::shared_ptr<IMaterial> MaterialFactory::createGlossy(
    double fuzz, const math::Color& albedo,
    const math::Color& specularAlbedo, double shininess) {
  return std::make_shared<Glossy>(fuzz, albedo, specularAlbedo, shininess);
}

std::shared_ptr<IMaterial> MaterialFactory::createGlass(
    double refractionIndex, const math::Color& tint,
    const math::Color& specularAlbedo, double shininess) {
  return std::make_shared<Glass>(refractionIndex, tint, specularAlbedo,
                                  shininess);
}

std::shared_ptr<IMaterial> MaterialFactory::createPrincipled(
    const math::Color& baseColor, double metallic, double roughness, double ior,
    double alpha) {
  return std::make_shared<PrincipledMaterial>(baseColor, metallic, roughness,
                                              ior, alpha);
}

// NOLINTNEXTLINE(misc-use-internal-linkage)
std::shared_ptr<IMaterial> MaterialFactory::createTextured(
    std::shared_ptr<raytracer::materials::ITexture> texture,
    const math::Color& albedo, const math::Color& specularAlbedo,
    double shininess) {
  return std::make_shared<TexturedMaterial>(std::move(texture), albedo,
                                             specularAlbedo, shininess);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
std::shared_ptr<raytracer::materials::ITexture> MaterialFactory::parseTexture(
    const libconfig::Setting& cfg) {
  std::string type;
  if (!cfg.lookupValue("type", type)) {
    throw RaytracerException(
        "MaterialFactory: missing required texture field 'type'");
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
  if (type == "bitmap") {
    std::string path;
    if (!cfg.lookupValue("path", path)) {
      throw RaytracerException(
          "MaterialFactory: bitmap texture requires a 'path' field");
    }
    auto texture = std::make_shared<ImageTexture>();
    if (!texture->loadFromFile(path)) {
      throw RaytracerException(
          "MaterialFactory: failed to load bitmap texture from '" + path + "'");
    }
    return texture;
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
  if (type == "principled") {
    return createPrincipledFromCfg(cfg);
  }
  if (type == "textured") {
    Color albedo(1.0, 1.0, 1.0);
    Color specularAlbedo(0.0, 0.0, 0.0);
    double shininess = 1.0;
    overrideColorIfPresent(cfg, "color", albedo);
    overrideColorIfPresent(cfg, "albedo", albedo);
    parsePhongParams(cfg, specularAlbedo, shininess);
    const std::shared_ptr<ITexture> texture =
        cfg.exists("texture")
            ? parseTexture(cfg.lookup("texture"))
            : std::make_shared<SolidColor>(Color(1.0, 1.0, 1.0));
    return createTextured(texture, albedo, specularAlbedo, shininess);
  }
  throw RaytracerException("MaterialFactory: unknown material type '" + type +
                           "'");
}

}  // namespace raytracer::core::factory
