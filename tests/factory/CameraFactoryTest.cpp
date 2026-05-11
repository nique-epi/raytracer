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
#include "factory/camera/CameraFactory.hpp"

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

TEST(CameraFactoryTest, CreatePerspectiveWithDefaultsReturnsNonNull) {
  auto camera = CameraFactory::createPerspective();
  EXPECT_NE(camera, nullptr);
}

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

TEST(CameraFactoryTest, CreatePerspectiveFromEmptyCfgFallsBackToDefaults) {
  CfgFromString cfg("camera = { };");
  auto camera = CameraFactory::create("perspective", cfg.at("camera"));
  EXPECT_NE(camera, nullptr);
}

TEST(CameraFactoryTest, CreateUnknownTypeThrows) {
  CfgFromString cfg("entry = { };");
  EXPECT_THROW(
      static_cast<void>(CameraFactory::create("orthographic", cfg.at("entry"))),
      raytracer::core::RaytracerException);
}
