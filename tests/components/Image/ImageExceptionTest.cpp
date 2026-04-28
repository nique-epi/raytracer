/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ImageException unit tests
*/

#include <gtest/gtest.h>
#include <string>
#include "components/Image/ImageException.hpp"
#include "constants/Errors.hpp"
#include "core/Exceptions.hpp"

using raytracer::components::ImageException;
using raytracer::components::ImageIOException;

TEST(ImageExceptionTest, IOExceptionIncludesPathAndReason) {
  ImageIOException e("/tmp/out.ppm", "disk full");
  std::string msg = e.what();
  EXPECT_NE(msg.find("/tmp/out.ppm"), std::string::npos);
  EXPECT_NE(msg.find("disk full"), std::string::npos);
  EXPECT_NE(msg.find(raytracer::constants::errors::IMAGE_IO_ERROR), std::string::npos);
}

TEST(ImageExceptionTest, IOExceptionCaughtAsImageException) {
  try {
    throw ImageIOException("p", "r");
  } catch (const ImageException&) {
    SUCCEED();
  } catch (...) {
    FAIL() << "not caught as ImageException";
  }
}

TEST(ImageExceptionTest, IOExceptionCaughtAsRaytracerException) {
  try {
    throw ImageIOException("p", "r");
  } catch (const raytracer::core::RaytracerException&) {
    SUCCEED();
  } catch (...) {
    FAIL() << "not caught as RaytracerException";
  }
}
