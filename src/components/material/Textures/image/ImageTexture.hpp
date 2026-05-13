/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ImageTexture
*/

#pragma once

#include <string>
#include <vector>
#include "components/material/Textures/ITexture.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Vector3D.hpp"

namespace raytracer::materials::textures {

/**
 * @brief Bitmap image texture mapped onto surfaces via UV coordinates.
 *
 * Samples are looked up by clamping (u, v) coordinates to [0, 1] and
 * mapping them to pixel indices: x = u*(width-1), y = (1-v)*(height-1).
 * If no image has been successfully loaded, @c sample() returns magenta
 * (1, 0, 1) as a visible debug indicator.
 *
 * @note Image loading requires the project to be compiled with
 *       @c BUILD_BONUS=ON. Without it, @c loadFromFile() always returns
 *       @c false. With @c BUILD_BONUS=ON, supported formats are JPEG, PNG,
 *       BMP, TGA and PPM (binary P6) via stb_image.
 */
class ImageTexture : public raytracer::materials::ITexture {
 public:
  ImageTexture() = default;
  ~ImageTexture() override = default;

  ImageTexture(const ImageTexture&) = delete;
  ImageTexture& operator=(const ImageTexture&) = delete;
  ImageTexture(ImageTexture&&) = delete;
  ImageTexture& operator=(ImageTexture&&) = delete;

  /**
   * @brief Load a bitmap image from disk.
   *
   * Requires @c BUILD_BONUS=ON (stb_image). Supported formats: JPEG, PNG,
   * BMP, TGA, PPM (binary P6). Always returns @c false without the bonus.
   *
   * @param [in] path Filesystem path to the image file.
   * @returns @c true if the file was loaded successfully.
   * @returns @c false if loading failed or @c BUILD_BONUS is not enabled.
   */
  bool loadFromFile(const std::string& path);

  /**
   * @brief Sample the texture at the given UV coordinates.
   *
   * UV coordinates are clamped to [0, 1]. The vertical axis is flipped so
   * that v = 0 maps to the bottom of the image.
   * Returns magenta (1, 0, 1) if no image has been loaded.
   *
   * @param [in] u     Horizontal texture coordinate (clamped to [0, 1]).
   * @param [in] v     Vertical texture coordinate (clamped to [0, 1]).
   * @param [in] point 3-D hit position (unused — UV mapping only).
   * @returns The color at (u, v), or magenta if the texture is not loaded.
   */
  [[nodiscard]] raytracer::math::Color sample(
      double u, double v,
      const raytracer::math::Vector3D& point) const override;

 private:
  std::vector<raytracer::math::Color> pixels_;
  std::size_t width_ = 0;
  std::size_t height_ = 0;
  bool loaded_ = false;
};

}  // namespace raytracer::materials::textures
