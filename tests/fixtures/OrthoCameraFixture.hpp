/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** OrthoCameraFixture
*/

/**
 * @file OrthoCameraFixture.hpp
 * @brief Simple mock camera that shoots orthographic rays for testing.
 */

#ifndef TESTS_FIXTURES_ORTHOCAMERAFIXTURE_HPP_
#define TESTS_FIXTURES_ORTHOCAMERAFIXTURE_HPP_

#include "components/camera/ICamera.hpp"

/**
 * @brief Mock camera that shoots orthographic rays.
 */
class OrthoCameraFixture : public ICamera {
 public:
  OrthoCameraFixture() = default;
  ~OrthoCameraFixture() override = default;

  [[nodiscard]] raytracer::math::Ray getRay(double u, double v) const override;
  void setResolution(int width, int height) override;
};

#endif  // TESTS_FIXTURES_ORTHOCAMERAFIXTURE_HPP_
