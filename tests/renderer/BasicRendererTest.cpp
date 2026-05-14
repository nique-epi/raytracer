/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MonoThreadRendererTest
*/

#include <gtest/gtest.h>
#include <memory>
#include <type_traits>
#include "../fixtures/OrthoCameraFixture.hpp"
#include "../fixtures/SphereFixture.hpp"
#include "renderer/Frame.hpp"
#include "renderer/IRenderer.hpp"
#include "renderer/RendererConfig.hpp"
#include "renderer/monoThreadRenderer/MonoThreadRenderer.hpp"
#include "scene/Scene.hpp"
#include "utils/math/RenderSettings.hpp"

static_assert(std::is_base_of_v<raytracer::core::IRenderer,
                                raytracer::core::MonoThreadRenderer>);

TEST(MonoThreadRendererTest, RendersOneSphereAndHashStable) {
  auto scene = std::make_shared<raytracer::scene::Scene>();

  auto sphere =
      std::make_shared<SphereFixture>(raytracer::math::Vector3D(0, 0, -2), 1.0);
  scene->add(sphere);

  auto camera = std::make_shared<OrthoCameraFixture>();
  raytracer::math::RenderSettings settings;
  settings.imageWidth = 101;
  settings.imageHeight = 101;
  settings.maxDepth = 1;

  raytracer::core::MonoThreadRenderer renderer;
  int progressCalls = 0;
  renderer.setProgressCallback([&](double) { progressCalls++; });

  const raytracer::core::RendererConfig config{
      .scene = scene, .settings = settings, .integrator = nullptr};
  const raytracer::core::Frame frame{.camera = camera};
  raytracer::components::Image img = renderer.render(config, frame);

  EXPECT_GT(progressCalls, 0);

  double colorSum = 0;
  for (int y = 0; y < settings.imageHeight; ++y) {
    for (int x = 0; x < settings.imageWidth; ++x) {
      auto pixel = img.getPixel(x, y);
      colorSum += pixel.r + pixel.g + pixel.b;
    }
  }

  EXPECT_NEAR(colorSum, 14646.313963815759, 1e-5);

  auto centerPixel = img.getPixel(50, 50);
  EXPECT_NEAR(centerPixel.r, 0.61630695443645078, 1e-4);
  EXPECT_NEAR(centerPixel.g, 0.61630695443645078, 1e-4);
  EXPECT_NEAR(centerPixel.b, 0.80379746835443022, 1e-4);
}
