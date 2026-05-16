/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Viewport
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <atomic>
#include <string>
#include "components/image/Image.hpp"
#include "interface/Framebuffer.hpp"
#include "interface/StatusOverlay.hpp"
#include "output/IImageWriter.hpp"

namespace raytracer::interface {

/**
 * @brief SFML live-display window that doubles as a PPM exporter.
 *
 * The `sf::RenderWindow`, event polling and drawing must all happen on
 * a single thread — on macOS that thread must be the process main
 * thread (Cocoa rejects `NSApplication` mutations from any other).
 * The class therefore expects to be constructed, ticked and destroyed
 * from the caller's main thread; only `liveDisplay`, `setStatus` and
 * `shouldClose` are safe to call from worker threads, because they
 * only touch the mutex-protected `Framebuffer` / `StatusOverlay`.
 *
 * Concrete `IImageWriter` so the live-display path reuses the same
 * writer abstraction as the default PPM path. The actual file export
 * is delegated to `raytracer::components::Image::savePPM`.
 */
class Viewport : public raytracer::output::IImageWriter {
 public:
  /// Frame interval in milliseconds for the tick / pump loops.
  static constexpr int displayFrameMs = 16;

  /**
   * @brief Open the SFML window on the caller's thread.
   *
   * @param [in] width  Window and texture width in pixels.
   * @param [in] height Window and texture height in pixels.
   * @param [in] title  Window title bar caption.
   */
  Viewport(int width, int height, const std::string& title);

  /**
   * @brief Upload the final image, keep the window open until the
   *        user closes it, then export to disk.
   *
   * Must be called from the same thread that constructed the viewport.
   * If @p path is non-empty the image is serialised to PPM via
   * `Image::savePPM` once the window is closed.
   *
   * @param [in] image Final image to display and export.
   * @param [in] path  Destination path; empty means "no export".
   */
  void write(const raytracer::components::Image& image,
             const std::string& path) override;

  /**
   * @brief Whether this writer claims the given extension.
   *
   * @param [in] ext File extension (lower-case, leading dot).
   * @returns true when @p ext is ".ppm".
   */
  [[nodiscard]] bool supports(const std::string& ext) const override;

  /**
   * @brief Push an intermediate accumulator snapshot to the window.
   *
   * Safe to call from any thread.
   *
   * @param [in] image Latest averaged image to show.
   */
  void liveDisplay(const raytracer::components::Image& image);

  /**
   * @brief Whether the user has requested the window to close.
   *
   * Safe to call from any thread.
   *
   * @returns true once the SFML window has received a close event.
   */
  [[nodiscard]] bool shouldClose() const;

  /**
   * @brief Pump events, upload pending pixels and redraw once.
   *
   * Must be called from the thread that constructed the viewport.
   */
  void tick();

  /**
   * @brief Queue a new status overlay line.
   *
   * Safe to call from any thread.
   *
   * @param [in] status New status line.
   */
  void setStatus(std::string status);

 private:
  Framebuffer framebuffer_;
  StatusOverlay overlay_;
  sf::RenderWindow window_;
  sf::Texture texture_;
  sf::Sprite sprite_;
  std::atomic<bool> shouldClose_{false};
};

}  // namespace raytracer::interface
