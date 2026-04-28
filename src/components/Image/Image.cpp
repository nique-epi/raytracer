/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Image
*/

#include "Image.hpp"
#include <fstream>
#include "ImageException.hpp"
#include "constants/Errors.hpp"

namespace {
std::size_t pixelCount(int width, int height) {
  if (width < 0 || height < 0) {
    throw Components::ImageException(
        Constants::Errors::IMAGE_NEGATIVE_DIMENSIONS);
  }
  return static_cast<std::size_t>(width) * static_cast<std::size_t>(height);
}
}  // namespace

namespace Components {

Image::Image(int width, int height)
    : _width(width), _height(height), _pixels(pixelCount(width, height)) {}

void Image::setPixel(int x, int y, Math::Color c) {
  if (x < 0 || x >= _width || y < 0 || y >= _height) {
    throw ImageException(Constants::Errors::IMAGE_PIXEL_OUT_OF_BOUNDS);
  }
  _pixels[(static_cast<std::size_t>(y) * static_cast<std::size_t>(_width)) +
          static_cast<std::size_t>(x)] = c;
}

Math::Color Image::getPixel(int x, int y) const {
  if (x < 0 || x >= _width || y < 0 || y >= _height) {
    throw ImageException(Constants::Errors::IMAGE_PIXEL_OUT_OF_BOUNDS);
  }
  return _pixels[(static_cast<std::size_t>(y) *
                  static_cast<std::size_t>(_width)) +
                 static_cast<std::size_t>(x)];
}

int Image::getWidth() const { return _width; }

int Image::getHeight() const { return _height; }

void Image::savePPM(const std::string& path) const {
  std::ofstream out;
  out.exceptions(std::ofstream::failbit | std::ofstream::badbit);

  try {
    out.open(path);
    out << "P3\n" << _width << ' ' << _height << "\n255\n";
    for (const auto& pixel : _pixels) {
      auto [r, g, b] = pixel.toRGB();
      out << r << ' ' << g << ' ' << b << '\n';
    }
    out.close();
  } catch (const std::ofstream::failure& e) {
    throw ImageIOException(path, e.what());
  }
}

}  // namespace Components
