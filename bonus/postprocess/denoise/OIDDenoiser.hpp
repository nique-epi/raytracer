/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** OIDDenoiser
*/

#pragma once

#include <vector>
#include "components/image/Image.hpp"

class OIDDenoiser {
 public:
  OIDDenoiser();
  ~OIDDenoiser();

  OIDDenoiser(const OIDDenoiser&) = delete;
  OIDDenoiser& operator=(const OIDDenoiser&) = delete;
  OIDDenoiser(OIDDenoiser&&) = delete;
  OIDDenoiser& operator=(OIDDenoiser&&) = delete;

    static void denoise(raytracer::components::Image& image);

 protected:
 private:
    static std::vector<float> toFloatArray(raytracer::components::Image& image);
    static raytracer::components::Image toImage(const std::vector<float>& floatArray,
                                                                             int width, int height);
};
