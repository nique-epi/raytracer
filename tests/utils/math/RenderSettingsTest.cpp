/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** RenderSettings unit tests
*/

#include <gtest/gtest.h>
#include "utils/math/RenderSettings.hpp"

using raytracer::math::RenderSettings;

TEST(RenderSettingsTest, DefaultValues) {
  RenderSettings s;
  EXPECT_EQ(s.imageWidth, 800);
  EXPECT_EQ(s.imageHeight, 600);
  EXPECT_EQ(s.tileWidth, 32);
  EXPECT_EQ(s.tileHeight, 32);
  EXPECT_GT(s.numThreads, 0);
  EXPECT_EQ(s.samplesPerPixel, 1);
  EXPECT_EQ(s.maxDepth, 5);
}

TEST(RenderSettingsTest, ValidateDefaults) {
  RenderSettings s;
  EXPECT_TRUE(s.validate());
}

TEST(RenderSettingsTest, ValidateRejectsZeroWidth) {
  RenderSettings s;
  s.imageWidth = 0;
  EXPECT_FALSE(s.validate());
}

TEST(RenderSettingsTest, ValidateRejectsZeroHeight) {
  RenderSettings s;
  s.imageHeight = 0;
  EXPECT_FALSE(s.validate());
}

TEST(RenderSettingsTest, ValidateRejectsZeroSamples) {
  RenderSettings s;
  s.samplesPerPixel = 0;
  EXPECT_FALSE(s.validate());
}

TEST(RenderSettingsTest, ValidateRejectsZeroDepth) {
  RenderSettings s;
  s.maxDepth = 0;
  EXPECT_FALSE(s.validate());
}

TEST(RenderSettingsTest, ValidateRejectsNegativeValues) {
  RenderSettings s;
  s.tileWidth = -1;
  EXPECT_FALSE(s.validate());
}

TEST(RenderSettingsTest, ValidateCustomValid) {
  RenderSettings s;
  s.imageWidth = 1920;
  s.imageHeight = 1080;
  s.samplesPerPixel = 64;
  s.maxDepth = 10;
  EXPECT_TRUE(s.validate());
}
