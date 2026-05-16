/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** viewport
*/

#pragma once

#include <memory>
#include <string>
#include "components/image/Image.hpp"
#include "output/IImageWriter.hpp"
#include "rendering/renderer/Frame.hpp"
#include "rendering/renderer/RendererConfig.hpp"
#include "rendering/renderer/raytracerRenderer/RaytracerRenderer.hpp"

namespace raytracer::bonus {

/**
 * @brief SFML live-display window that doubles as a PPM exporter.
 *
 * The `sf::RenderWindow`, event polling and drawing must all happen on
 * a single thread — on macOS that thread must be the process main
 * thread (Cocoa rejects `NSApplication` mutations from any other).
 * The class therefore expects to be constructed, ticked and destroyed
 * from the caller's main thread; only `liveDisplay` and `shouldClose`
 * are safe to call from worker threads, because they only touch an
 * internal pixel buffer behind a mutex.
 *
 * Concrete `IImageWriter` so an `--viewport` invocation reuses the
 * same writer abstraction as the default PPM path. The actual file
 * export is delegated to `raytracer::components::Image::savePPM`.
 */
class Viewport : public raytracer::output::IImageWriter {
 public:
  /**
   * @brief Construct a viewport and open the SFML window on the
   *        caller's thread.
   *
   * @param [in] width  Window and texture width in pixels.
   * @param [in] height Window and texture height in pixels.
   * @param [in] title  Window title bar caption.
   */
  Viewport(int width, int height, std::string title);

  /**
   * @brief Close the SFML window on the caller's thread.
   */
  ~Viewport() override;

  Viewport(const Viewport&) = delete;
  Viewport& operator=(const Viewport&) = delete;
  Viewport(Viewport&&) = delete;
  Viewport& operator=(Viewport&&) = delete;

  /**
   * @brief Upload the final image, keep the window open until the
   *        user closes it, then export to disk.
   *
   * Must be called from the same thread that constructed the viewport.
   * Pushes @p image to the texture, then enters a tick loop until the
   * user closes the window. If @p path is non-empty the averaged image
   * is then serialised to PPM via `Image::savePPM`.
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
   * @returns true when @p ext is ".ppm" since the export delegate is
   *          `Image::savePPM`.
   */
  [[nodiscard]] bool supports(const std::string& ext) const override;

  /**
   * @brief Push an intermediate accumulator snapshot to the window.
   *
   * Safe to call from any thread: copies the pixel data into an
   * internal buffer protected by a mutex and flags it dirty for the
   * next `tick` call to upload to the GPU.
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
   * Must be called from the same thread that constructed the
   * viewport. Intended to be driven in a loop by the main thread
   * while a worker thread feeds frames through `liveDisplay`.
   */
  void tick();

  /**
   * @brief Queue a new window title to be applied at the next `tick`.
   *
   * Safe to call from any thread: stores the requested title under a
   * mutex, the main thread's `tick` picks it up and calls
   * `sf::Window::setTitle` (which on macOS must run on the main
   * thread).
   *
   * @param [in] status New title bar string.
   */
  void setStatus(std::string status);

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

/**
 * @brief Drive the progressive accumulation render loop with a Viewport.
 *
 * Must be called from the process main thread (macOS Cocoa
 * constraint). Constructs the `Viewport`, then spawns a worker
 * thread that re-renders the scene one sample per pixel per pass,
 * accumulating into a buffer and pushing the running average to the
 * viewport after each pass. Meanwhile the calling thread pumps the
 * SFML event loop. Exits early if the user closes the window. On
 * exit, the (partial or full) averaged image is written to
 * `out.ppm`.
 *
 * @param [in,out] renderer   Renderer to drive (its per-tile scheduler
 *                            is reused as-is, only the sample count is
 *                            overridden to 1 per pass).
 * @param [in] baseConfig     Scene, shading context and target settings.
 * @param [in] frame          Per-frame camera state.
 * @returns 0 on success, non-zero on irrecoverable error.
 */
int runWithViewport(raytracer::core::RaytracerRenderer& renderer,
                    const raytracer::core::RendererConfig& baseConfig,
                    const raytracer::core::Frame& frame);

}  // namespace raytracer::bonus
