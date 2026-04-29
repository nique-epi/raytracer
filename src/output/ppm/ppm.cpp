/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PPM
*/

#include "output/ppm/ppm.hpp"
#include <fstream>
#include "constants/Errors.hpp"
#include "core/Exceptions.hpp"

namespace raytracer::output {
void ppm::write(const raytracer::components::Image& image,
                const std::string& path) {
  std::ofstream outputFile(path);
  if (!outputFile.is_open()) {
    throw core::RaytracerException(
        raytracer::constants::errors::IMAGE_CANNOT_WRITE);
  }
  outputFile << "P3\n";
  outputFile << image.getWidth() << " " << image.getHeight() << "\n";
  outputFile << "255\n";
  for (std::size_t y = 0; y < image.getHeight(); y++) {
    for (std::size_t x = 0; x < image.getWidth(); x++) {
      const auto [r, g, b] = image.getPixel(x, y).toRGB();
      outputFile << r << " " << g << " " << b << "\n";
    }
  }
  outputFile.close();
}

bool ppm::supports(const std::string& ext) const { return ext == ".ppm"; }
}  // namespace raytracer::output
