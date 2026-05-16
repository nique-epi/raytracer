/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** viewport
*/

#include "viewport.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <tuple>
#include <utility>
#include <vector>
#include "common/helper/Logger.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/RenderSettings.hpp"

#ifndef VIEWPORT_FONT_PATH
#error "VIEWPORT_FONT_PATH must be defined by CMake"
#endif

namespace raytracer::bonus {

namespace {
constexpr int displayFrameMs = 16;
constexpr std::size_t channels = 4;
constexpr std::uint8_t alphaOpaque = 255;
constexpr unsigned int overlayFontSize = 18;
constexpr float overlayPadding = 12.0F;
constexpr float overlayMargin = 16.0F;
constexpr float overlayPaddingTotal = overlayPadding * 2.0F;
constexpr std::uint8_t overlayBackgroundAlpha = 160;

// RaytracerRenderer only applies sub-pixel jitter when samplesPerPixel > 1
// (see RaytracerRenderer.hpp determinism contract). A pass must therefore
// render at least 2 samples, otherwise every pass is bit-identical and
// accumulation never reduces noise.
constexpr int samplesPerPass = 2;
}  // namespace

struct Viewport::Impl {
  Impl(int w, int h, std::string t)
      : width_(w),
        height_(h),
        title_(std::move(t)),
        pixelBuffer_(static_cast<std::size_t>(w) * static_cast<std::size_t>(h) *
                         channels,
                     0),
        window_(sf::VideoMode(static_cast<unsigned int>(w),
                              static_cast<unsigned int>(h)),
                title_) {
    for (std::size_t i = channels - 1; i < pixelBuffer_.size(); i += channels) {
      pixelBuffer_[i] = alphaOpaque;
    }
    if (!texture_.create(static_cast<unsigned int>(w),
                         static_cast<unsigned int>(h))) {
      throw std::runtime_error("Viewport: failed to create SFML texture");
    }
    sprite_.setTexture(texture_, true);

    fontLoaded_ = font_.loadFromFile(VIEWPORT_FONT_PATH);
    if (!fontLoaded_) {
      logger_.warn("failed to load font at ", VIEWPORT_FONT_PATH,
                   " — status overlay disabled");
      return;
    }
    statusText_.setFont(font_);
    statusText_.setCharacterSize(overlayFontSize);
    statusText_.setFillColor(sf::Color::White);
    statusText_.setPosition(overlayMargin + overlayPadding,
                            overlayMargin + overlayPadding);
    statusBackground_.setFillColor(
        sf::Color(0, 0, 0, overlayBackgroundAlpha));
    statusBackground_.setPosition(overlayMargin, overlayMargin);
  }

  void uploadPixels(const raytracer::components::Image& image) {
    const std::size_t width = image.getWidth();
    const std::size_t height = image.getHeight();
    const std::lock_guard<std::mutex> lock(bufferMutex_);
    for (std::size_t y = 0; y < height; ++y) {
      for (std::size_t x = 0; x < width; ++x) {
        const auto [r, g, b] = image.getPixel(x, y).toRGB();
        const std::size_t base = ((y * width) + x) * channels;
        pixelBuffer_[base + 0] = static_cast<std::uint8_t>(r);
        pixelBuffer_[base + 1] = static_cast<std::uint8_t>(g);
        pixelBuffer_[base + 2] = static_cast<std::uint8_t>(b);
        pixelBuffer_[base + 3] = alphaOpaque;
      }
    }
    bufferDirty_.store(true);
  }

  void refreshOverlayLayout() {
    const sf::FloatRect bounds = statusText_.getLocalBounds();
    statusBackground_.setSize(
        sf::Vector2f(bounds.width + overlayPaddingTotal,
                     static_cast<float>(overlayFontSize) +
                         overlayPaddingTotal));
  }

  void tick() {
    if (!window_.isOpen()) {
      shouldClose_.store(true);
      return;
    }
    sf::Event event{};
    while (window_.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        shouldClose_.store(true);
      }
    }
    if (bufferDirty_.exchange(false)) {
      const std::lock_guard<std::mutex> lock(bufferMutex_);
      texture_.update(pixelBuffer_.data());
    }
    if (statusDirty_.exchange(false) && fontLoaded_) {
      std::string nextStatus;
      {
        const std::lock_guard<std::mutex> lock(statusMutex_);
        nextStatus = pendingStatus_;
      }
      statusText_.setString(
          sf::String::fromUtf8(nextStatus.begin(), nextStatus.end()));
      refreshOverlayLayout();
    }
    window_.clear(sf::Color::Black);
    window_.draw(sprite_);
    if (fontLoaded_) {
      window_.draw(statusBackground_);
      window_.draw(statusText_);
    }
    window_.display();
    if (shouldClose_.load() && window_.isOpen()) {
      window_.close();
    }
  }

  void queueStatus(std::string status) {
    {
      const std::lock_guard<std::mutex> lock(statusMutex_);
      pendingStatus_ = std::move(status);
    }
    statusDirty_.store(true);
  }

