/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ImageTexture unit tests
*/

#include <gtest/gtest.h>
#include <string>
#include "components/material/Textures/image/ImageTexture.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::materials::textures::ImageTexture;
using raytracer::math::Color;
using raytracer::math::Vector3D;

TEST(ImageTextureTest, SampleReturnsMagentaBeforeLoad) {
  ImageTexture texture;
  const Color result = texture.sample(0.5, 0.5, Vector3D(0.0, 0.0, 0.0));

  EXPECT_DOUBLE_EQ(result.r, 1.0);
  EXPECT_DOUBLE_EQ(result.g, 0.0);
  EXPECT_DOUBLE_EQ(result.b, 1.0);
}

TEST(ImageTextureTest, LoadFromFileFailsOnAbsentFile) {
  ImageTexture texture;

  EXPECT_FALSE(texture.loadFromFile("/nonexistent/path/image.ppm"));
}

TEST(ImageTextureTest, SampleIsDeterministic) {
  ImageTexture texture;

  const Color first  = texture.sample(0.3, 0.7, Vector3D(1.0, 2.0, 3.0));
  const Color second = texture.sample(0.3, 0.7, Vector3D(1.0, 2.0, 3.0));

  EXPECT_DOUBLE_EQ(first.r, second.r);
  EXPECT_DOUBLE_EQ(first.g, second.g);
  EXPECT_DOUBLE_EQ(first.b, second.b);
}
