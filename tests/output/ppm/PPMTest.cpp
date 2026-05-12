/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PPM unit tests
*/

#include <gtest/gtest.h>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include "../fixture/PPMFixture.hpp"
#include "constants/Errors.hpp"
#include "exceptions/Exceptions.hpp"

TEST_F(PPMFixture, SupportsOnlyPpmExtension) {
  EXPECT_TRUE(writer.supports(".ppm"));
  EXPECT_FALSE(writer.supports(".png"));
  EXPECT_FALSE(writer.supports("ppm"));
  EXPECT_FALSE(writer.supports(".PPM"));
}

TEST_F(PPMFixture, WriteCreatesValidP3File) {
  writer.write(validImage, path);

  std::ifstream in(path);
  ASSERT_TRUE(in.good());

  std::stringstream buffer;
  buffer << in.rdbuf();
  in.close();
  std::remove(path.c_str());

  std::istringstream content(buffer.str());
  std::string line;

  ASSERT_TRUE(std::getline(content, line));
  EXPECT_EQ(line, "P3");

  ASSERT_TRUE(std::getline(content, line));
  EXPECT_EQ(line, "2 1");

  ASSERT_TRUE(std::getline(content, line));
  EXPECT_EQ(line, "255");

  ASSERT_TRUE(std::getline(content, line));
  EXPECT_EQ(line, "255 0 0");

  ASSERT_TRUE(std::getline(content, line));
  EXPECT_EQ(line, "0 255 0");
}

TEST_F(PPMFixture, WriteOnInvalidPathThrowsRaytracerException) {
  try {
    writer.write(invalidImage, "/nonexistent_dir_xyz/no_perm.ppm");
    FAIL() << "Expected write to throw";
  } catch (const raytracer::core::RaytracerException& e) {
    std::string msg = e.what();
    EXPECT_NE(msg.find(raytracer::constants::errors::IMAGE_CANNOT_WRITE),
              std::string::npos);
  }
}
