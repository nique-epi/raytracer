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
  std::ofstream outputFile(path, std::ios::binary);
  if (!outputFile.is_open()) {
    throw core::RaytracerException(
        raytracer::constants::errors::IMAGE_CANNOT_WRITE);
    return;
  }
  outputFile << "P3\n";
  outputFile << image.getWidth() << " " << image.getHeight() << "\n";
  outputFile << "255\n";
  for (std::size_t i = 0; i < image.getWidth(); i++) {
    for (std::size_t j = 0; j < image.getHeight(); j++) {
      const auto& pixel = image.getPixel(i, j);
      outputFile << static_cast<int>(pixel.r) << " "
                 << static_cast<int>(pixel.g) << " "
                 << static_cast<int>(pixel.b) << "\n";
    }
  }
  outputFile.close();
}

bool ppm::supports(const std::string& ext) const { return ext == ".ppm"; }
}  // namespace raytracer::output
