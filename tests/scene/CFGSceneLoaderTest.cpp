/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** CFGSceneLoader integration tests
*/

#include <gtest/gtest.h>
#include "fixtures/SceneBuilderFixture.hpp"
#include "scene/CFGSceneLoader.hpp"
#include "scene/SceneBuilder.hpp"
#include "utils/math/RenderSettings.hpp"

using raytracer::scene::CFGSceneLoader;
using raytracer::scene::SceneBuilder;

static const std::string ExampleCfg = SCENES_DIR "/example.cfg";
static const std::string NoSettingsCfg = SCENE_FIXTURES_DIR "/no_settings.cfg";
static const std::string PartialSettingsCfg =
    SCENE_FIXTURES_DIR "/partial_settings.cfg";
static const std::string NoPrimitivesCfg =
    SCENE_FIXTURES_DIR "/no_primitives.cfg";
static const std::string EmptyListsCfg = SCENE_FIXTURES_DIR "/empty_lists.cfg";
static const std::string NoCameraCfg = SCENE_FIXTURES_DIR "/no_camera.cfg";
static const std::string MalformedCfg = SCENE_FIXTURES_DIR "/malformed.cfg";

class CFGSceneLoaderTest : public SceneBuilderFixture {
 protected:
  CFGSceneLoader loader;
};

// Given: a fresh CFGSceneLoader.
// When:  supports() is called with various file extensions.
// Then:  it returns true only for "cfg" and false for any other extension.
TEST_F(CFGSceneLoaderTest, SupportsOnlyCfg) {
  EXPECT_TRUE(loader.supports("cfg"));
  EXPECT_FALSE(loader.supports("json"));
  EXPECT_FALSE(loader.supports("xml"));
  EXPECT_FALSE(loader.supports(""));
}

// Given: the canonical example.cfg scene file on disk.
// When:  load() is called with a fresh builder and settings.
// Then:  it returns true, indicating the file was parsed without error.
TEST_F(CFGSceneLoaderTest, LoadExampleSucceeds) {
  SceneBuilder builder(objectRegistry, lightRegistry, cameraRegistry,
                       materialRegistry);
  raytracer::math::RenderSettings settings;

  EXPECT_TRUE(loader.load(ExampleCfg, builder, settings));
}

// Given: example.cfg declares 2 spheres, 1 plane, 1 ambient light,
//        1 directional light and 1 camera.
// When:  load() is called.
// Then:  the builder has recorded exactly that many calls of each type.
TEST_F(CFGSceneLoaderTest, LoadExampleContainsExpectedObjects) {
  SceneBuilder builder(objectRegistry, lightRegistry, cameraRegistry,
                       materialRegistry);
  raytracer::math::RenderSettings settings;

  loader.load(ExampleCfg, builder, settings);

  EXPECT_EQ(builder.count("sphere"), 2u);
  EXPECT_EQ(builder.count("plane"), 1u);
  EXPECT_EQ(builder.count("ambient"), 1u);
  EXPECT_EQ(builder.count("directional"), 1u);
  EXPECT_EQ(builder.count("camera"), 1u);
}

// Given: example.cfg contains a `settings` block with imageWidth=1920,
//        imageHeight=1080, samplesPerPixel=4, maxDepth=10.
// When:  load() is called.
// Then:  the RenderSettings struct reflects every value defined in the file.
TEST_F(CFGSceneLoaderTest, LoadExamplePopulatesSettings) {
  SceneBuilder builder(objectRegistry, lightRegistry, cameraRegistry,
                       materialRegistry);
  raytracer::math::RenderSettings settings;

  loader.load(ExampleCfg, builder, settings);

  EXPECT_EQ(settings.imageWidth, 1920);
  EXPECT_EQ(settings.imageHeight, 1080);
  EXPECT_EQ(settings.samplesPerPixel, 4);
  EXPECT_EQ(settings.maxDepth, 10);
}

// Given: a file path that does not exist on disk.
// When:  load() is called with that path.
// Then:  it catches the libconfig::FileIOException and returns false
//        (no exception escapes to the caller).
TEST_F(CFGSceneLoaderTest, LoadMissingFileReturnsFalse) {
  SceneBuilder builder(objectRegistry, lightRegistry, cameraRegistry,
                       materialRegistry);
  raytracer::math::RenderSettings settings;

  EXPECT_FALSE(loader.load("/nonexistent/path/scene.cfg", builder, settings));
}

