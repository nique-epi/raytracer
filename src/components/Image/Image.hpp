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

namespace Components {

class Image {
 public:
  Image(int width, int height);

  Image(const Image&) = default;
  Image& operator=(const Image&) = default;
  Image(Image&&) = default;
  Image& operator=(Image&&) = default;
  ~Image() = default;

  void setPixel(int x, int y, Math::Color c);
  [[nodiscard]] Math::Color getPixel(int x, int y) const;

  [[nodiscard]] int getWidth() const;
  [[nodiscard]] int getHeight() const;

  void savePPM(const std::string& path) const;

 private:
  int _width;
  int _height;
  std::vector<Math::Color> _pixels;
};

}  // namespace Components

#endif  // COMPONENTS_IMAGE_IMAGE_HPP_
