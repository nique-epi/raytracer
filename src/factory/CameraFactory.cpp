/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** CameraFactory
*/

#include "CameraFactory.hpp"
#include <libconfig.h++>
#include <memory>
#include <string>
#include "components/camera/perspective/PerspectiveCamera.hpp"
#include "exceptions/Exceptions.hpp"
#include "utils/math/Vector3D.hpp"

namespace raytracer::core::factory {

namespace {

using raytracer::components::camera::perspective::DEFAULT_FOV;
using raytracer::components::camera::perspective::Perspective;
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

std::shared_ptr<ICamera> createPerspectiveFromCfg(
    const libconfig::Setting& cfg) {
  Vector3D lookFrom{0.0, 0.0, 0.0};
  Vector3D lookAt{0.0, 0.0, -1.0};
  Vector3D up{0.0, 1.0, 0.0};
  double fov = DEFAULT_FOV;
  double aperture = 0.0;
  double focusDist = 1.0;

  overrideVec3IfPresent(cfg, "position", lookFrom);
  // `direction` is a relative vector from the camera; `lookAt` is absolute.
  // Whichever is present wins; if both, `lookAt` takes precedence.
  if (cfg.exists("direction")) {
    Vector3D direction{0.0, 0.0, -1.0};
    direction = parseVec3OrDefault(cfg.lookup("direction"), direction);
    lookAt = lookFrom + direction;
  }
  overrideVec3IfPresent(cfg, "lookAt", lookAt);
  overrideVec3IfPresent(cfg, "up", up);
  cfg.lookupValue("fov", fov);
  cfg.lookupValue("vfov", fov);
  cfg.lookupValue("aperture", aperture);
  cfg.lookupValue("focusDist", focusDist);

  return CameraFactory::createPerspective(lookFrom, lookAt, up, fov, 1.0,
                                          aperture, focusDist);
}

}  // namespace

std::shared_ptr<ICamera> CameraFactory::createPerspective(
    const math::Vector3D& lookFrom, const math::Vector3D& lookAt,
    const math::Vector3D& vup, double vfov, double aspectRatio,
    double aperture, double focusDist) {
  return std::make_shared<Perspective>(lookFrom, lookAt, vup, vfov, aspectRatio,
                                       aperture, focusDist);
}

std::shared_ptr<ICamera> CameraFactory::create(const std::string& type,
                                                const libconfig::Setting& cfg) {
  if (type == "perspective") {
    return createPerspectiveFromCfg(cfg);
  }
  throw RaytracerException("CameraFactory: unknown camera type '" + type + "'");
}

}  // namespace raytracer::core::factory
