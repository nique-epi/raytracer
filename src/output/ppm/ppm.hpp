/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PPM
*/

#pragma once

#include "output/IImageWriter.hpp"

namespace raytracer::output {
class ppm : public IImageWriter {
 public:
  ppm() = default;
  ~ppm() override = default;

  ppm(const ppm& other) = delete;
  ppm& operator=(const ppm& other) = delete;
  ppm(ppm&& other) = delete;
  ppm& operator=(ppm&& other) = delete;

  void write(const raytracer::components::Image& image,
             const std::string& path) override;
  [[nodiscard]] bool supports(const std::string& ext) const override;
};
}  // namespace raytracer::output
