/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** TransformationFactory
*/

#include "TransformationFactory.hpp"
#include <libconfig.h++>
#include <memory>
#include <string>
#include "components/Transformations/Rotation/Rotation.hpp"
#include "components/Transformations/Translation/Translation.hpp"
#include "exceptions/Exceptions.hpp"
#include "utils/math/Vector3D.hpp"

namespace raytracer::core::factory {

namespace {

using raytracer::components::transformation::Rotation;
using raytracer::components::transformation::Translation;
using raytracer::math::Vector3D;

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

std::shared_ptr<ITransformation> createTranslationFromCfg(
    const libconfig::Setting& cfg) {
  Vector3D offset{0.0, 0.0, 0.0};
  overrideVec3IfPresent(cfg, "offset", offset);
  return TransformationFactory::createTranslation(offset);
}

std::shared_ptr<ITransformation> createRotationFromCfg(
    const libconfig::Setting& cfg) {
  Vector3D axis{0.0, 0.0, 1.0};
  double angle = 0.0;
  overrideVec3IfPresent(cfg, "axis", axis);
  cfg.lookupValue("angle", angle);
  return TransformationFactory::createRotation(axis, angle);
}

}  // namespace

std::shared_ptr<ITransformation> TransformationFactory::createTranslation(
    const math::Vector3D& offset) {
  return std::make_shared<Translation>(offset);
}

std::shared_ptr<ITransformation> TransformationFactory::createRotation(
    const math::Vector3D& axis, double angle) {
  return std::make_shared<Rotation>(axis, angle);
}

std::shared_ptr<ITransformation> TransformationFactory::create(
    const std::string& type, const libconfig::Setting& cfg) {
  if (type == "translation") {
    return createTranslationFromCfg(cfg);
  }
  if (type == "rotation") {
    return createRotationFromCfg(cfg);
  }
  throw RaytracerException(
      "TransformationFactory: unknown transformation type '" + type + "'");
}

}  // namespace raytracer::core::factory
