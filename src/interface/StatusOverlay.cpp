/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** StatusOverlay
*/

#include "interface/StatusOverlay.hpp"
#include <utility>

#ifndef VIEWPORT_FONT_PATH
#error "VIEWPORT_FONT_PATH must be defined by CMake"
#endif

namespace raytracer::interface {

StatusOverlay::StatusOverlay() {
  fontLoaded_ = font_.loadFromFile(VIEWPORT_FONT_PATH);
  if (!fontLoaded_) {
    logger_.warn("failed to load font at ", VIEWPORT_FONT_PATH,
                 " - status overlay disabled");
    return;
  }
  text_.setFont(font_);
  text_.setCharacterSize(fontSize);
  text_.setFillColor(sf::Color::White);
  text_.setPosition(margin + padding, margin + padding);
  background_.setFillColor(sf::Color(0, 0, 0, backgroundAlpha));
  background_.setPosition(margin, margin);
}

void StatusOverlay::setStatus(std::string status) {
  {
    const std::lock_guard<std::mutex> lock(mutex_);
    pendingStatus_ = std::move(status);
  }
  dirty_.store(true);
}

void StatusOverlay::applyPendingStatus() {
  std::string next;
  {
    const std::lock_guard<std::mutex> lock(mutex_);
    next = pendingStatus_;
  }
  text_.setString(sf::String::fromUtf8(next.begin(), next.end()));
  const sf::FloatRect bounds = text_.getLocalBounds();
  background_.setSize(
      sf::Vector2f(bounds.width + paddingTotal,
                   static_cast<float>(fontSize) + paddingTotal));
}

void StatusOverlay::draw(sf::RenderWindow& window) {
  if (!fontLoaded_) {
    return;
  }
  if (dirty_.exchange(false)) {
    applyPendingStatus();
  }
  window.draw(background_);
  window.draw(text_);
}

}  // namespace raytracer::interface
