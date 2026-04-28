/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Image unit tests
*/

#include <gtest/gtest.h>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include "components/Image/Image.hpp"
#include "components/Image/ImageException.hpp"
#include "utils/math/Color.hpp"

using raytracer::components::Image;
using raytracer::components::ImageException;
using raytracer::components::ImageIOException;
using raytracer::math::Color;

TEST(ImageTest, ConstructorAllocatesBlackPixels) {
  Image img(4, 3);
  EXPECT_EQ(img.getWidth(), 4);
  EXPECT_EQ(img.getHeight(), 3);
  for (int y = 0; y < 3; ++y) {
    for (int x = 0; x < 4; ++x) {
      Color c = img.getPixel(x, y);
      EXPECT_DOUBLE_EQ(c.r, 0.0);
      EXPECT_DOUBLE_EQ(c.g, 0.0);
      EXPECT_DOUBLE_EQ(c.b, 0.0);
    }
  }
}

TEST(ImageTest, ConstructorRejectsNegativeDimensions) {
  EXPECT_THROW(Image(-1, 10), ImageException);
  EXPECT_THROW(Image(10, -1), ImageException);
}

TEST(ImageTest, SetGetPixelRoundtrip) {
  Image img(2, 2);
  Color red(1.0, 0.0, 0.0);
  img.setPixel(1, 0, red);
  Color got = img.getPixel(1, 0);
  EXPECT_DOUBLE_EQ(got.r, 1.0);
  EXPECT_DOUBLE_EQ(got.g, 0.0);
  EXPECT_DOUBLE_EQ(got.b, 0.0);
  Color other = img.getPixel(0, 0);
  EXPECT_DOUBLE_EQ(other.r, 0.0);
}

TEST(ImageTest, SetPixelOutOfBoundsThrows) {
  Image img(2, 2);
  Color c(0.5, 0.5, 0.5);
  EXPECT_THROW(img.setPixel(-1, 0, c), ImageException);
  EXPECT_THROW(img.setPixel(0, -1, c), ImageException);
  EXPECT_THROW(img.setPixel(2, 0, c), ImageException);
  EXPECT_THROW(img.setPixel(0, 2, c), ImageException);
}

TEST(ImageTest, GetPixelOutOfBoundsThrows) {
  Image img(2, 2);
  EXPECT_THROW((void)img.getPixel(-1, 0), ImageException);
  EXPECT_THROW((void)img.getPixel(0, -1), ImageException);
  EXPECT_THROW((void)img.getPixel(2, 0), ImageException);
  EXPECT_THROW((void)img.getPixel(0, 2), ImageException);
}

TEST(ImageTest, SavePPMWritesValidP3File) {
  Image img(2, 1);
  img.setPixel(0, 0, Color(1.0, 0.0, 0.0));
  img.setPixel(1, 0, Color(0.0, 1.0, 0.0));

  std::string path = testing::TempDir() + "image_test.ppm";
  img.savePPM(path);

  std::ifstream in(path);
  ASSERT_TRUE(in.good());
  std::stringstream buf;
  buf << in.rdbuf();
  in.close();
  std::remove(path.c_str());

  std::string content = buf.str();
  EXPECT_NE(content.find("P3"), std::string::npos);
  EXPECT_NE(content.find("2 1"), std::string::npos);
  EXPECT_NE(content.find("255"), std::string::npos);
  EXPECT_NE(content.find("255 0 0"), std::string::npos);
  EXPECT_NE(content.find("0 255 0"), std::string::npos);
}

TEST(ImageTest, SavePPMOnInvalidPathThrowsImageIOException) {
  Image img(1, 1);
  EXPECT_THROW(img.savePPM("/nonexistent_dir_xyz/no_perm.ppm"),
               ImageIOException);
}
