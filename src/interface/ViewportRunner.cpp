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
#include <optional>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include "interface/Viewport.hpp"
#include "rendering/shading/ShadingContext.hpp"
#include "rendering/shading/ShadingPool.hpp"
#include "scene/Scene.hpp"
#include "scene/World.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/RenderSettings.hpp"

#ifdef BUILD_BONUS
#include "postprocess/denoise/OIDDenoiser.hpp"
#endif

namespace raytracer::interface {

ViewportRunner::ViewportRunner(
    raytracer::core::RaytracerRenderer& renderer,
    raytracer::core::RendererConfig baseConfig, raytracer::core::Frame frame,
    std::shared_ptr<raytracer::shading::ShadingPool> shadingPool)
    : renderer_(renderer),
      baseConfig_(std::move(baseConfig)),
      frame_(std::move(frame)),
      shadingPool_(std::move(shadingPool)),
      width_(baseConfig_.settings.imageWidth),
      height_(baseConfig_.settings.imageHeight),
      targetSamples_(std::max(1, baseConfig_.settings.samplesPerPixel)),
      totalPasses_((targetSamples_ + samplesPerPass - 1) / samplesPerPass),
      effectiveSamples_(totalPasses_ * samplesPerPass),
      currentMode_(baseConfig_.scene->getWorld().viewportMode()),
      finalImage_(width_, height_),
      accumulator_(static_cast<std::size_t>(width_) *
                   static_cast<std::size_t>(height_)),
      displayImage_(width_, height_) {}

int ViewportRunner::run() {
  Viewport viewport(width_, height_, "Raytracer");
  const raytracer::scene::ViewportMode initialMode = currentMode_.load();
  viewport.setViewportMode(initialMode);
  viewport.setStatus(decorateStatus("Starting render"));
  logger_.info("viewport opened ", width_, 'x', height_, ", mode ",
               raytracer::scene::viewportModeName(initialMode), ", requested ",
               targetSamples_, " samples -> ", totalPasses_, " passes x ",
               samplesPerPass, " = ", effectiveSamples_, " effective");

  std::thread worker([this, &viewport]() { accumulate(viewport); });

  while (!viewport.shouldClose()) {
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
  viewport.write(exportImage, baseConfig_.outputPath);
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

void ViewportRunner::resetAccumulation() {
  std::ranges::fill(accumulator_, raytracer::math::Color{});
  pass_ = 1;
  completedPasses_.store(0);
  displayImage_ = raytracer::components::Image(width_, height_);
  const std::lock_guard<std::mutex> lock(finalImageMutex_);
  finalImage_ = raytracer::components::Image(width_, height_);
}

void ViewportRunner::accumulate(Viewport& viewport) {
  auto loopTimer = logger_.scope("viewport accumulation");
  const raytracer::core::RendererConfig perPassConfig = buildPassConfig();

  while (!viewport.shouldClose()) {
    if (handleModeSwitch(viewport)) {
      continue;
    }
    if (pass_ > totalPasses_) {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(Viewport::displayFrameMs));
      continue;
    }
    renderPass(viewport, perPassConfig);
  }
}

bool ViewportRunner::handleModeSwitch(Viewport& viewport) {
  const std::optional<raytracer::scene::ViewportMode> requested =
      viewport.consumeModeRequest();
  if (!requested) {
    return false;
  }
  applyViewportModeSwitch(*requested);
  resetAccumulation();
  viewport.liveDisplay(displayImage_);
  viewport.setStatus(decorateStatus("Restarting render"));
  return true;
}

void ViewportRunner::renderPass(
    Viewport& viewport, const raytracer::core::RendererConfig& perPassConfig) {
  const int pass = pass_;
  renderer_.setProgressCallback([this, &viewport, pass](double fraction) {
    reportPassProgress(viewport, pass, fraction);
  });
  const raytracer::components::Image passImage =
      renderer_.render(perPassConfig, frame_);
  if (viewport.shouldClose() || viewport.hasModeRequest()) {
    return;
  }
  accumulatePass(passImage);
  publishPass(viewport);
  pass_ = pass + 1;
}

void ViewportRunner::applyViewportModeSwitch(
    raytracer::scene::ViewportMode mode) {
  currentMode_.store(mode);
  baseConfig_.shadingContext->setStrategy(shadingPool_->get(mode));
  logger_.info("viewport mode -> ", raytracer::scene::viewportModeName(mode),
               ", restarting accumulation");
}

std::string ViewportRunner::decorateStatus(const std::string& body) const {
  return '[' +
         std::string(raytracer::scene::viewportModeName(currentMode_.load())) +
         "]  " + body + "   |   Tab: switch view mode";
}

void ViewportRunner::reportPassProgress(Viewport& viewport, int pass,
                                        double fraction) const {
  if (viewport.hasModeRequest()) {
    return;
  }
  const int completedSamples = (pass - 1) * samplesPerPass;
  const int percent = static_cast<int>(fraction * 100.0);
  viewport.setStatus(
      decorateStatus("Rendering " + std::to_string(completedSamples) + " / " +
                     std::to_string(effectiveSamples_) + " samples (pass " +
                     std::to_string(percent) + "%)"));
}

void ViewportRunner::accumulatePass(
    const raytracer::components::Image& passImage) {
  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {
      const std::size_t index =
          (static_cast<std::size_t>(y) * static_cast<std::size_t>(width_)) +
          static_cast<std::size_t>(x);
      accumulator_[index] =
          accumulator_[index] + passImage.getPixel(static_cast<std::size_t>(x),
                                                   static_cast<std::size_t>(y));
      displayImage_.setPixel(static_cast<std::size_t>(x),
                             static_cast<std::size_t>(y),
                             accumulator_[index] / static_cast<double>(pass_));
    }
  }
}

void ViewportRunner::publishPass(Viewport& viewport) {
  viewport.liveDisplay(displayImage_);
  {
    const std::lock_guard<std::mutex> lock(finalImageMutex_);
    finalImage_ = displayImage_;
  }
  completedPasses_.store(pass_);
  const int samples = pass_ * samplesPerPass;
  if (pass_ >= totalPasses_) {
#ifdef BUILD_BONUS
    raytracer::components::Image denoised = displayImage_;
    OIDDenoiser::denoise(denoised);
    viewport.liveDisplay(denoised);
    {
      const std::lock_guard<std::mutex> lock(finalImageMutex_);
      finalImage_ = denoised;
    }
#endif
    viewport.setStatus(decorateStatus("Done - " + std::to_string(samples) +
                                      " samples - close window to save"));
  } else {
    viewport.setStatus(
        decorateStatus("Rendering " + std::to_string(samples) + " / " +
                       std::to_string(effectiveSamples_) + " samples"));
  }
  logger_.debug("pass ", pass_, '/', totalPasses_, " accumulated");
}

void ViewportRunner::reportOutcome(Viewport& viewport) {
  const int donePasses = completedPasses_.load();
  const int finalSamples = donePasses * samplesPerPass;
  const std::string& outFile = baseConfig_.outputPath;
  if (donePasses == 0) {
    viewport.setStatus("Aborted before first pass | saving black " + outFile);
    logger_.warn(
        "viewport closed before any pass completed; exporting black image");
  } else if (donePasses == totalPasses_) {
    viewport.setStatus("Done | " + std::to_string(finalSamples) +
                       " samples | close window to save " + outFile);
    logger_.info("render finished: ", finalSamples,
                 " samples - close window to export");
  } else {
    viewport.setStatus("Interrupted at " + std::to_string(finalSamples) +
                       " / " + std::to_string(effectiveSamples_) +
                       " samples | close window to save partial " + outFile);
    logger_.info("render interrupted: ", finalSamples, '/', effectiveSamples_,
                 " samples - close window to export partial result");
  }
}

}  // namespace raytracer::interface
