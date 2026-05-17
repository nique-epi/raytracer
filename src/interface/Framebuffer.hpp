/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Framebuffer
*/

#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <mutex>
#include <vector>
#include "components/image/Image.hpp"

namespace raytracer::interface {

/**
 * @brief Thread-safe RGBA8 pixel buffer bridging the render and UI threads.
 *
 * The render thread calls `store` to publish a fresh image; the UI
 * thread calls `consume` to hand the latest pixels to a sink exactly
 * once per change. Both operations are mutex-protected, and a dirty
 * flag lets the UI thread skip redundant uploads.
 */
class Framebuffer {
 public:
  /**
   * @brief Allocate an opaque-black buffer of @p width × @p height pixels.
   *
   * @param [in] width  Buffer width in pixels.
   * @param [in] height Buffer height in pixels.
   */
  Framebuffer(int width, int height);

  /**
   * @brief Publish @p image as the latest frame.
   *
   * Safe to call from any thread. Converts the linear-space colours to
   * RGBA8 and flags the buffer dirty.
   *
   * @param [in] image Image to publish.
   */
  void store(const raytracer::components::Image& image);

  /**
   * @brief Hand the latest pixels to @p sink if new ones were published.
   *
   * @param [in] sink Invoked under lock with a pointer to width×height
   *                  RGBA8 bytes when the buffer is dirty.
   * @returns true when @p sink ran (the buffer was dirty), false
   *          otherwise.
   */
  bool consume(const std::function<void(const std::uint8_t*)>& sink);

 private:
  static constexpr std::size_t channels = 4;
  static constexpr std::uint8_t alphaOpaque = 255;

  std::vector<std::uint8_t> pixels_;
  std::mutex mutex_;
  std::atomic<bool> dirty_{false};
};

}  // namespace raytracer::interface
