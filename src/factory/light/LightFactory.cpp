/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** LightFactory
*/

#include "LightFactory.hpp"
#include <libconfig.h++>
#include <memory>
#include <string>
#include "components/light/ambient/AmbientLight.hpp"
#include "components/light/directional/Directional.hpp"
#include "components/light/point/Point.hpp"
#include "exceptions/Exceptions.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Vector3D.hpp"

namespace raytracer::core::factory {

namespace {

using raytracer::components::light::ambient::Ambient;
using raytracer::components::light::directional::Directional;
using raytracer::components::light::point::PointLight;
using raytracer::math::Color;
using raytracer::math::Vector3D;

constexpr double colorChannelMaximum = 255.0;

Vector3D parseVec3OrDefault(const libconfig::Setting& s,
                            const Vector3D& fallback) {
  double x = fallback.x;
  double y = fallback.y;
  double z = fallback.z;
  s.lookupValue("x", x);
  s.lookupValue("y", y);
  s.lookupValue("z", z);
  return {x, y, z};
}

void overrideVec3IfPresent(const libconfig::Setting& cfg, const char* key,
                           Vector3D& target) {
  if (cfg.exists(key)) {
    target = parseVec3OrDefault(cfg.lookup(key), target);
  }
}

Color parseColorOrDefault(const libconfig::Setting& s, const Color& fallback) {
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

void overrideColorIfPresent(const libconfig::Setting& cfg, const char* key,
                            Color& target) {
  if (cfg.exists(key)) {
    target = parseColorOrDefault(cfg.lookup(key), target);
  }
}

std::shared_ptr<ILight> createAmbientFromCfg(const libconfig::Setting& cfg) {
  Color color(1.0, 1.0, 1.0);
  double intensity = 1.0;
  overrideColorIfPresent(cfg, "color", color);
  cfg.lookupValue("intensity", intensity);
  return LightFactory::createAmbient(color, intensity);
}

std::shared_ptr<ILight> createDirectionalFromCfg(
    const libconfig::Setting& cfg) {
  Vector3D direction{0.0, -1.0, 0.0};
  Color color(1.0, 1.0, 1.0);
  double intensity = 1.0;
  overrideVec3IfPresent(cfg, "direction", direction);
  overrideColorIfPresent(cfg, "color", color);
  cfg.lookupValue("intensity", intensity);
  return LightFactory::createDirectional(direction, color, intensity);
}

std::shared_ptr<ILight> createPointFromCfg(const libconfig::Setting& cfg) {
  Vector3D position{0.0, 1.0, 0.0};
  Color color(1.0, 1.0, 1.0);
  double intensity = 1.0;
  overrideVec3IfPresent(cfg, "position", position);
  overrideColorIfPresent(cfg, "color", color);
  cfg.lookupValue("intensity", intensity);
  return LightFactory::createPoint(position, color, intensity);
}

}  // namespace

std::shared_ptr<ILight> LightFactory::createAmbient(const math::Color& color,
                                                    double intensity) {
  return std::make_shared<Ambient>(color, intensity);
}

std::shared_ptr<ILight> LightFactory::createDirectional(
    const math::Vector3D& direction, const math::Color& color,
    double intensity) {
  return std::make_shared<Directional>(direction, color, intensity);
}

std::shared_ptr<ILight> LightFactory::createPoint(
    const math::Vector3D& position, const math::Color& color,
    double intensity) {
  return std::make_shared<PointLight>(position, color, intensity);
}

std::shared_ptr<ILight> LightFactory::create(const std::string& type,
                                              const libconfig::Setting& cfg) {
  if (type == "ambient") {
    return createAmbientFromCfg(cfg);
  }
  if (type == "directional") {
    return createDirectionalFromCfg(cfg);
  }
  if (type == "point") {
    return createPointFromCfg(cfg);
  }
  throw RaytracerException("LightFactory: unknown light type '" + type + "'");
}

}  // namespace raytracer::core::factory
