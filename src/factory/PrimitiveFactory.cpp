/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PrimitiveFactory
*/

#include "PrimitiveFactory.hpp"
#include <libconfig.h++>
#include <memory>
#include <string>
#include <utility>
#include "components/Primitives/cone/Cone.hpp"
#include "components/Primitives/cylinder/Cylinder.hpp"
#include "components/Primitives/plane/Plane.hpp"
#include "components/Primitives/sphere/Sphere.hpp"
#include "exceptions/Exceptions.hpp"
#include "utils/math/Vector3D.hpp"

namespace raytracer::core::factory {

namespace {

using raytracer::components::primitives::Cone;
using raytracer::components::primitives::Cylinder;
using raytracer::components::primitives::Plane;
using raytracer::components::primitives::Sphere;
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

Vector3D axisVectorFromName(const std::string& name) {
  if (name == "X" || name == "x") {
    return {1.0, 0.0, 0.0};
  }
  if (name == "Y" || name == "y") {
    return {0.0, 1.0, 0.0};
  }
  if (name == "Z" || name == "z") {
    return {0.0, 0.0, 1.0};
  }
  return {0.0, 1.0, 0.0};
}

std::shared_ptr<IObject> createSphereFromCfg(const libconfig::Setting& cfg) {
  Vector3D center{0.0, 0.0, 0.0};
  double radius = 1.0;
  overrideVec3IfPresent(cfg, "center", center);
  cfg.lookupValue("radius", radius);
  return PrimitiveFactory::createSphere(center, radius);
}

std::shared_ptr<IObject> createConeFromCfg(const libconfig::Setting& cfg) {
  Vector3D apex{0.0, 0.0, 0.0};
  Vector3D axis{0.0, 1.0, 0.0};
  double angle = defaultConeAngle;
  double height = 1.0;
  overrideVec3IfPresent(cfg, "apex", apex);
  overrideVec3IfPresent(cfg, "axis", axis);
  cfg.lookupValue("angle", angle);
  cfg.lookupValue("height", height);
  return PrimitiveFactory::createCone(apex, axis, angle, height);
}

std::shared_ptr<IObject> createCylinderFromCfg(const libconfig::Setting& cfg) {
  Vector3D center{0.0, 0.0, 0.0};
  Vector3D axis{0.0, 1.0, 0.0};
  double radius = 1.0;
  double height = 1.0;
  overrideVec3IfPresent(cfg, "center", center);
  overrideVec3IfPresent(cfg, "axis", axis);
  cfg.lookupValue("radius", radius);
  cfg.lookupValue("height", height);
  return PrimitiveFactory::createCylinder(center, axis, radius, height);
}

std::shared_ptr<IObject> createPlaneFromCfg(const libconfig::Setting& cfg) {
  Vector3D point{0.0, 0.0, 0.0};
  Vector3D normal{0.0, 1.0, 0.0};
  overrideVec3IfPresent(cfg, "point", point);
  overrideVec3IfPresent(cfg, "normal", normal);
  if (cfg.exists("axis") && cfg.exists("position")) {
    std::string axisName;
    double position = 0.0;
    cfg.lookupValue("axis", axisName);
    cfg.lookupValue("position", position);
    const Vector3D axisVec = axisVectorFromName(axisName);
    point = axisVec * position;
    normal = axisVec;
  }
  return PrimitiveFactory::createPlane(point, normal);
}

}  // namespace

std::shared_ptr<IObject> PrimitiveFactory::createSphere(
    const math::Vector3D& center, double radius,
    std::shared_ptr<IMaterial> material) {
  return std::make_shared<Sphere>(center, radius, std::move(material));
}

std::shared_ptr<IObject> PrimitiveFactory::createCone(
    const math::Vector3D& apex, const math::Vector3D& axis, double angle,
    double height, std::shared_ptr<IMaterial> material) {
  return std::make_shared<Cone>(apex, axis, angle, height,
                                std::move(material));
}

std::shared_ptr<IObject> PrimitiveFactory::createCylinder(
    const math::Vector3D& center, const math::Vector3D& axis, double radius,
    double height, std::shared_ptr<IMaterial> material) {
  return std::make_shared<Cylinder>(center, axis, radius, height,
                                    std::move(material));
}

std::shared_ptr<IObject> PrimitiveFactory::createPlane(
    const math::Vector3D& point, const math::Vector3D& normal,
    std::shared_ptr<IMaterial> material) {
  return std::make_shared<Plane>(point, normal, std::move(material));
}

std::shared_ptr<IObject> PrimitiveFactory::create(
    const std::string& type, const libconfig::Setting& cfg) {
  if (type == "sphere") {
    return createSphereFromCfg(cfg);
  }
  if (type == "cone") {
    return createConeFromCfg(cfg);
  }
  if (type == "cylinder") {
    return createCylinderFromCfg(cfg);
  }
  if (type == "plane") {
    return createPlaneFromCfg(cfg);
  }
  throw RaytracerException(
      "PrimitiveFactory: unknown primitive type '" + type + "'");
}

}  // namespace raytracer::core::factory