  int width_;
  int height_;
  std::string title_;
  std::vector<std::uint8_t> pixelBuffer_;
  std::mutex bufferMutex_;
  std::atomic<bool> bufferDirty_{false};
  std::atomic<bool> shouldClose_{false};
  std::string pendingStatus_;
  std::mutex statusMutex_;
  std::atomic<bool> statusDirty_{false};
  sf::RenderWindow window_;
  sf::Texture texture_;
  sf::Sprite sprite_;
  sf::Font font_;
  sf::Text statusText_;
  sf::RectangleShape statusBackground_;
  bool fontLoaded_{false};
  raytracer::common::Logger logger_{"Viewport"};
};

Viewport::Viewport(int width, int height, std::string title)
    : impl_(std::make_unique<Impl>(width, height, std::move(title))) {}

Viewport::~Viewport() = default;

void Viewport::write(const raytracer::components::Image& image,
                     const std::string& path) {
  impl_->uploadPixels(image);
  while (!impl_->shouldClose_.load()) {
    impl_->tick();
    std::this_thread::sleep_for(std::chrono::milliseconds(displayFrameMs));
  }
  if (!path.empty()) {
    image.savePPM(path);
  }
}

bool Viewport::supports(const std::string& ext) const { return ext == ".ppm"; }

void Viewport::liveDisplay(const raytracer::components::Image& image) {
  impl_->uploadPixels(image);
}

bool Viewport::shouldClose() const { return impl_->shouldClose_.load(); }

void Viewport::tick() { impl_->tick(); }

void Viewport::setStatus(std::string status) {
  impl_->queueStatus(std::move(status));
}

int runWithViewport(raytracer::core::RaytracerRenderer& renderer,
                    const raytracer::core::RendererConfig& baseConfig,
                    const raytracer::core::Frame& frame) {
  raytracer::common::Logger logger("ViewportLoop");
  const int width = baseConfig.settings.imageWidth;
  const int height = baseConfig.settings.imageHeight;
  const int targetSamples = std::max(1, baseConfig.settings.samplesPerPixel);
  const int totalPasses = std::max(1, targetSamples / samplesPerPass);
  const int effectiveSamples = totalPasses * samplesPerPass;

  Viewport viewport(width, height, "Raytracer");
  logger.info("viewport opened ", width, 'x', height, ", ", totalPasses,
              " passes of ", samplesPerPass, " samples (", effectiveSamples,
              " total)");

  raytracer::math::RenderSettings perPassSettings = baseConfig.settings;
  perPassSettings.samplesPerPixel = samplesPerPass;
  const raytracer::core::RendererConfig perPassConfig{
      .scene = baseConfig.scene,
      .settings = perPassSettings,
      .shadingContext = baseConfig.shadingContext};

  std::mutex finalImageMutex;
  raytracer::components::Image finalImage(width, height);
  std::atomic<int> completedPasses{0};
  std::atomic<bool> renderDone{false};

  std::thread renderThread([&]() {
    auto loopTimer = logger.scope("viewport accumulation");
    std::vector<raytracer::math::Color> accumulator(
        static_cast<std::size_t>(width) * static_cast<std::size_t>(height));
    raytracer::components::Image displayImage(width, height);
    for (int pass = 1; pass <= totalPasses; ++pass) {
      if (viewport.shouldClose()) {
        logger.info("user closed viewport at pass ", pass - 1, '/',
                    totalPasses);
        break;
      }
      raytracer::components::Image passImage =
          renderer.render(perPassConfig, frame);
      for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
          const std::size_t index =
              (static_cast<std::size_t>(y) * static_cast<std::size_t>(width)) +
              static_cast<std::size_t>(x);
          accumulator[index] =
              accumulator[index] +
              passImage.getPixel(static_cast<std::size_t>(x),
                                 static_cast<std::size_t>(y));
          displayImage.setPixel(
              static_cast<std::size_t>(x), static_cast<std::size_t>(y),
              accumulator[index] / static_cast<double>(pass));
        }
      }
      viewport.liveDisplay(displayImage);
      {
        const std::lock_guard<std::mutex> lock(finalImageMutex);
        finalImage = displayImage;
      }
      completedPasses.store(pass);
      viewport.setStatus("Rendering " +
                         std::to_string(pass * samplesPerPass) + " / " +
                         std::to_string(effectiveSamples) + " samples");
      logger.debug("pass ", pass, '/', totalPasses, " accumulated");
    }
    renderDone.store(true);
  });

  while (!viewport.shouldClose() && !renderDone.load()) {
    viewport.tick();
    std::this_thread::sleep_for(std::chrono::milliseconds(displayFrameMs));
  }

  renderThread.join();

  if (completedPasses.load() == 0) {
    logger.warn(
        "viewport closed before any pass completed; nothing to export");
    return 0;
  }
  const int finalSamples = completedPasses.load() * samplesPerPass;
  if (completedPasses.load() == totalPasses) {
    viewport.setStatus("Done | " + std::to_string(finalSamples) +
                       " samples | close window to save out.ppm");
    logger.info("render finished: ", finalSamples,
                " samples - close window to export");
  } else {
    viewport.setStatus("Interrupted at " + std::to_string(finalSamples) +
                       " / " + std::to_string(effectiveSamples) +
                       " samples | close window to save partial out.ppm");
    logger.info("render interrupted: ", finalSamples, '/', effectiveSamples,
                " samples - close window to export partial result");
  }
  const std::lock_guard<std::mutex> lock(finalImageMutex);
  viewport.write(finalImage, "out.ppm");
  return 0;
}

}  // namespace raytracer::bonus
