/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SceneAssembler end-to-end load tests
*/

#include <gtest/gtest.h>

#include <fstream>
#include <memory>
#include <optional>
#include <string>

#include "application/SceneAssembler.hpp"
#include "exceptions/Exceptions.hpp"
#include "scene/Scene.hpp"
#include "scene/World.hpp"

using raytracer::core::RaytracerException;
using raytracer::core::SceneAssembler;

namespace {

const std::string ExampleCfg = SCENES_DIR "/example.cfg";
const std::string InvalidDimensionsCfg =
    SCENE_FIXTURES_DIR "/invalid_dimensions.cfg";
const std::string WorldWireframeCfg =
    SCENE_FIXTURES_DIR "/world_wireframe.cfg";

}  // namespace

TEST(SceneAssemblerTest, LoadsValidCfgScene) {
  SceneAssembler assembler;

  const auto loaded = assembler.load(ExampleCfg, /*useBVH=*/false, std::nullopt);

  ASSERT_NE(loaded.scene, nullptr);
  EXPECT_EQ(loaded.settings.imageWidth, 1920);
  EXPECT_EQ(loaded.settings.imageHeight, 1080);
  EXPECT_EQ(loaded.outputPath, "out.ppm");
}

TEST(SceneAssemblerTest, PreservesViewportModeFromCfg) {
  SceneAssembler assembler;

  const auto loaded =
      assembler.load(WorldWireframeCfg, /*useBVH=*/false, std::nullopt);

  ASSERT_NE(loaded.scene, nullptr);
  EXPECT_EQ(loaded.scene->getWorld().viewportMode(),
            raytracer::scene::ViewportMode::Wireframe);
}

TEST(SceneAssemblerTest, UnknownExtensionThrows) {
  SceneAssembler assembler;

  EXPECT_THROW(
      assembler.load("scene.totally_unknown", /*useBVH=*/false, std::nullopt),
      RaytracerException);
}

TEST(SceneAssemblerTest, NonExistentFileThrows) {
  SceneAssembler assembler;

  EXPECT_THROW(
      assembler.load("/does/not/exist.cfg", /*useBVH=*/false, std::nullopt),
      RaytracerException);
}

TEST(SceneAssemblerTest, InvalidSettingsThrows) {
  SceneAssembler assembler;

  EXPECT_THROW(assembler.load(InvalidDimensionsCfg, /*useBVH=*/false,
                              std::nullopt),
               RaytracerException);
}

TEST(SceneAssemblerTest, UseBVHTrueDoesNotThrow) {
  SceneAssembler assembler;

  EXPECT_NO_THROW(static_cast<void>(
      assembler.load(ExampleCfg, /*useBVH=*/true, std::nullopt)));
}

#ifdef BUILD_BONUS

namespace {

struct JsonConfigFixture {
  std::string path;

  JsonConfigFixture(const std::string& contents, const std::string& filename)
      : path(std::string{SCENE_FIXTURES_DIR} + "/" + filename) {
    std::ofstream file(path);
    file << contents;
  }

  ~JsonConfigFixture() { std::remove(path.c_str()); }

  JsonConfigFixture(const JsonConfigFixture&) = delete;
  JsonConfigFixture& operator=(const JsonConfigFixture&) = delete;
  JsonConfigFixture(JsonConfigFixture&&) = delete;
  JsonConfigFixture& operator=(JsonConfigFixture&&) = delete;
};

}  // namespace

TEST(SceneAssemblerTest, JsonConfigOverridesImageWidth) {
  SceneAssembler assembler;
  JsonConfigFixture json(R"({ "imageWidth": 42, "imageHeight": 24 })",
                         "scene_assembler_override_width.json");

  const auto loaded = assembler.load(ExampleCfg, /*useBVH=*/false, json.path);

  EXPECT_EQ(loaded.settings.imageWidth, 42);
  EXPECT_EQ(loaded.settings.imageHeight, 24);
}

TEST(SceneAssemblerTest, JsonConfigOverridesViewportMode) {
  SceneAssembler assembler;
  JsonConfigFixture json(
      R"({ "imageWidth": 100, "imageHeight": 100, "viewportMode": "wireframe" })",
      "scene_assembler_override_viewport.json");

  const auto loaded = assembler.load(ExampleCfg, /*useBVH=*/false, json.path);

  EXPECT_EQ(loaded.scene->getWorld().viewportMode(),
            raytracer::scene::ViewportMode::Wireframe);
}

TEST(SceneAssemblerTest, JsonConfigOverridesOutputFile) {
  SceneAssembler assembler;
  JsonConfigFixture json(
      R"({ "imageWidth": 100, "imageHeight": 100, "name": "custom.ppm" })",
      "scene_assembler_override_output.json");

  const auto loaded = assembler.load(ExampleCfg, /*useBVH=*/false, json.path);

  EXPECT_EQ(loaded.outputPath, "custom.ppm");
}

#endif  // BUILD_BONUS
