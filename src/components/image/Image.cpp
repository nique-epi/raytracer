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
    throw raytracer::components::ImageException(
        raytracer::constants::errors::IMAGE_NEGATIVE_DIMENSIONS);
  }
  return static_cast<std::size_t>(width) * static_cast<std::size_t>(height);
}
}  // namespace

namespace raytracer::components {

Image::Image(int width, int height)
    : _width(width), _height(height), _pixels(pixelCount(width, height)) {}

void Image::setPixel(std::size_t x, std::size_t y, raytracer::math::Color c) {
  if (x >= _width || y >= _height) {
    throw ImageException(
        raytracer::constants::errors::IMAGE_PIXEL_OUT_OF_BOUNDS);
  }
  _pixels[(y * _width) + x] = c;
}

raytracer::math::Color Image::getPixel(std::size_t x, std::size_t y) const {
  if (x >= _width || y >= _height) {
    throw ImageException(
        raytracer::constants::errors::IMAGE_PIXEL_OUT_OF_BOUNDS);
  }
  return _pixels[(y * _width) + x];
}

std::size_t Image::getWidth() const { return _width; }

std::size_t Image::getHeight() const { return _height; }

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

}  // namespace raytracer::components