// Given: a .cfg file containing invalid libconfig syntax.
// When:  load() is called with that file.
// Then:  it catches the libconfig::ParseException and returns false.
TEST_F(CFGSceneLoaderTest, LoadMalformedFileReturnsFalse) {
  SceneBuilder builder(objectRegistry, lightRegistry, cameraRegistry,
                       materialRegistry);
  raytracer::math::RenderSettings settings;

  EXPECT_FALSE(loader.load(MalformedCfg, builder, settings));
}

// Given: a .cfg file that has no `settings` block, and a freshly
//        default-constructed RenderSettings.
// When:  load() is called.
// Then:  every field of RenderSettings stays at its compile-time default.
TEST_F(CFGSceneLoaderTest, LoadWithoutSettingsBlockKeepsDefaults) {
  SceneBuilder builder(objectRegistry, lightRegistry, cameraRegistry,
                       materialRegistry);
  raytracer::math::RenderSettings settings;
  const raytracer::math::RenderSettings defaults;

  EXPECT_TRUE(loader.load(NoSettingsCfg, builder, settings));

  EXPECT_EQ(settings.imageWidth, defaults.imageWidth);
  EXPECT_EQ(settings.imageHeight, defaults.imageHeight);
  EXPECT_EQ(settings.tileWidth, defaults.tileWidth);
  EXPECT_EQ(settings.tileHeight, defaults.tileHeight);
  EXPECT_EQ(settings.samplesPerPixel, defaults.samplesPerPixel);
  EXPECT_EQ(settings.maxDepth, defaults.maxDepth);
}

// Given: a .cfg file whose `settings` block defines only samplesPerPixel.
// When:  load() is called with a fresh RenderSettings.
// Then:  samplesPerPixel is updated, while every other field stays at default.
TEST_F(CFGSceneLoaderTest, LoadWithPartialSettingsKeepsOtherDefaults) {
  SceneBuilder builder(objectRegistry, lightRegistry, cameraRegistry,
                       materialRegistry);
  raytracer::math::RenderSettings settings;
  const raytracer::math::RenderSettings defaults;

  EXPECT_TRUE(loader.load(PartialSettingsCfg, builder, settings));
  EXPECT_EQ(settings.samplesPerPixel, 16);
  EXPECT_EQ(settings.imageWidth, defaults.imageWidth);
  EXPECT_EQ(settings.imageHeight, defaults.imageHeight);
  EXPECT_EQ(settings.tileWidth, defaults.tileWidth);
  EXPECT_EQ(settings.tileHeight, defaults.tileHeight);
  EXPECT_EQ(settings.maxDepth, defaults.maxDepth);
}

// Given: a .cfg file with no `primitives` block at all.
// When:  load() is called.
// Then:  it returns true and the builder has zero primitive calls.
TEST_F(CFGSceneLoaderTest, LoadWithoutPrimitivesSucceedsWithNoObjects) {
  SceneBuilder builder(objectRegistry, lightRegistry, cameraRegistry,
                       materialRegistry);
  raytracer::math::RenderSettings settings;

  EXPECT_TRUE(loader.load(NoPrimitivesCfg, builder, settings));
  EXPECT_EQ(builder.count("sphere"), 0u);
  EXPECT_EQ(builder.count("plane"), 0u);
}

// Given: a .cfg file where every list is declared but empty, plus a camera.
// When:  load() is called.
// Then:  it returns true, no primitive or light is registered, camera is parsed.
TEST_F(CFGSceneLoaderTest, LoadWithEmptyListsSucceedsWithNoObjects) {
  SceneBuilder builder(objectRegistry, lightRegistry, cameraRegistry,
                       materialRegistry);
  raytracer::math::RenderSettings settings;

  EXPECT_TRUE(loader.load(EmptyListsCfg, builder, settings));
  EXPECT_EQ(builder.count("sphere"), 0u);
  EXPECT_EQ(builder.count("plane"), 0u);
  EXPECT_EQ(builder.count("ambient"), 0u);
  EXPECT_EQ(builder.count("directional"), 0u);
  EXPECT_EQ(builder.count("camera"), 1u);
}

// Given: a .cfg file with primitives but no `camera` block.
// When:  load() is called.
// Then:  it returns true, no camera is registered, primitives are parsed.
TEST_F(CFGSceneLoaderTest, LoadWithoutCameraSucceedsWithNoCamera) {
  SceneBuilder builder(objectRegistry, lightRegistry, cameraRegistry,
                       materialRegistry);
  raytracer::math::RenderSettings settings;

  EXPECT_TRUE(loader.load(NoCameraCfg, builder, settings));
  EXPECT_EQ(builder.count("camera"), 0u);
  EXPECT_EQ(builder.count("sphere"), 1u);
}
