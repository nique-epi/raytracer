/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Image
*/

#ifndef COMPONENTS_IMAGE_IMAGE_HPP_
#define COMPONENTS_IMAGE_IMAGE_HPP_

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

  void setPixel(int x, int y, raytracer::math::Color c);
  [[nodiscard]] raytracer::math::Color getPixel(int x, int y) const;

  [[nodiscard]] int getWidth() const;
  [[nodiscard]] int getHeight() const;

  void savePPM(const std::string& path) const;

 private:
  int _width;
  int _height;
  std::vector<raytracer::math::Color> _pixels;
};

}  // namespace raytracer::components

#endif  // COMPONENTS_IMAGE_IMAGE_HPP_
