/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** IImageWriter
*/

#pragma once

#include <string>
#include "components/Image/Image.hpp"

class IImageWriter {
 public:
  IImageWriter() = default;
  virtual ~IImageWriter() = default;

  IImageWriter(const IImageWriter&) = delete;
  IImageWriter& operator=(const IImageWriter&) = delete;
  IImageWriter(IImageWriter&&) = delete;
  IImageWriter& operator=(IImageWriter&&) = delete;

  virtual void write(const raytracer::components::Image& image,
                     const std::string& path) = 0;
  [[nodiscard]] virtual bool supports(const std::string& ext) const = 0;
};
