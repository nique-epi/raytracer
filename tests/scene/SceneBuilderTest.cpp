/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SceneBuilder unit tests
*/

#include <gtest/gtest.h>
#include <stdexcept>
#include "core/registry/registry.hpp"
#include "fixtures/SceneBuilderFixture.hpp"
#include "scene/SceneBuilder.hpp"

using raytracer::core::registry::CameraRegistry;
using raytracer::scene::SceneBuilder;

// Given: a builder with mock registries.
// When:  addObject is called with a registered type.
// Then:  count("sphere") equals 1 and no exception is thrown.
TEST_F(SceneBuilderFixture, AddObjectIncreasesCount) {
  SceneBuilder builder(objectRegistry, lightRegistry, cameraRegistry,
                       materialRegistry);
  EXPECT_NO_THROW(builder.addObject("sphere", *stubSetting));
  EXPECT_EQ(builder.count("sphere"), 1u);
}

// Given: a builder with mock registries.
// When:  addObject is called twice with the same type.
// Then:  count("sphere") equals 2.
TEST_F(SceneBuilderFixture, AddObjectCountsMultipleCalls) {
  SceneBuilder builder(objectRegistry, lightRegistry, cameraRegistry,
                       materialRegistry);
  builder.addObject("sphere", *stubSetting);
  builder.addObject("sphere", *stubSetting);
  EXPECT_EQ(builder.count("sphere"), 2u);
}

// Given: a builder with mock registries.
// When:  addLight is called with a registered type.
// Then:  count("ambient") equals 1.
TEST_F(SceneBuilderFixture, AddLightIncreasesCount) {
  SceneBuilder builder(objectRegistry, lightRegistry, cameraRegistry,
                       materialRegistry);
  EXPECT_NO_THROW(builder.addLight("ambient", *stubSetting));
  EXPECT_EQ(builder.count("ambient"), 1u);
}

// Given: a builder with mock registries.
// When:  addCamera is called.
// Then:  count("camera") equals 1.
TEST_F(SceneBuilderFixture, AddCameraIncreasesCount) {
  SceneBuilder builder(objectRegistry, lightRegistry, cameraRegistry,
                       materialRegistry);
  EXPECT_NO_THROW(builder.addCamera(*stubSetting));
  EXPECT_EQ(builder.count("camera"), 1u);
}

// Given: a builder with a registered camera and light.
// When:  build() is called.
// Then:  it returns a non-null shared_ptr<Scene>.
TEST_F(SceneBuilderFixture, BuildReturnsScene) {
  SceneBuilder builder(objectRegistry, lightRegistry, cameraRegistry,
                       materialRegistry);
  builder.addCamera(*stubSetting);
  builder.addLight("ambient", *stubSetting);
  auto scene = builder.build();
  EXPECT_NE(scene, nullptr);
}

// Given: a builder with a camera and light added.
// When:  build() is called.
// Then:  the returned scene has a camera set.
TEST_F(SceneBuilderFixture, BuildSceneHasCamera) {
  SceneBuilder builder(objectRegistry, lightRegistry, cameraRegistry,
                       materialRegistry);
  builder.addCamera(*stubSetting);
  builder.addLight("ambient", *stubSetting);
  auto scene = builder.build();
  EXPECT_NE(scene->getCamera(), nullptr);
}

// Given: a builder with a camera and light added.
// When:  build() is called.
// Then:  the returned scene has at least one light.
TEST_F(SceneBuilderFixture, BuildSceneHasLight) {
  SceneBuilder builder(objectRegistry, lightRegistry, cameraRegistry,
                       materialRegistry);
  builder.addCamera(*stubSetting);
  builder.addLight("ambient", *stubSetting);
  auto scene = builder.build();
  EXPECT_FALSE(scene->getLights().empty());
}

// Given: a builder with no camera added.
// When:  build() is called.
// Then:  it throws std::runtime_error.
TEST_F(SceneBuilderFixture, BuildThrowsWhenNoCamera) {
  SceneBuilder builder(objectRegistry, lightRegistry, cameraRegistry,
                       materialRegistry);
  builder.addLight("ambient", *stubSetting);
  EXPECT_THROW(builder.build(), std::runtime_error);
}

// Given: a builder with no lights added.
// When:  build() is called.
// Then:  it throws std::runtime_error.
TEST_F(SceneBuilderFixture, BuildThrowsWhenNoLights) {
  SceneBuilder builder(objectRegistry, lightRegistry, cameraRegistry,
                       materialRegistry);
  builder.addCamera(*stubSetting);
  EXPECT_THROW(builder.build(), std::runtime_error);
}

// Given: a completely empty builder.
// When:  build() is called.
// Then:  it throws std::runtime_error.
TEST_F(SceneBuilderFixture, BuildThrowsWhenEmpty) {
  SceneBuilder builder(objectRegistry, lightRegistry, cameraRegistry,
                       materialRegistry);
  EXPECT_THROW(builder.build(), std::runtime_error);
}

// Given: a builder where an unregistered object type is requested.
// When:  addObject is called with that unknown type.
// Then:  no exception propagates and the attempt is counted.
TEST_F(SceneBuilderFixture, AddObjectWithUnknownTypeSkipsGracefully) {
  SceneBuilder builder(objectRegistry, lightRegistry, cameraRegistry,
                       materialRegistry);
  EXPECT_NO_THROW(builder.addObject("unknown_primitive", *stubSetting));
  EXPECT_EQ(builder.count("unknown_primitive"), 1u);
}

// Given: a builder where an unregistered light type is requested.
// When:  addLight is called with that unknown type.
// Then:  no exception propagates and the attempt is counted.
TEST_F(SceneBuilderFixture, AddLightWithUnknownTypeSkipsGracefully) {
  SceneBuilder builder(objectRegistry, lightRegistry, cameraRegistry,
                       materialRegistry);
  EXPECT_NO_THROW(builder.addLight("unknown_light", *stubSetting));
  EXPECT_EQ(builder.count("unknown_light"), 1u);
}

// Given: a builder where the camera registry has no "perspective" type.
// When:  addCamera is called.
// Then:  no exception propagates and count("camera") equals 1.
TEST_F(SceneBuilderFixture, AddCameraWithEmptyRegistrySkipsGracefully) {
  CameraRegistry emptyCamReg;
  SceneBuilder builder(objectRegistry, lightRegistry, emptyCamReg,
                       materialRegistry);
  EXPECT_NO_THROW(builder.addCamera(*stubSetting));
  EXPECT_EQ(builder.count("camera"), 1u);
}

// Given: a builder with zero entries.
// When:  count() is called for any type.
// Then:  it returns 0.
TEST_F(SceneBuilderFixture, CountReturnsZeroForUnseenType) {
  SceneBuilder builder(objectRegistry, lightRegistry, cameraRegistry,
                       materialRegistry);
  EXPECT_EQ(builder.count("sphere"), 0u);
  EXPECT_EQ(builder.count("camera"), 0u);
  EXPECT_EQ(builder.count("anything"), 0u);
}
