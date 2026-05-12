/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SceneBuilder unit tests
*/

#include <gtest/gtest.h>
#include "exceptions/Exceptions.hpp"
#include "../fixtures/ComponentFactoryFixture.hpp"
#include "../fixtures/SphereFixture.hpp"
#include "fixtures/SceneBuilderFixture.hpp"
#include "fixtures/TransformsBuilderFixture.hpp"
#include "scene/Scene.hpp"
#include "scene/SceneBuilder.hpp"

using raytracer::scene::SceneBuilder;

// Given: a builder with mock registries.
// When:  addObject is called with a registered type.
// Then:  count("sphere") equals 1 and no exception is thrown.
TEST_F(SceneBuilderFixture, AddObjectIncreasesCount) {
  SceneBuilder builder(factory_);
  EXPECT_NO_THROW(builder.addObject("sphere", *stubSetting_));
  EXPECT_EQ(builder.count("sphere"), 1u);
}

// Given: a builder with mock registries.
// When:  addObject is called twice with the same type.
// Then:  count("sphere") equals 2.
TEST_F(SceneBuilderFixture, AddObjectCountsMultipleCalls) {
  SceneBuilder builder(factory_);
  builder.addObject("sphere", *stubSetting_);
  builder.addObject("sphere", *stubSetting_);
  EXPECT_EQ(builder.count("sphere"), 2u);
}

// Given: a builder with mock registries.
// When:  addLight is called with a registered type.
// Then:  count("ambient") equals 1.
TEST_F(SceneBuilderFixture, AddLightIncreasesCount) {
  SceneBuilder builder(factory_);
  EXPECT_NO_THROW(builder.addLight("ambient", *stubSetting_));
  EXPECT_EQ(builder.count("ambient"), 1u);
}

// Given: a builder with mock registries.
// When:  addCamera is called.
// Then:  count("camera") equals 1.
TEST_F(SceneBuilderFixture, AddCameraIncreasesCount) {
  SceneBuilder builder(factory_);
  EXPECT_NO_THROW(builder.addCamera(*stubSetting_));
  EXPECT_EQ(builder.count("camera"), 1u);
}

// Given: a builder with a registered camera and light.
// When:  build() is called.
// Then:  it returns a non-null shared_ptr<Scene>.
TEST_F(SceneBuilderFixture, BuildReturnsScene) {
  SceneBuilder builder(factory_);
  builder.addCamera(*stubSetting_);
  builder.addLight("ambient", *stubSetting_);
  auto scene = builder.build();
  EXPECT_NE(scene, nullptr);
}

// Given: a builder with a camera and light added.
// When:  build() is called.
// Then:  the returned scene has a camera set.
TEST_F(SceneBuilderFixture, BuildSceneHasCamera) {
  SceneBuilder builder(factory_);
  builder.addCamera(*stubSetting_);
  builder.addLight("ambient", *stubSetting_);
  auto scene = builder.build();
  EXPECT_NE(scene->getCamera(), nullptr);
}

// Given: a builder with a camera and light added.
// When:  build() is called.
// Then:  the returned scene has at least one light.
TEST_F(SceneBuilderFixture, BuildSceneHasLight) {
  SceneBuilder builder(factory_);
  builder.addCamera(*stubSetting_);
  builder.addLight("ambient", *stubSetting_);
  auto scene = builder.build();
  EXPECT_FALSE(scene->getLights().empty());
}

// Given: a builder with no camera added.
// When:  build() is called.
// Then:  it throws std::runtime_error.
TEST_F(SceneBuilderFixture, BuildThrowsWhenNoCamera) {
  SceneBuilder builder(factory_);
  builder.addLight("ambient", *stubSetting_);
  EXPECT_THROW(builder.build(), raytracer::core::RaytracerException);
}

// Given: a builder with no lights added.
// When:  build() is called.
// Then:  it throws std::runtime_error.
TEST_F(SceneBuilderFixture, BuildThrowsWhenNoLights) {
  SceneBuilder builder(factory_);
  builder.addCamera(*stubSetting_);
  EXPECT_THROW(builder.build(), raytracer::core::RaytracerException);
}

// Given: a completely empty builder.
// When:  build() is called.
// Then:  it throws std::runtime_error.
TEST_F(SceneBuilderFixture, BuildThrowsWhenEmpty) {
  SceneBuilder builder(factory_);
  EXPECT_THROW(builder.build(), raytracer::core::RaytracerException);
}

