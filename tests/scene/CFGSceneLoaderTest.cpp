/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** CFGSceneLoader integration tests
*/

#include <gtest/gtest.h>
#include <memory>
#include "fixtures/SceneBuilderFixture.hpp"
#include "scene/CFGSceneLoader.hpp"
#include "scene/Scene.hpp"
#include "scene/SceneBuilder.hpp"
#include "scene/SceneFileNotFoundException.hpp"
#include "scene/SceneParseException.hpp"
#include "scene/World.hpp"
#include "scene/background/Gradient/GradientBackground.hpp"
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
static const std::string GradientBackgroundCfg =
    SCENE_FIXTURES_DIR "/gradient_background.cfg";
static const std::string WorldWireframeCfg =
    SCENE_FIXTURES_DIR "/world_wireframe.cfg";
static const std::string WorldMaterialPreviewCfg =
    SCENE_FIXTURES_DIR "/world_material_preview.cfg";
static const std::string WorldUnknownModeCfg =
    SCENE_FIXTURES_DIR "/world_unknown_mode.cfg";
static const std::string WorldAmbientOcclusionCfg =
    SCENE_FIXTURES_DIR "/world_ambient_occlusion.cfg";

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
  SceneBuilder builder(factory_);
  raytracer::math::RenderSettings settings;

  EXPECT_TRUE(loader.load(ExampleCfg, builder, settings));
}

// Given: example.cfg declares 2 spheres, 1 plane, 1 ambient light,
//        1 directional light and 1 camera.
// When:  load() is called.
// Then:  the builder has recorded exactly that many calls of each type.
TEST_F(CFGSceneLoaderTest, LoadExampleContainsExpectedObjects) {
  SceneBuilder builder(factory_);
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
  SceneBuilder builder(factory_);
  raytracer::math::RenderSettings settings;

  loader.load(ExampleCfg, builder, settings);

  EXPECT_EQ(settings.imageWidth, 1920);
  EXPECT_EQ(settings.imageHeight, 1080);
  EXPECT_EQ(settings.samplesPerPixel, 4);
  EXPECT_EQ(settings.maxDepth, 10);
}

// Given: a file path that does not exist on disk.
// When:  load() is called with that path.
// Then:  it throws SceneFileNotFoundException.
TEST_F(CFGSceneLoaderTest, LoadMissingFileThrows) {
  SceneBuilder builder(factory_);
  raytracer::math::RenderSettings settings;

  EXPECT_THROW(loader.load("/nonexistent/path/scene.cfg", builder, settings),
               raytracer::scene::SceneFileNotFoundException);
}

// Given: a .cfg file containing invalid libconfig syntax.
// When:  load() is called with that file.
// Then:  it throws SceneParseException.
TEST_F(CFGSceneLoaderTest, LoadMalformedFileThrows) {
  SceneBuilder builder(factory_);
  raytracer::math::RenderSettings settings;

  EXPECT_THROW(loader.load(MalformedCfg, builder, settings),
               raytracer::scene::SceneParseException);
}

// Given: a .cfg file that has no `settings` block, and a freshly
//        default-constructed RenderSettings.
// When:  load() is called.
// Then:  every field of RenderSettings stays at its compile-time default.
TEST_F(CFGSceneLoaderTest, LoadWithoutSettingsBlockKeepsDefaults) {
  SceneBuilder builder(factory_);
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
  SceneBuilder builder(factory_);
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
  SceneBuilder builder(factory_);
  raytracer::math::RenderSettings settings;

  EXPECT_TRUE(loader.load(NoPrimitivesCfg, builder, settings));
  EXPECT_EQ(builder.count("sphere"), 0u);
  EXPECT_EQ(builder.count("plane"), 0u);
}

// Given: a .cfg file where every list is declared but empty, plus a camera.
// When:  load() is called.
// Then:  it returns true, no primitive or light is registered, camera is
// parsed.
TEST_F(CFGSceneLoaderTest, LoadWithEmptyListsSucceedsWithNoObjects) {
  SceneBuilder builder(factory_);
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
  SceneBuilder builder(factory_);
  raytracer::math::RenderSettings settings;

  EXPECT_TRUE(loader.load(NoCameraCfg, builder, settings));
  EXPECT_EQ(builder.count("camera"), 0u);
  EXPECT_EQ(builder.count("sphere"), 1u);
}

// Given: a .cfg file whose background block declares a gradient.
// When:  load() is called.
// Then:  the resulting scene background is a GradientBackground with the
//        expected endpoint colours.
TEST_F(CFGSceneLoaderTest, LoadGradientBackgroundCreatesGradientBackground) {
  SceneBuilder builder(factory_);
  raytracer::math::RenderSettings settings;

  EXPECT_TRUE(loader.load(GradientBackgroundCfg, builder, settings));

  const auto scene = builder.build();
  const auto background = scene->getBackground();
  const auto gradient = std::dynamic_pointer_cast<
      raytracer::scene::background::GradientBackground>(background);

  ASSERT_NE(gradient, nullptr);

  const raytracer::math::Ray topRay(raytracer::math::Vector3D(0, 0, 0),
                                    raytracer::math::Vector3D(0, -1, 0));
  const raytracer::math::Ray bottomRay(raytracer::math::Vector3D(0, 0, 0),
                                       raytracer::math::Vector3D(0, 1, 0));

  const auto topColor = gradient->getColor(topRay);
  const auto bottomColor = gradient->getColor(bottomRay);

  EXPECT_DOUBLE_EQ(topColor.r, 1.0);
  EXPECT_DOUBLE_EQ(topColor.g, 1.0);
  EXPECT_DOUBLE_EQ(topColor.b, 1.0);
  EXPECT_DOUBLE_EQ(bottomColor.r, 0.0);
  EXPECT_DOUBLE_EQ(bottomColor.g, 0.0);
  EXPECT_DOUBLE_EQ(bottomColor.b, 0.0);
}

