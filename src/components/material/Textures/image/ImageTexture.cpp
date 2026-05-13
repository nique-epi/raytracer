/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ImageTexture
*/

#include "ImageTexture.hpp"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "utils/math/Color.hpp"
#include "utils/math/Vector3D.hpp"

#ifdef BUILD_BONUS
#include "stb_image.h"
#endif

// NOLINTBEGIN(llvm-prefer-static-over-anonymous-namespace)
namespace {

#ifdef BUILD_BONUS

bool loadWithStb(const std::string& path,
                 std::vector<raytracer::math::Color>& pixels,
                 std::size_t& outWidth, std::size_t& outHeight) {
  int width = 0;
  int height = 0;
  // NOLINTBEGIN(cppcoreguidelines-init-variables)
  int channels = 0;
  // NOLINTEND(cppcoreguidelines-init-variables)
  constexpr int requestedChannels = 3;

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels,
                                  requestedChannels);
  if (data == nullptr || width <= 0 || height <= 0) {
    stbi_image_free(data);
    return false;
  }

  constexpr double byteScale = 1.0 / 255.0;
  const auto pixelCount =
      static_cast<std::size_t>(width) * static_cast<std::size_t>(height);
  pixels.resize(pixelCount);

  for (std::size_t index = 0; index < pixelCount; ++index) {
    // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
    const std::size_t byteIndex = index * 3;
    // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
    pixels[index] = {
        static_cast<double>(data[byteIndex]) * byteScale,
        static_cast<double>(data[byteIndex + 1]) * byteScale,
        static_cast<double>(data[byteIndex + 2]) * byteScale,
    };
  }

  stbi_image_free(data);
  outWidth = static_cast<std::size_t>(width);
  outHeight = static_cast<std::size_t>(height);
  return true;
}

#endif

}  // namespace
// NOLINTEND(llvm-prefer-static-over-anonymous-namespace)

namespace raytracer::materials::textures {

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
bool ImageTexture::loadFromFile(const std::string& path) {
#ifdef BUILD_BONUS
  std::vector<raytracer::math::Color> pixels;
  std::size_t pixelWidth = 0;
  std::size_t pixelHeight = 0;
  if (!loadWithStb(path, pixels, pixelWidth, pixelHeight)) {
    return false;
  }
  pixels_ = std::move(pixels);
  width_ = pixelWidth;
  height_ = pixelHeight;
  loaded_ = true;
  return true;
#else
  std::cerr << "ImageTexture: bitmap textures require BUILD_BONUS=ON "
               "(recompile with -DBUILD_BONUS=ON)\n";
  (void)path;
  return false;
#endif
}

raytracer::math::Color ImageTexture::sample(
    double u, double v,
    const raytracer::math::Vector3D& /*point*/) const {
  if (!loaded_) {
    // Magenta: visible debug sentinel when no image is loaded
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
    return {1.0, 0.0, 1.0};
  }

  // NOLINTBEGIN(cppcoreguidelines-init-variables)
  const double clampedU = std::clamp(u, 0.0, 1.0);
  const double clampedV = std::clamp(v, 0.0, 1.0);
  // NOLINTEND(cppcoreguidelines-init-variables)

  const auto pixelX = static_cast<std::size_t>(
      clampedU * static_cast<double>(width_ - 1));
  const auto pixelY = static_cast<std::size_t>(
      (1.0 - clampedV) * static_cast<double>(height_ - 1));

  return pixels_[(pixelY * width_) + pixelX];
}

}  // namespace raytracer::materials::textures
