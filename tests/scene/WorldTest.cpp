/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** World viewport-mode state and ViewportMode cycle helpers
*/

#include <gtest/gtest.h>
#include <string>
#include "scene/World.hpp"

using raytracer::scene::nextViewportMode;
using raytracer::scene::ViewportMode;
using raytracer::scene::viewportModeName;
using raytracer::scene::World;

TEST(WorldTest, DefaultViewportModeIsRendered) {
  const World world;
  EXPECT_EQ(world.viewportMode(), ViewportMode::Rendered);
}

TEST(WorldTest, SetViewportModeRoundTrips) {
  World world;

  world.setViewportMode(ViewportMode::Wireframe);
  EXPECT_EQ(world.viewportMode(), ViewportMode::Wireframe);

  world.setViewportMode(ViewportMode::MaterialPreview);
  EXPECT_EQ(world.viewportMode(), ViewportMode::MaterialPreview);

  world.setViewportMode(ViewportMode::Rendered);
  EXPECT_EQ(world.viewportMode(), ViewportMode::Rendered);
}

TEST(WorldTest, NextViewportModeFollowsCycleOrder) {
  EXPECT_EQ(nextViewportMode(ViewportMode::Wireframe),
            ViewportMode::MaterialPreview);
  EXPECT_EQ(nextViewportMode(ViewportMode::MaterialPreview),
            ViewportMode::Rendered);
  EXPECT_EQ(nextViewportMode(ViewportMode::Rendered), ViewportMode::Wireframe);
}

TEST(WorldTest, NextViewportModeReturnsToStartAfterThreeSteps) {
  ViewportMode mode = ViewportMode::Wireframe;

  mode = nextViewportMode(mode);
  mode = nextViewportMode(mode);
  mode = nextViewportMode(mode);

  EXPECT_EQ(mode, ViewportMode::Wireframe);
}

TEST(WorldTest, ViewportModeNameMatchesEachMode) {
  EXPECT_EQ(std::string(viewportModeName(ViewportMode::Wireframe)),
            "Wireframe");
  EXPECT_EQ(std::string(viewportModeName(ViewportMode::MaterialPreview)),
            "Material Preview");
  EXPECT_EQ(std::string(viewportModeName(ViewportMode::Rendered)), "Rendered");
}
