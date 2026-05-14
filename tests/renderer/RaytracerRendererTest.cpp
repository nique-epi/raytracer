/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** RaytracerRenderer unit tests
*/

#include <gtest/gtest.h>
#include <chrono>
#include <cstdlib>
#include <memory>
#include <thread>
#include <type_traits>

#include "../fixtures/OrthoCameraFixture.hpp"
#include "../fixtures/SphereFixture.hpp"
#include "components/image/Image.hpp"
#include "integrator/pathIntegrator/PathIntegrator.hpp"
#include "renderer/Frame.hpp"
#include "renderer/IRenderer.hpp"
#include "renderer/RendererConfig.hpp"
#include "renderer/monoThreadRenderer/MonoThreadRenderer.hpp"
#include "renderer/raytracerRenderer/RaytracerRenderer.hpp"
#include "scene/Scene.hpp"
#include "utils/math/RenderSettings.hpp"
#include "utils/math/Vector3D.hpp"

namespace {

std::shared_ptr<raytracer::scene::Scene> buildSphereScene() {
  auto scene = std::make_shared<raytracer::scene::Scene>();
  scene->add(std::make_shared<SphereFixture>(raytracer::math::Vector3D(0, 0, -2),
                                             1.0));
  return scene;
}

raytracer::math::RenderSettings makeSettings(int width, int height,
                                             int numThreads) {
  raytracer::math::RenderSettings settings;
  settings.imageWidth = width;
  settings.imageHeight = height;
  settings.tileWidth = 32;
  settings.tileHeight = 32;
  settings.numThreads = numThreads;
  settings.samplesPerPixel = 1;
  settings.maxDepth = 1;
  return settings;
}

raytracer::components::Image renderWith(raytracer::core::IRenderer& renderer,
                                        const raytracer::math::RenderSettings&
                                            settings) {
  auto scene = buildSphereScene();
  auto camera = std::make_shared<OrthoCameraFixture>();

  const raytracer::core::RendererConfig config{
      .scene = scene,
      .settings = settings,
      .integrator = std::make_shared<raytracer::core::PathIntegrator>()};
  const raytracer::core::Frame frame{.camera = camera};
  return renderer.render(config, frame);
}

bool pixelsEqual(const raytracer::components::Image& lhs,
                 const raytracer::components::Image& rhs) {
  if (lhs.getWidth() != rhs.getWidth() || lhs.getHeight() != rhs.getHeight()) {
    return false;
  }
  for (std::size_t y = 0; y < lhs.getHeight(); ++y) {
    for (std::size_t x = 0; x < lhs.getWidth(); ++x) {
      const auto left = lhs.getPixel(x, y);
      const auto right = rhs.getPixel(x, y);
      if (left.r != right.r || left.g != right.g || left.b != right.b) {
        return false;
      }
    }
  }
  return true;
}

}  // namespace

static_assert(std::is_base_of_v<raytracer::core::IRenderer,
                                raytracer::core::RaytracerRenderer>);

TEST(RaytracerRendererTest, ProducesSameImageAsMonoThread) {
  const auto settings = makeSettings(101, 101, 4);

  raytracer::core::MonoThreadRenderer mono;
  raytracer::core::RaytracerRenderer multi;

  const auto referenceImage = renderWith(mono, settings);
  const auto threadedImage = renderWith(multi, settings);

  EXPECT_TRUE(pixelsEqual(referenceImage, threadedImage));
}

TEST(RaytracerRendererTest, DeterministicAcrossThreadCounts) {
  const auto baseSettings = makeSettings(80, 60, 1);

  raytracer::core::RaytracerRenderer single;
  raytracer::core::RaytracerRenderer eight;

  auto withThreads = baseSettings;
  withThreads.numThreads = 1;
  const auto imageSingle = renderWith(single, withThreads);

  withThreads.numThreads = 8;
  const auto imageEight = renderWith(eight, withThreads);

  EXPECT_TRUE(pixelsEqual(imageSingle, imageEight));
}

TEST(RaytracerRendererTest, ZeroThreadsResolvesToHardwareConcurrency) {
  // We cannot directly observe pool size; instead we verify the renderer
  // runs to completion with numThreads == 0 and produces the same image as
  // an explicit thread count.
  const auto settings = makeSettings(64, 48, 0);

  raytracer::core::RaytracerRenderer zero;
  raytracer::core::RaytracerRenderer explicitCount;

  const auto imageZero = renderWith(zero, settings);

  auto explicitSettings = settings;
  explicitSettings.numThreads = 4;
  const auto imageExplicit = renderWith(explicitCount, explicitSettings);

  EXPECT_EQ(imageZero.getWidth(), 64u);
  EXPECT_EQ(imageZero.getHeight(), 48u);
  EXPECT_TRUE(pixelsEqual(imageZero, imageExplicit));
}

TEST(RaytracerRendererTest, ProgressCallbackCalled) {
  const auto settings = makeSettings(64, 48, 2);
  raytracer::core::RaytracerRenderer renderer;

  int calls = 0;
  double lastProgress = -1.0;
  renderer.setProgressCallback([&](double progress) {
    ++calls;
    lastProgress = progress;
  });

  (void)renderWith(renderer, settings);
  EXPECT_GT(calls, 0);
  EXPECT_DOUBLE_EQ(lastProgress, 1.0);
}

// Speedup test (acceptance criterion). Skipped when the host has fewer than
// 4 hardware threads or when the env var RT_SKIP_PERF_TESTS is set, both to
// keep CI green on small runners.
TEST(RaytracerRendererTest, MultiThreadIsFasterThanMonoOn4Threads) {
  if (std::getenv("RT_SKIP_PERF_TESTS") != nullptr) {
    GTEST_SKIP() << "Performance test skipped via RT_SKIP_PERF_TESTS";
  }
  if (std::thread::hardware_concurrency() < 4) {
    GTEST_SKIP() << "Host reports fewer than 4 hardware threads";
  }

  const auto settings = makeSettings(800, 600, 4);

  using Clock = std::chrono::steady_clock;

  raytracer::core::MonoThreadRenderer mono;
  const auto monoBegin = Clock::now();
  (void)renderWith(mono, settings);
  const auto monoElapsed = Clock::now() - monoBegin;

  raytracer::core::RaytracerRenderer multi;
  const auto multiBegin = Clock::now();
  (void)renderWith(multi, settings);
  const auto multiElapsed = Clock::now() - multiBegin;

  const double speedup =
      std::chrono::duration<double>(monoElapsed).count() /
      std::chrono::duration<double>(multiElapsed).count();

  // The ticket asks for > 2x speedup on 4 threads. The fixture is
  // deliberately simple (ortho camera + single sphere), so the result is
  // shading-bound and scales close to linear in practice.
  EXPECT_GT(speedup, 2.0)
      << "mono: "
      << std::chrono::duration_cast<std::chrono::milliseconds>(monoElapsed)
             .count()
      << " ms, multi: "
      << std::chrono::duration_cast<std::chrono::milliseconds>(multiElapsed)
             .count()
      << " ms";
}
