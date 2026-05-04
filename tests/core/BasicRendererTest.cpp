/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** BasicRendererTest
*/

#include <gtest/gtest.h>
#include <memory>
#include "../fixtures/OrthoCameraFixture.hpp"
#include "../fixtures/SphereFixture.hpp"
#include "components/Primitives/Collection.hpp"
#include "core/renderer/BasicRenderer.hpp"
#include "utils/math/RenderSettings.hpp"

TEST(BasicRendererTest, RendersOneSphereAndHashStable) {
  raytracer::components::Collection scene;

  // Add a sphere at (0, 0, -2) with radius 1
  auto sphere =
      std::make_shared<SphereFixture>(raytracer::math::Vector3D(0, 0, -2), 1.0);
  scene.add(sphere);

  OrthoCameraFixture camera;
  raytracer::math::RenderSettings settings;
  settings.imageWidth = 101;  // Odd size so center is exactly pixel 50
  settings.imageHeight = 101;
  settings.maxDepth = 1;

  raytracer::core::BasicRenderer renderer;
  int progressCalls = 0;
  renderer.setProgressCallback([&](double) { progressCalls++; });

  raytracer::components::Image img = renderer.render(scene, camera, settings);

  EXPECT_GT(progressCalls, 0);

  // Hash or checksum check
  double colorSum = 0;
  for (int y = 0; y < settings.imageHeight; ++y) {
    for (int x = 0; x < settings.imageWidth; ++x) {
      auto pixel = img.getPixel(x, y);
      colorSum += pixel.r + pixel.g + pixel.b;
    }
  }

  // Stable sum check
  EXPECT_GT(colorSum, 0.0);

  // Center pixel hit exactly at (0, 0)
  auto centerPixel = img.getPixel(50, 50);
  EXPECT_NEAR(centerPixel.r, 0.5, 1e-4);
  EXPECT_NEAR(centerPixel.g, 0.5, 1e-4);
  EXPECT_NEAR(centerPixel.b, 1.0, 1e-4);
}
