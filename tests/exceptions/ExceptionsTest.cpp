/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** RaytracerException base unit tests
*/

#include <gtest/gtest.h>
#include <exception>
#include <string>
#include "exceptions/Exceptions.hpp"

using raytracer::core::RaytracerException;

TEST(RaytracerExceptionTest, StoresMessage) {
  RaytracerException e("boom");
  EXPECT_STREQ(e.what(), "boom");
}

TEST(RaytracerExceptionTest, IsStdException) {
  RaytracerException e("oops");
  const std::exception& ref = e;
  EXPECT_STREQ(ref.what(), "oops");
}

TEST(RaytracerExceptionTest, MessageStoredByValue) {
  std::string s = "transient";
  RaytracerException e(s);
  s = "mutated";
  EXPECT_STREQ(e.what(), "transient");
}
