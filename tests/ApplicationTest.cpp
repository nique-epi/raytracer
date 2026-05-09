/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Application unit tests
*/

#include <gtest/gtest.h>
#include <cstdio>
#include <fstream>
#include <string>
#include "application/Application.hpp"
#include "exceptions/Exceptions.hpp"

static const std::string ValidCfg = SCENES_DIR "/example.cfg";
static const std::string InvalidDimensionsCfg = SCENE_FIXTURES_DIR "/invalid_dimensions.cfg";
static const std::string OutputPpm = "out.ppm";

// Given: a .cfg whose settings block has imageWidth=0 and imageHeight=0.
// When:  run() is called.
// Then:  throws RaytracerException (invalid render settings).
TEST(ApplicationTest, InvalidSettingsThrows) {
  EXPECT_THROW(raytracer::core::Application{}.run(InvalidDimensionsCfg),
               raytracer::core::RaytracerException);
}

// Given: a file path whose extension has no registered loader (e.g. ".txt").
// When:  run() is called.
// Then:  throws RaytracerException (no loader available).
TEST(ApplicationTest, UnknownExtensionThrows) {
  EXPECT_THROW(raytracer::core::Application{}.run("scene.txt"),
               raytracer::core::RaytracerException);
}

// Given: a .cfg path that does not exist on disk.
// When:  run() is called.
// Then:  throws RaytracerException (failed to load scene).
TEST(ApplicationTest, NonExistentFileThrows) {
  EXPECT_THROW(raytracer::core::Application{}.run("/nonexistent/scene.cfg"),
               raytracer::core::RaytracerException);
}

// Given: the canonical example.cfg scene file.
// When:  run() is called.
// Then:  returns 0 and produces out.ppm on disk.
TEST(ApplicationTest, DISABLED_ValidSceneReturns0AndWritesPPM) {
  std::remove(OutputPpm.c_str());
  EXPECT_EQ(raytracer::core::Application{}.run(ValidCfg), 0);
  std::ifstream f(OutputPpm);
  EXPECT_TRUE(f.good());
  std::remove(OutputPpm.c_str());
}

// Given: the canonical example.cfg scene file.
// When:  run() is called.
// Then:  out.ppm starts with the P3 PPM header.
TEST(ApplicationTest, DISABLED_ValidSceneOutputsP3Header) {
  std::remove(OutputPpm.c_str());
  raytracer::core::Application{}.run(ValidCfg);

  std::ifstream f(OutputPpm);
  std::string magic;
  f >> magic;
  EXPECT_EQ(magic, "P3");
  std::remove(OutputPpm.c_str());
}

// Given: example.cfg declares imageWidth=1920 and imageHeight=1080.
// When:  run() is called.
// Then:  out.ppm contains "1920 1080" in its header.
TEST(ApplicationTest, DISABLED_ValidSceneOutputsCorrectDimensions) {
  std::remove(OutputPpm.c_str());
  raytracer::core::Application{}.run(ValidCfg);

  std::ifstream f(OutputPpm);
  std::string content((std::istreambuf_iterator<char>(f)),
                      std::istreambuf_iterator<char>());
  EXPECT_NE(content.find("1920 1080"), std::string::npos);
  std::remove(OutputPpm.c_str());
}
