/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** RenderSettings
*/

#include "RenderSettings.hpp"
#include <algorithm>
#include <thread>

namespace raytracer::math {

RenderSettings::RenderSettings()
    : numThreads(
          static_cast<int>(std::max(1U, std::thread::hardware_concurrency()))) {
}

bool RenderSettings::validate() const {
  return imageWidth > 0 && imageHeight > 0 && tileWidth > 0 && tileHeight > 0 &&
         numThreads > 0 && samplesPerPixel > 0 && maxDepth > 0;
}

}  // namespace raytracer::math
