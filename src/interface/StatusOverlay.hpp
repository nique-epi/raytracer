/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** StatusOverlay
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <atomic>
#include <cstdint>
#include <mutex>
#include <string>
#include "common/helper/Logger.hpp"

namespace raytracer::interface {

/**
 * @brief Semi-transparent progress text drawn on top of the viewport.
 *
 * `setStatus` is callable from any thread and only queues a string.
 * `draw` must run on the window-owning thread: it applies the latest
 * queued status and renders the overlay. When the font cannot be
 * loaded the overlay disables itself — `draw` becomes a no-op.
 */
class StatusOverlay {
 public:
  /**
   * @brief Load the overlay font and prepare the text and background.
   *
   * Logs a warning and self-disables when the font fails to load.
   */
  StatusOverlay();

  /**
   * @brief Queue @p status as the next overlay text.
   *
   * Safe to call from any thread.
   *
   * @param [in] status New status line.
   */
  void setStatus(std::string status);

  /**
   * @brief Apply the queued status and draw the overlay into @p window.
   *
   * Must be called from the window-owning thread. No-op when the font
   * failed to load.
   *
   * @param [in,out] window Render target to draw into.
   */
  void draw(sf::RenderWindow& window);

 private:
  static constexpr unsigned int fontSize = 18;
  static constexpr float padding = 12.0F;
  static constexpr float margin = 16.0F;
  static constexpr float paddingTotal = padding * 2.0F;
  static constexpr std::uint8_t backgroundAlpha = 160;

  void applyPendingStatus();

  sf::Font font_;
  sf::Text text_;
  sf::RectangleShape background_;
  bool fontLoaded_{false};
  std::string pendingStatus_;
  std::mutex mutex_;
  std::atomic<bool> dirty_{false};
  raytracer::common::Logger logger_{"Viewport"};
};

}  // namespace raytracer::interface
