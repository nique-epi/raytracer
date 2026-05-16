/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ViewportRunner
*/

#include "interface/ViewportRunner.hpp"
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <mutex>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include "interface/Viewport.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/RenderSettings.hpp"

namespace raytracer::interface {

ViewportRunner::ViewportRunner(raytracer::core::RaytracerRenderer& renderer,
                               raytracer::core::RendererConfig baseConfig,
                               raytracer::core::Frame frame)
    : renderer_(renderer),
      baseConfig_(std::move(baseConfig)),
      frame_(std::move(frame)),
      width_(baseConfig_.settings.imageWidth),
      height_(baseConfig_.settings.imageHeight),
      targetSamples_(std::max(1, baseConfig_.settings.samplesPerPixel)),
      totalPasses_(std::max(1, targetSamples_ / samplesPerPass)),
      effectiveSamples_(totalPasses_ * samplesPerPass),
      finalImage_(width_, height_) {}

int ViewportRunner::run() {
  Viewport viewport(width_, height_, "Raytracer");
  logger_.info("viewport opened ", width_, 'x', height_, ", ", totalPasses_,
               " passes of ", samplesPerPass, " samples (", effectiveSamples_,
               " total)");

  std::atomic<bool> renderDone{false};
  std::thread worker([this, &viewport, &renderDone]() {
    accumulate(viewport);
    renderDone.store(true);
  });

  while (!viewport.shouldClose() && !renderDone.load()) {
    viewport.tick();
    std::this_thread::sleep_for(
        std::chrono::milliseconds(Viewport::displayFrameMs));
  }
  worker.join();

  reportOutcome(viewport);

  raytracer::components::Image exportImage(width_, height_);
  {
    const std::lock_guard<std::mutex> lock(finalImageMutex_);
    exportImage = finalImage_;
  }
  viewport.write(exportImage, "out.ppm");
  return 0;
}

raytracer::core::RendererConfig ViewportRunner::buildPassConfig() const {
  raytracer::math::RenderSettings perPassSettings = baseConfig_.settings;
  perPassSettings.samplesPerPixel = samplesPerPass;
  return raytracer::core::RendererConfig{
      .scene = baseConfig_.scene,
      .settings = perPassSettings,
      .shadingContext = baseConfig_.shadingContext};
}

void ViewportRunner::accumulate(Viewport& viewport) {
  auto loopTimer = logger_.scope("viewport accumulation");
  const raytracer::core::RendererConfig perPassConfig = buildPassConfig();
  std::vector<raytracer::math::Color> accumulator(
      static_cast<std::size_t>(width_) * static_cast<std::size_t>(height_));
  raytracer::components::Image displayImage(width_, height_);

  for (int pass = 1; pass <= totalPasses_; ++pass) {
    if (viewport.shouldClose()) {
      logger_.info("user closed viewport at pass ", pass - 1, '/',
                   totalPasses_);
      break;
    }
    const raytracer::components::Image passImage =
        renderer_.render(perPassConfig, frame_);
    accumulatePass(accumulator, passImage, displayImage, pass);
    publishPass(viewport, displayImage, pass);
  }
}

void ViewportRunner::accumulatePass(
    std::vector<raytracer::math::Color>& accumulator,
    const raytracer::components::Image& passImage,
    raytracer::components::Image& displayImage, int pass) const {
  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {
      const std::size_t index =
          (static_cast<std::size_t>(y) * static_cast<std::size_t>(width_)) +
          static_cast<std::size_t>(x);
      accumulator[index] = accumulator[index] +
                           passImage.getPixel(static_cast<std::size_t>(x),
                                              static_cast<std::size_t>(y));
      displayImage.setPixel(static_cast<std::size_t>(x),
                            static_cast<std::size_t>(y),
                            accumulator[index] / static_cast<double>(pass));
    }
  }
}

void ViewportRunner::publishPass(
    Viewport& viewport, const raytracer::components::Image& displayImage,
    int pass) {
  viewport.liveDisplay(displayImage);
  {
    const std::lock_guard<std::mutex> lock(finalImageMutex_);
    finalImage_ = displayImage;
  }
  completedPasses_.store(pass);
  viewport.setStatus("Rendering " + std::to_string(pass * samplesPerPass) +
                     " / " + std::to_string(effectiveSamples_) + " samples");
  logger_.debug("pass ", pass, '/', totalPasses_, " accumulated");
}

void ViewportRunner::reportOutcome(Viewport& viewport) {
  const int donePasses = completedPasses_.load();
  const int finalSamples = donePasses * samplesPerPass;
  if (donePasses == 0) {
    viewport.setStatus("Aborted before first pass | saving black out.ppm");
    logger_.warn(
        "viewport closed before any pass completed; exporting black image");
  } else if (donePasses == totalPasses_) {
    viewport.setStatus("Done | " + std::to_string(finalSamples) +
                       " samples | close window to save out.ppm");
    logger_.info("render finished: ", finalSamples,
                 " samples - close window to export");
  } else {
    viewport.setStatus("Interrupted at " + std::to_string(finalSamples) +
                       " / " + std::to_string(effectiveSamples_) +
                       " samples | close window to save partial out.ppm");
    logger_.info("render interrupted: ", finalSamples, '/', effectiveSamples_,
                 " samples - close window to export partial result");
  }
}

}  // namespace raytracer::interface
