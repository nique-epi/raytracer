/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** OIDDenoiser
*/

#include "OIDDenoiser.hpp"
#include <iostream>
#include <vector>
#include "OID/OpenImageDenoise/oidn.hpp"
#include "components/image/Image.hpp"

OIDDenoiser::OIDDenoiser() = default;

std::vector<float> OIDDenoiser::toFloatArray(
    raytracer::components::Image& image) {
  std::size_t width = image.getWidth();
  std::size_t height = image.getHeight();
  std::size_t total = width * height * 3;
  std::vector<float> floatArray(total);

  for (std::size_t y = 0; y < height; ++y) {
    for (std::size_t x = 0; x < width; ++x) {
      raytracer::math::Color c = image.getPixel(x, y);
      std::size_t idx = (y * width + x) * 3;
      floatArray[idx] = static_cast<float>(c.r);
      floatArray[idx + 1] = static_cast<float>(c.g);
      floatArray[idx + 2] = static_cast<float>(c.b);
    }
  }
  return floatArray;
}

raytracer::components::Image OIDDenoiser::toImage(
    const std::vector<float>& floatArray, int width, int height) {
  raytracer::components::Image image(width, height);

  auto w = static_cast<std::size_t>(width);
  auto h = static_cast<std::size_t>(height);

  for (std::size_t y = 0; y < h; ++y) {
    for (std::size_t x = 0; x < w; ++x) {
      std::size_t idx = (y * w + x) * 3;
      raytracer::math::Color c(floatArray[idx], floatArray[idx + 1],
                               floatArray[idx + 2]);
      image.setPixel(x, y, c);
    }
  }
  return image;
}

void OIDDenoiser::denoise(raytracer::components::Image& image) {
  std::size_t width = image.getWidth();
  std::size_t height = image.getHeight();

  std::vector<float> floatArray = toFloatArray(image);
  float* colorPtr = floatArray.data();

  oidn::DeviceRef device = oidn::newDevice();
  device.commit();
  oidn::FilterRef filter = device.newFilter("RT");
  filter.setImage("color", colorPtr, oidn::Format::Float3, width, height);
  filter.setImage("output", colorPtr, oidn::Format::Float3, width, height);
  filter.set("hdr", true);
  filter.commit();
  filter.execute();
  const char* errorMessage = nullptr;
  if (device.getError(errorMessage) != oidn::Error::None) {
    if (errorMessage != nullptr) {
      std::cerr << "Erreur OIDN : " << errorMessage << '\n';
    } else {
      std::cerr << "Erreur OIDN inconnue" << '\n';
    }
  }
  image =
      toImage(floatArray, static_cast<int>(width), static_cast<int>(height));
}
