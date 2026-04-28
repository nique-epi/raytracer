/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PluginException unit tests
*/

#include <gtest/gtest.h>
#include <string>
#include "constants/Errors.hpp"
#include "core/Exceptions.hpp"
#include "plugin/PluginException.hpp"

using raytracer::plugin::PluginException;
using raytracer::plugin::PluginLoadException;
using raytracer::plugin::PluginSymbolException;

TEST(PluginExceptionTest, LoadExceptionIncludesPathAndReason) {
  PluginLoadException e("/path/to/plug.so", "file not found");
  std::string msg = e.what();
  EXPECT_NE(msg.find("/path/to/plug.so"), std::string::npos);
  EXPECT_NE(msg.find("file not found"), std::string::npos);
  EXPECT_NE(msg.find(raytracer::constants::errors::PLUGIN_LOAD_FAILED), std::string::npos);
}

TEST(PluginExceptionTest, SymbolExceptionIncludesSymbolAndPath) {
  PluginSymbolException e("/lib/foo.so", "create");
  std::string msg = e.what();
  EXPECT_NE(msg.find("/lib/foo.so"), std::string::npos);
  EXPECT_NE(msg.find("create"), std::string::npos);
  EXPECT_NE(msg.find(raytracer::constants::errors::PLUGIN_SYMBOL_NOT_FOUND),
            std::string::npos);
}

TEST(PluginExceptionTest, LoadExceptionCaughtAsPluginException) {
  try {
    throw PluginLoadException("a", "b");
  } catch (const PluginException&) {
    SUCCEED();
  } catch (...) {
    FAIL() << "not caught as PluginException";
  }
}

TEST(PluginExceptionTest, SymbolExceptionCaughtAsRaytracerException) {
  try {
    throw PluginSymbolException("a", "b");
  } catch (const raytracer::core::RaytracerException&) {
    SUCCEED();
  } catch (...) {
    FAIL() << "not caught as RaytracerException";
  }
}
