/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ArgsParser unit tests
*/

#include <gtest/gtest.h>
#include <variant>
#include "application/ArgsParser.hpp"

using raytracer::core::AppConfig;
using raytracer::core::ArgsParser;
using raytracer::core::HelpRequest;
using raytracer::core::SceneRequest;

// Given: only the program name (argc=1).
// When:  parse() is called.
// Then:  returns nullopt — too few arguments.
TEST(ArgsParserTest, NoArgsReturnsNullopt) {
  const char* args[] = {"raytracer"};
  EXPECT_FALSE(ArgsParser::parse(1, args).has_value());
}

// Given: three arguments (argc=3).
// When:  parse() is called.
// Then:  returns nullopt — too many arguments.
TEST(ArgsParserTest, TooManyArgsReturnsNullopt) {
  const char* args[] = {"raytracer", "scene.cfg", "extra"};
  EXPECT_FALSE(ArgsParser::parse(3, args).has_value());
}

// Given: argv[1] is "-h".
// When:  parse() is called.
// Then:  returns a HelpRequest variant.
TEST(ArgsParserTest, ShortHelpFlagReturnsHelpRequest) {
  const char* args[] = {"raytracer", "-h"};
  const auto result = ArgsParser::parse(2, args);
  ASSERT_TRUE(result.has_value());
  EXPECT_TRUE(std::holds_alternative<HelpRequest>(*result));
}

// Given: argv[1] is "--help".
// When:  parse() is called.
// Then:  returns a HelpRequest variant.
TEST(ArgsParserTest, LongHelpFlagReturnsHelpRequest) {
  const char* args[] = {"raytracer", "--help"};
  const auto result = ArgsParser::parse(2, args);
  ASSERT_TRUE(result.has_value());
  EXPECT_TRUE(std::holds_alternative<HelpRequest>(*result));
}

// Given: a valid scene path argument.
// When:  parse() is called.
// Then:  returns a SceneRequest variant (not nullopt, not HelpRequest).
TEST(ArgsParserTest, ScenePathReturnsSceneRequest) {
  const char* args[] = {"raytracer", "scenes/example.cfg"};
  const auto result = ArgsParser::parse(2, args);
  ASSERT_TRUE(result.has_value());
  EXPECT_TRUE(std::holds_alternative<SceneRequest>(*result));
}

// Given: argv[1] is "scenes/example.cfg".
// When:  parse() is called.
// Then:  the SceneRequest's scenePath equals "scenes/example.cfg".
TEST(ArgsParserTest, ScenePathIsPreservedInSceneRequest) {
  const char* args[] = {"raytracer", "scenes/example.cfg"};
  const auto result = ArgsParser::parse(2, args);
  ASSERT_TRUE(result.has_value());
  ASSERT_TRUE(std::holds_alternative<SceneRequest>(*result));
  EXPECT_EQ(std::get<SceneRequest>(*result).scenePath, "scenes/example.cfg");
}

// Given: argv[1] looks like a flag but is not -h or --help (e.g. "--version").
// When:  parse() is called.
// Then:  it is treated as a scene path (SceneRequest), not a HelpRequest.
TEST(ArgsParserTest, UnknownFlagTreatedAsScenePath) {
  const char* args[] = {"raytracer", "--version"};
  const auto result = ArgsParser::parse(2, args);
  ASSERT_TRUE(result.has_value());
  EXPECT_TRUE(std::holds_alternative<SceneRequest>(*result));
}

// Given: a valid scene path with no extra flags (argc==2).
// When:  parse() is called.
// Then:  useBVH is true (BVH active by default).
TEST(ArgsParserTest, SceneOnlyDefaultsToUseBVHTrue) {
  const char* args[] = {"raytracer", "scenes/example.cfg"};
  const auto result = ArgsParser::parse(2, args);
  ASSERT_TRUE(result.has_value());
  ASSERT_TRUE(std::holds_alternative<SceneRequest>(*result));
  EXPECT_TRUE(std::get<SceneRequest>(*result).useBVH);
}

// Given: a valid scene path followed by "--no-bvh" (argc==3).
// When:  parse() is called.
// Then:  returns a SceneRequest with useBVH set to false.
TEST(ArgsParserTest, NoBVHFlagDisablesBVH) {
  const char* args[] = {"raytracer", "scenes/example.cfg", "--no-bvh"};
  const auto result = ArgsParser::parse(3, args);
  ASSERT_TRUE(result.has_value());
  ASSERT_TRUE(std::holds_alternative<SceneRequest>(*result));
  EXPECT_FALSE(std::get<SceneRequest>(*result).useBVH);
}

// Given: a valid scene path followed by an unknown third argument.
// When:  parse() is called.
// Then:  returns nullopt — unrecognised flags are rejected.
TEST(ArgsParserTest, UnknownThirdArgReturnsNullopt) {
  const char* args[] = {"raytracer", "scenes/example.cfg", "--unknown"};
  EXPECT_FALSE(ArgsParser::parse(3, args).has_value());
}