// Given: a builder where an unregistered object type is requested.
// When:  addObject is called with that unknown type.
// Then:  RaytracerException is thrown and the attempt is counted.
TEST_F(SceneBuilderFixture, AddObjectWithUnknownTypeThrows) {
  SceneBuilder builder(factory_);
  EXPECT_THROW(builder.addObject("unknown_primitive", *stubSetting_),
               raytracer::core::RaytracerException);
  EXPECT_EQ(builder.count("unknown_primitive"), 1u);
}

// Given: a builder where an unregistered light type is requested.
// When:  addLight is called with that unknown type.
// Then:  RaytracerException is thrown and the attempt is counted.
TEST_F(SceneBuilderFixture, AddLightWithUnknownTypeThrows) {
  SceneBuilder builder(factory_);
  EXPECT_THROW(builder.addLight("unknown_light", *stubSetting_),
               raytracer::core::RaytracerException);
  EXPECT_EQ(builder.count("unknown_light"), 1u);
}

// Given: a builder backed by a factory_ that has no "perspective" camera.
// When:  addCamera is called.
// Then:  RaytracerException is thrown and count("camera") equals 1.
TEST_F(SceneBuilderFixture, AddCameraWithEmptyRegistryThrows) {
  ComponentFactoryFixture emptyFactory;
  SceneBuilder builder(emptyFactory);
  EXPECT_THROW(builder.addCamera(*stubSetting_),
               raytracer::core::RaytracerException);
  EXPECT_EQ(builder.count("camera"), 1u);
}

// Given: a builder with zero entries.
// When:  count() is called for any type.
// Then:  it returns 0.
TEST_F(SceneBuilderFixture, CountReturnsZeroForUnseenType) {
  SceneBuilder builder(factory_);
  EXPECT_EQ(builder.count("sphere"), 0u);
  EXPECT_EQ(builder.count("camera"), 0u);
  EXPECT_EQ(builder.count("anything"), 0u);
}

// Given: a builder backed by a real sphere creator and translation factory.
// When:  addObject parses a sphere(center=0,r=0.5) with translation(10,0,0).
// Then:  the resulting scene hits identically to a sphere(center=10,0,0).
TEST_F(TransformsBuilderFixture, AddObjectAppliesTransformsInScene) {
  SceneBuilder builder(factory_);
  libconfig::Config config;
  auto& sphereCfg =
      config.getRoot().add("sphere", libconfig::Setting::TypeGroup);
  auto& transforms =
      sphereCfg.add("transforms", libconfig::Setting::TypeList);
  auto& entry = transforms.add(libconfig::Setting::TypeGroup);
  entry.add("type", libconfig::Setting::TypeString) = "translation";
  auto& offset = entry.add("offset", libconfig::Setting::TypeGroup);
  offset.add("x", libconfig::Setting::TypeFloat) = 10.0;
  offset.add("y", libconfig::Setting::TypeFloat) = 0.0;
  offset.add("z", libconfig::Setting::TypeFloat) = 0.0;
  auto& cameraCfg =
      config.getRoot().add("camera", libconfig::Setting::TypeGroup);
  auto& lightCfg =
      config.getRoot().add("light", libconfig::Setting::TypeGroup);

  builder.addObject("sphere", sphereCfg);
  builder.addCamera(cameraCfg);
  builder.addLight("ambient", lightCfg);
  auto scene = builder.build();

  const raytracer::math::Ray ray(raytracer::math::Vector3D(0.0, 0.0, 0.0),
                                 raytracer::math::Vector3D(1.0, 0.0, 0.0));
  raytracer::math::HitRecord transformedRec;
  ASSERT_TRUE(scene->hit(ray, 0.001, 1000.0, transformedRec));

  SphereFixture direct(raytracer::math::Vector3D(10.0, 0.0, 0.0), 0.5);
  raytracer::math::HitRecord directRec;
  ASSERT_TRUE(direct.hits(ray, 0.001, 1000.0, directRec));

  EXPECT_NEAR(transformedRec.t, directRec.t, 1e-9);
  EXPECT_NEAR(transformedRec.point.x, directRec.point.x, 1e-9);
  EXPECT_NEAR(transformedRec.point.y, directRec.point.y, 1e-9);
  EXPECT_NEAR(transformedRec.point.z, directRec.point.z, 1e-9);
}
