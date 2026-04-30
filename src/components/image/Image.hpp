/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Image
*/

#ifndef COMPONENTS_IMAGE_IMAGE_HPP_
#define COMPONENTS_IMAGE_IMAGE_HPP_

#include <cstddef>
#include <string>
#include <vector>
#include "utils/math/Color.hpp"

namespace raytracer::components {

class Image {
 public:
  Image(int width, int height);

  Image(const Image&) = default;
  Image& operator=(const Image&) = default;
  Image(Image&&) = default;
  Image& operator=(Image&&) = default;
  ~Image() = default;

  void setPixel(std::size_t x, std::size_t y, raytracer::math::Color c);
  [[nodiscard]] raytracer::math::Color getPixel(std::size_t x,
                                                std::size_t y) const;

  [[nodiscard]] std::size_t getWidth() const;
  [[nodiscard]] std::size_t getHeight() const;

  void savePPM(const std::string& path) const;

 private:
  std::size_t _width;
  std::size_t _height;
  std::vector<raytracer::math::Color> _pixels;
};

}  // namespace raytracer::components

#endif  // COMPONENTS_IMAGE_IMAGE_HPP_
