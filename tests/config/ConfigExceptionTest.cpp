/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ConfigException unit tests
*/

#include <gtest/gtest.h>
#include <string>
#include "config/ConfigException.hpp"
#include "constants/Errors.hpp"
#include "exceptions/Exceptions.hpp"

using raytracer::config::ConfigException;

TEST(ConfigExceptionTest, MissingFieldIncludesFieldName) {
  ConfigException e("camera.width");
  std::string msg = e.what();
  EXPECT_NE(msg.find("camera.width"), std::string::npos);
  EXPECT_NE(msg.find(raytracer::constants::errors::CONFIG_FIELD_MISSING),
            std::string::npos);
}

TEST(ConfigExceptionTest, InvalidTypeIncludesFieldAndReason) {
  ConfigException e("camera.fov", "expected float");
  std::string msg = e.what();
  EXPECT_NE(msg.find("camera.fov"), std::string::npos);
  EXPECT_NE(msg.find("expected float"), std::string::npos);
  EXPECT_NE(msg.find(raytracer::constants::errors::CONFIG_TYPE_INVALID),
            std::string::npos);
}

TEST(ConfigExceptionTest, MissingFieldCaughtAsRaytracerException) {
  try {
    throw ConfigException("camera.width");
  } catch (const raytracer::core::RaytracerException&) {
    SUCCEED();
  } catch (...) {
    FAIL() << "not caught as RaytracerException";
  }
}

TEST(ConfigExceptionTest, InvalidTypeCaughtAsRaytracerException) {
  try {
    throw ConfigException("camera.fov", "expected float");
  } catch (const raytracer::core::RaytracerException&) {
    SUCCEED();
  } catch (...) {
    FAIL() << "not caught as RaytracerException";
  }
}
