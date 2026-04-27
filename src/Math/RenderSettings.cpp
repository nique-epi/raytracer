/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** RenderSettings
*/

#include "Math/RenderSettings.hpp"
#include <thread>

namespace Math {

RenderSettings::RenderSettings()
    : numThreads(static_cast<int>(std::thread::hardware_concurrency())) {}

bool RenderSettings::validate() const {
  return imageWidth > 0 && imageHeight > 0 && tileWidth > 0 && tileHeight > 0 &&
         numThreads > 0 && samplesPerPixel > 0 && maxDepth > 0;
}

}  // namespace Math
