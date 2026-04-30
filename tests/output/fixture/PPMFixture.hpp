/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PPM test fixture
*/

#pragma once

#include <gtest/gtest.h>
#include <cstdio>
#include <string>
#include "components/Image/Image.hpp"
#include "output/ppm/ppm.hpp"
#include "utils/math/Color.hpp"

class PPMFixture : public ::testing::Test {
 protected:
  PPMFixture() : validImage(2, 1), invalidImage(1, 1) {}

  void SetUp() override {
    path = testing::TempDir() + "ppm_writer_test.ppm";

    validImage.setPixel(0, 0, raytracer::math::Color(1.0, 0.0, 0.0));
    validImage.setPixel(1, 0, raytracer::math::Color(0.0, 1.0, 0.0));
  }

  void TearDown() override { std::remove(path.c_str()); }

  raytracer::output::ppm writer;
  raytracer::components::Image validImage;
  raytracer::components::Image invalidImage;
  std::string path;
};
