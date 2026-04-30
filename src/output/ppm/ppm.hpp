/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PPM
*/

#pragma once

#include "output/IImageWriter.hpp"

/**
 * @brief PPM image output utilities.
 */
namespace raytracer::output {
/**
 * @brief Serialize an image to an ASCII PPM file.
 *
 * @see IImageWriter for the writer contract.
 */
class ppm : public IImageWriter {
 public:
  /**
   * @brief Construct a PPM writer.
   */
  ppm() = default;

  /**
   * @brief Destroy a PPM writer.
   */
  ~ppm() override = default;

  ppm(const ppm& other) = delete;
  ppm& operator=(const ppm& other) = delete;
  ppm(ppm&& other) = delete;
  ppm& operator=(ppm&& other) = delete;

  /**
   * @brief Write an image to a file at @p path.
   *
   * The file is written in ASCII P3 PPM format.
   *
   * @param [in] image Image to serialise.
   * @param [in] path  Destination file path.
   *
   * @throws core::RaytracerException If the output file cannot be opened.
   */
  void write(const raytracer::components::Image& image,
             const std::string& path) override;
  /**
   * @brief Check whether this writer supports a file extension.
   *
   * @param [in] ext File extension to test.
   * @returns true when @p ext is ".ppm".
   */
  [[nodiscard]] bool supports(const std::string& ext) const override;
};
}  // namespace raytracer::output
