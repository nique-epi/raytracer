/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** CameraFactory unit tests
*/

#include <gtest/gtest.h>
#include <libconfig.h++>
#include <memory>
#include "exceptions/Exceptions.hpp"
#include "factory/CameraFactory.hpp"
#include "utils/math/Ray.hpp"

using raytracer::core::factory::CameraFactory;

namespace {

class CfgFromString {
 public:
  explicit CfgFromString(const char* source) {
    config_.readString(source);
  }

  [[nodiscard]] const libconfig::Setting& at(const char* path) const {
    return config_.lookup(path);
  }

 private:
  libconfig::Config config_;
};

}  // namespace

// Given: a default call to createPerspective.
// When:  no parameters are passed.
// Then:  the factory returns a non-null camera that produces rays.
TEST(CameraFactoryTest, CreatePerspectiveWithDefaultsReturnsNonNull) {
  auto camera = CameraFactory::createPerspective();
  ASSERT_NE(camera, nullptr);
  const auto ray = camera->getRay(0.5, 0.5);
  // A ray-origin equality test would couple us to the camera basis; we
  // just assert the call path runs and returns a finite ray.
  EXPECT_TRUE(std::isfinite(ray.getOrigin().x));
}

// Given: the example.cfg camera schema (position + direction + fov).
// When:  create("perspective", cfg) is called.
// Then:  the resulting camera is non-null.
TEST(CameraFactoryTest, CreatePerspectiveFromExampleCfgSchema) {
  CfgFromString cfg(
      "camera = {"
      " position = { x = 0.0; y = 0.0; z = 5.0; };"
      " direction = { x = 0.0; y = 0.0; z = -1.0; };"
      " fov = 90.0;"
      "};");
  auto camera = CameraFactory::create("perspective", cfg.at("camera"));
  EXPECT_NE(camera, nullptr);
}

// Given: an empty libconfig setting.
// When:  create("perspective", cfg) is called.
// Then:  the camera is created with default arguments.
TEST(CameraFactoryTest, CreatePerspectiveFromEmptyCfgFallsBackToDefaults) {
  CfgFromString cfg("camera = { };");
  auto camera = CameraFactory::create("perspective", cfg.at("camera"));
  EXPECT_NE(camera, nullptr);
}

// Given: an unknown type name.
// When:  create() is called.
// Then:  it throws RaytracerException.
TEST(CameraFactoryTest, CreateUnknownTypeThrows) {
  CfgFromString cfg("entry = { };");
  EXPECT_THROW(CameraFactory::create("orthographic", cfg.at("entry")),
               raytracer::core::RaytracerException);
}
