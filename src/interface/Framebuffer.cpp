/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Framebuffer
*/

#include "interface/Framebuffer.hpp"
#include <cstddef>
#include "utils/math/Color.hpp"

namespace raytracer::interface {

Framebuffer::Framebuffer(int width, int height)
    : pixels_(static_cast<std::size_t>(width) *
                  static_cast<std::size_t>(height) * channels,
              0) {
  for (std::size_t i = channels - 1; i < pixels_.size(); i += channels) {
    pixels_[i] = alphaOpaque;
  }
}

void Framebuffer::store(const raytracer::components::Image& image) {
  const std::size_t width = image.getWidth();
  const std::size_t height = image.getHeight();
  const std::lock_guard<std::mutex> lock(mutex_);
  for (std::size_t y = 0; y < height; ++y) {
    for (std::size_t x = 0; x < width; ++x) {
      const auto [r, g, b] = image.getPixel(x, y).toRGB();
      const std::size_t base = ((y * width) + x) * channels;
      pixels_[base + 0] = static_cast<std::uint8_t>(r);
      pixels_[base + 1] = static_cast<std::uint8_t>(g);
      pixels_[base + 2] = static_cast<std::uint8_t>(b);
      pixels_[base + 3] = alphaOpaque;
    }
  }
  dirty_.store(true);
}

bool Framebuffer::consume(
    const std::function<void(const std::uint8_t*)>& sink) {
  if (!dirty_.exchange(false)) {
    return false;
  }
  const std::lock_guard<std::mutex> lock(mutex_);
  sink(pixels_.data());
  return true;
}

}  // namespace raytracer::interface