// Given: a .cfg file with no `world` block (no_settings.cfg, which has a
//        camera and an ambient light but no world configuration).
// When:  load() is called and the scene is built.
// Then:  the scene's World defaults to ViewportMode::Rendered, matching
//        the World default-constructor and the final-image path.
TEST_F(CFGSceneLoaderTest, LoadWithoutWorldBlockDefaultsToRendered) {
  SceneBuilder builder(factory_);
  raytracer::math::RenderSettings settings;

  EXPECT_TRUE(loader.load(NoSettingsCfg, builder, settings));

  const auto scene = builder.build();
  EXPECT_EQ(scene->getWorld().viewportMode(),
            raytracer::scene::ViewportMode::Rendered);
}

// Given: a .cfg file whose world block declares mode = "wireframe".
// When:  load() is called and the scene is built.
// Then:  the scene's World reports ViewportMode::Wireframe. The scene
//        builds successfully even without any ILight because Wireframe
//        does not require explicit lighting.
TEST_F(CFGSceneLoaderTest, LoadWireframeModeSetsViewportToWireframe) {
  SceneBuilder builder(factory_);
  raytracer::math::RenderSettings settings;

  EXPECT_TRUE(loader.load(WorldWireframeCfg, builder, settings));

  const auto scene = builder.build();
  EXPECT_EQ(scene->getWorld().viewportMode(),
            raytracer::scene::ViewportMode::Wireframe);
}

// Given: a .cfg file whose world block declares mode = "materialPreview".
// When:  load() is called and the scene is built.
// Then:  the scene's World reports ViewportMode::MaterialPreview. Like
//        Wireframe, this mode does not require explicit ILight sources.
TEST_F(CFGSceneLoaderTest, LoadMaterialPreviewModeSetsViewportToMaterialPreview) {
  SceneBuilder builder(factory_);
  raytracer::math::RenderSettings settings;

  EXPECT_TRUE(loader.load(WorldMaterialPreviewCfg, builder, settings));

  const auto scene = builder.build();
  EXPECT_EQ(scene->getWorld().viewportMode(),
            raytracer::scene::ViewportMode::MaterialPreview);
}

// Given: a .cfg file whose world block declares an unrecognised mode
//        string ("definitelyNotARealMode").
// When:  load() is called and the scene is built.
// Then:  parseWorld() silently falls back to ViewportMode::Rendered
//        rather than throwing. The fixture includes an ambient light so
//        the Rendered-mode lights-required check passes at build time.
TEST_F(CFGSceneLoaderTest, LoadUnknownModeFallsBackToRendered) {
  SceneBuilder builder(factory_);
  raytracer::math::RenderSettings settings;

  EXPECT_TRUE(loader.load(WorldUnknownModeCfg, builder, settings));

  const auto scene = builder.build();
  EXPECT_EQ(scene->getWorld().viewportMode(),
            raytracer::scene::ViewportMode::Rendered);
}

// Given: a .cfg file whose world block declares an ambientOcclusion
//        configuration with non-default values.
// When:  the scene is built.
// Then:  the World's AO settings reflect the parsed values verbatim.
TEST_F(CFGSceneLoaderTest, LoadWorldAmbientOcclusionPopulatesSettings) {
  SceneBuilder builder(factory_);
  raytracer::math::RenderSettings settings;

  EXPECT_TRUE(loader.load(WorldAmbientOcclusionCfg, builder, settings));

  const auto scene = builder.build();
  const auto& ao = scene->getWorld().ambientOcclusion();
  EXPECT_TRUE(ao.enabled);
  EXPECT_EQ(ao.samples, 24);
  EXPECT_DOUBLE_EQ(ao.radius, 1.5);
  EXPECT_DOUBLE_EQ(ao.intensity, 0.75);
}

// Given: a .cfg file with no ambientOcclusion block.
// When:  the scene is built.
// Then:  AO settings keep their disabled defaults so nothing changes.
TEST_F(CFGSceneLoaderTest, LoadWithoutAmbientOcclusionKeepsDefaults) {
  SceneBuilder builder(factory_);
  raytracer::math::RenderSettings settings;

  EXPECT_TRUE(loader.load(WorldWireframeCfg, builder, settings));

  const auto scene = builder.build();
  const auto& ao = scene->getWorld().ambientOcclusion();
  EXPECT_FALSE(ao.enabled);
}
