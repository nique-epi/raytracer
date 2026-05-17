/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ProgressBar
*/

#include "application/ProgressBar.hpp"

#include <unistd.h>

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>

#include "rendering/renderer/raytracerRenderer/RaytracerRenderer.hpp"

namespace raytracer::core {

namespace {

constexpr int progressBarWidth = 30;

struct ProgressBarState {
  std::mutex mutex;
  int lastDrawnPercent{-1};
  std::chrono::steady_clock::time_point lastDrawnAt;
};

std::string formatRemainingTime(std::int64_t remainingSeconds) {
  const auto hours = remainingSeconds / 3600;
  const auto minutes = (remainingSeconds % 3600) / 60;
  const auto seconds = remainingSeconds % 60;

  std::ostringstream stream;
  if (hours > 0) {
    stream << std::setw(2) << std::setfill('0') << hours << ':';
  }
  stream << std::setw(2) << std::setfill('0') << minutes << ':' << std::setw(2)
         << std::setfill('0') << seconds;
  return stream.str();
}

}  // namespace

void attachConsoleProgressBar(RaytracerRenderer& renderer) {
  if (::isatty(::fileno(stderr)) == 0) {
    return;
  }
  const auto state = std::make_shared<ProgressBarState>();
  renderer.setProgressCallback([state](double progress) {
    const int percent = static_cast<int>(progress * 100.0);
    const std::lock_guard<std::mutex> lock(state->mutex);
    if (percent <= state->lastDrawnPercent) {
      return;
    }
    const auto now = std::chrono::steady_clock::now();
    std::string remainingText = "--:--";
    if (state->lastDrawnPercent >= 0 && percent > state->lastDrawnPercent) {
      const auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(
                                      now - state->lastDrawnAt)
                                      .count();
      const int percentDelta = percent - state->lastDrawnPercent;
      const std::int64_t remainingSeconds =
          static_cast<std::int64_t>(elapsedSeconds) * (100 - percent) /
          percentDelta;
      remainingText = formatRemainingTime(remainingSeconds);
    }
    state->lastDrawnPercent = percent;
    state->lastDrawnAt = now;
    const int filled = (percent * progressBarWidth) / 100;
    std::cerr << "\rRendering [";
    for (int i = 0; i < progressBarWidth; ++i) {
      std::cerr << (i < filled ? "█" : "░");
    }
    std::cerr << "] " << std::setw(3) << percent
              << "% remaining=" << remainingText << std::flush;
    if (progress >= 1.0) {
      std::cerr << '\n';
    }
  });
}

}  // namespace raytracer::core
