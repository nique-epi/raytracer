/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** RenderSettings
*/

#ifndef MATH_RENDERSETTINGS_HPP_
#define MATH_RENDERSETTINGS_HPP_

namespace raytracer::math {

namespace defaults {
constexpr int imageWidth = 400;
constexpr int imageHeight = 200;
constexpr int tileWidth = 32;
constexpr int tileHeight = 32;
constexpr int numThreads = 0;
constexpr int samplesPerPixel = 1;
constexpr int maxDepth = 2;
}  // namespace defaults

class RenderSettings {
 public:
  int imageWidth{defaults::imageWidth};
  int imageHeight{defaults::imageHeight};
  int tileWidth{defaults::tileWidth};
  int tileHeight{defaults::tileHeight};
  int numThreads{defaults::numThreads};
  int samplesPerPixel{defaults::samplesPerPixel};
  int maxDepth{defaults::maxDepth};

  RenderSettings();

  RenderSettings(const RenderSettings&) = default;
  RenderSettings& operator=(const RenderSettings&) = default;
  RenderSettings(RenderSettings&&) = default;
  RenderSettings& operator=(RenderSettings&&) = default;
  ~RenderSettings() = default;

  [[nodiscard]] bool validate() const;
};

}  // namespace raytracer::math

#endif  // MATH_RENDERSETTINGS_HPP_
