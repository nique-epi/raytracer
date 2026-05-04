/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Application unit tests
*/

#include <gtest/gtest.h>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include "Application.hpp"

static const std::string ValidCfg = SCENES_DIR "/example.cfg";
static const std::string OutputPpm = "out.ppm";

namespace {

struct StderrCapture {
  StderrCapture() : _old(std::cerr.rdbuf(_buf.rdbuf())) {}
  ~StderrCapture() { std::cerr.rdbuf(_old); }
  std::string str() const { return _buf.str(); }

 private:
  std::ostringstream _buf;
  std::streambuf* _old;
};

}  // namespace

// Given: a file path whose extension has no registered loader (e.g. ".txt").
// When:  run() is called.
// Then:  returns 84 and writes a diagnostic to stderr.
TEST(ApplicationTest, UnknownExtensionReturns84) {
  StderrCapture err;
  EXPECT_EQ(raytracer::Application{}.run("scene.txt"), 84);
  EXPECT_FALSE(err.str().empty());
}

// Given: a .cfg path that does not exist on disk.
// When:  run() is called.
// Then:  returns 84 (CFGSceneLoader reports the I/O error to stderr).
TEST(ApplicationTest, NonExistentFileReturns84) {
  StderrCapture err;
  EXPECT_EQ(raytracer::Application{}.run("/nonexistent/scene.cfg"), 84);
}

// Given: the canonical example.cfg scene file.
// When:  run() is called.
// Then:  returns 0 and produces out.ppm on disk.
TEST(ApplicationTest, ValidSceneReturns0AndWritesPPM) {
  std::remove(OutputPpm.c_str());
  EXPECT_EQ(raytracer::Application{}.run(ValidCfg), 0);
  std::ifstream f(OutputPpm);
  EXPECT_TRUE(f.good());
  std::remove(OutputPpm.c_str());
}

// Given: the canonical example.cfg scene file.
// When:  run() is called.
// Then:  out.ppm starts with the P3 PPM header.
TEST(ApplicationTest, ValidSceneOutputsP3Header) {
  std::remove(OutputPpm.c_str());
  raytracer::Application{}.run(ValidCfg);

  std::ifstream f(OutputPpm);
  std::string magic;
  f >> magic;
  EXPECT_EQ(magic, "P3");
  std::remove(OutputPpm.c_str());
}

// Given: example.cfg declares imageWidth=1920 and imageHeight=1080.
// When:  run() is called.
// Then:  out.ppm contains "1920 1080" in its header.
TEST(ApplicationTest, ValidSceneOutputsCorrectDimensions) {
  std::remove(OutputPpm.c_str());
  raytracer::Application{}.run(ValidCfg);

  std::ifstream f(OutputPpm);
  std::string content((std::istreambuf_iterator<char>(f)),
                      std::istreambuf_iterator<char>());
  EXPECT_NE(content.find("1920 1080"), std::string::npos);
  std::remove(OutputPpm.c_str());
}
