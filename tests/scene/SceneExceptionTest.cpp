/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SceneException unit tests
*/

#include <gtest/gtest.h>
#include <string>
#include "constants/Errors.hpp"
#include "exceptions/Exceptions.hpp"
#include "scene/SceneException.hpp"

using raytracer::scene::SceneBuildException;
using raytracer::scene::SceneFileNotFoundException;
using raytracer::scene::SceneLoaderException;
using raytracer::scene::SceneParseException;

TEST(SceneExceptionTest, FileNotFoundIncludesPath) {
  SceneFileNotFoundException e("/tmp/missing.cfg");
  std::string msg = e.what();
  EXPECT_NE(msg.find("/tmp/missing.cfg"), std::string::npos);
  EXPECT_NE(msg.find(raytracer::constants::errors::SCENE_FILE_NOT_FOUND),
            std::string::npos);
}

TEST(SceneExceptionTest, ParseExceptionIncludesPathAndReason) {
  SceneParseException e("/tmp/scene.cfg", "unexpected token");
  std::string msg = e.what();
  EXPECT_NE(msg.find("/tmp/scene.cfg"), std::string::npos);
  EXPECT_NE(msg.find("unexpected token"), std::string::npos);
  EXPECT_NE(msg.find(raytracer::constants::errors::SCENE_PARSE_ERROR),
            std::string::npos);
}

TEST(SceneExceptionTest, ParseExceptionWithLineIncludesLineNumber) {
  SceneParseException e("/tmp/scene.cfg", 42, "unexpected token");
  std::string msg = e.what();
  EXPECT_NE(msg.find("/tmp/scene.cfg"), std::string::npos);
  EXPECT_NE(msg.find("42"), std::string::npos);
  EXPECT_NE(msg.find("unexpected token"), std::string::npos);
}

TEST(SceneExceptionTest, FileNotFoundCaughtAsSceneLoaderException) {
  try {
    throw SceneFileNotFoundException("/tmp/missing.cfg");
  } catch (const SceneLoaderException&) {
    SUCCEED();
  } catch (...) {
    FAIL() << "not caught as SceneLoaderException";
  }
}

TEST(SceneExceptionTest, ParseExceptionCaughtAsSceneLoaderException) {
  try {
    throw SceneParseException("/tmp/scene.cfg", "err");
  } catch (const SceneLoaderException&) {
    SUCCEED();
  } catch (...) {
    FAIL() << "not caught as SceneLoaderException";
  }
}

TEST(SceneExceptionTest, FileNotFoundCaughtAsRaytracerException) {
  try {
    throw SceneFileNotFoundException("/tmp/missing.cfg");
  } catch (const raytracer::core::RaytracerException&) {
    SUCCEED();
  } catch (...) {
    FAIL() << "not caught as RaytracerException";
  }
}

TEST(SceneExceptionTest, SceneBuildExceptionCaughtAsRaytracerException) {
  try {
    throw SceneBuildException("no camera defined");
  } catch (const raytracer::core::RaytracerException&) {
    SUCCEED();
  } catch (...) {
    FAIL() << "not caught as RaytracerException";
  }
}
