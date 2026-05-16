/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Viewport
*/

#include "interface/Viewport.hpp"
#include <SFML/Window.hpp>
#include <chrono>
#include <cstdint>
#include <thread>
#include "interface/ViewportException.hpp"

namespace raytracer::interface {

Viewport::Viewport(int width, int height, const std::string& title)
    : framebuffer_(width, height),
      window_(sf::VideoMode(static_cast<unsigned int>(width),
                            static_cast<unsigned int>(height)),
              title) {
  if (!texture_.create(static_cast<unsigned int>(width),
                       static_cast<unsigned int>(height))) {
    throw ViewportException("Viewport: failed to create SFML texture");
  }
  sprite_.setTexture(texture_, true);
}

void Viewport::write(const raytracer::components::Image& image,
                     const std::string& path) {
  if (!path.empty()) {
    const auto dot = path.rfind('.');
    const std::string extension =
        dot == std::string::npos ? std::string{} : path.substr(dot);
    if (!supports(extension)) {
      throw ViewportException(
          "Viewport::write: unsupported output extension in '" + path +
          "', only .ppm is written");
    }
  }
  framebuffer_.store(image);
  while (!shouldClose_.load()) {
    tick();
    std::this_thread::sleep_for(std::chrono::milliseconds(displayFrameMs));
  }
  if (!path.empty()) {
    image.savePPM(path);
  }
}

bool Viewport::supports(const std::string& ext) const { return ext == ".ppm"; }

void Viewport::liveDisplay(const raytracer::components::Image& image) {
  framebuffer_.store(image);
}

bool Viewport::shouldClose() const { return shouldClose_.load(); }

void Viewport::setStatus(std::string status) {
  overlay_.setStatus(std::move(status));
}

void Viewport::setViewportMode(raytracer::scene::ViewportMode mode) {
  currentMode_.store(mode);
}

std::optional<raytracer::scene::ViewportMode> Viewport::consumeModeRequest() {
  const int raw = pendingMode_.exchange(-1);
  if (raw < 0) {
    return std::nullopt;
  }
  return static_cast<raytracer::scene::ViewportMode>(raw);
}

bool Viewport::hasModeRequest() const { return pendingMode_.load() >= 0; }

void Viewport::requestViewportMode(raytracer::scene::ViewportMode mode) {
  currentMode_.store(mode);
  pendingMode_.store(static_cast<int>(mode));
}

void Viewport::handleKeyPress(sf::Keyboard::Key key) {
  if (key == sf::Keyboard::Tab) {
    requestViewportMode(
        raytracer::scene::nextViewportMode(currentMode_.load()));
  }
}

void Viewport::tick() {
  if (!window_.isOpen()) {
    shouldClose_.store(true);
    return;
  }
  sf::Event event{};
  while (window_.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      shouldClose_.store(true);
    } else if (event.type == sf::Event::KeyPressed) {
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
      handleKeyPress(event.key.code);
    }
  }
  framebuffer_.consume(
      [this](const std::uint8_t* pixels) { texture_.update(pixels); });
  window_.clear(sf::Color::Black);
  window_.draw(sprite_);
  overlay_.draw(window_);
  window_.display();
  if (shouldClose_.load() && window_.isOpen()) {
    window_.close();
  }
}

}  // namespace raytracer::interface
