/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ShadingModeFactory viewport-mode to shader mapping
*/

#include <gtest/gtest.h>
#include <memory>
#include "rendering/shading/IShadingMode.hpp"
#include "rendering/shading/ShadingModeFactory.hpp"
#include "rendering/shading/materialPreview/MaterialPreviewShader.hpp"
#include "rendering/shading/rendered/RenderedShader.hpp"
#include "rendering/shading/wireframe/WireframeShader.hpp"
#include "scene/World.hpp"

using raytracer::scene::ViewportMode;
using raytracer::shading::IShadingMode;
using raytracer::shading::MaterialPreviewShader;
using raytracer::shading::RenderedShader;
using raytracer::shading::ShadingModeFactory;
using raytracer::shading::WireframeShader;

TEST(ShadingModeFactoryTest, WireframeModeBuildsWireframeShader) {
  const std::shared_ptr<IShadingMode> shader =
      ShadingModeFactory::create(ViewportMode::Wireframe);

  ASSERT_NE(shader, nullptr);
  EXPECT_NE(std::dynamic_pointer_cast<WireframeShader>(shader), nullptr);
}

TEST(ShadingModeFactoryTest, MaterialPreviewModeBuildsMaterialPreviewShader) {
  const std::shared_ptr<IShadingMode> shader =
      ShadingModeFactory::create(ViewportMode::MaterialPreview);

  ASSERT_NE(shader, nullptr);
  EXPECT_NE(std::dynamic_pointer_cast<MaterialPreviewShader>(shader), nullptr);
}

TEST(ShadingModeFactoryTest, RenderedModeBuildsRenderedShader) {
  const std::shared_ptr<IShadingMode> shader =
      ShadingModeFactory::create(ViewportMode::Rendered);

  ASSERT_NE(shader, nullptr);
  EXPECT_NE(std::dynamic_pointer_cast<RenderedShader>(shader), nullptr);
}

TEST(ShadingModeFactoryTest, EachCallReturnsAFreshInstance) {
  const std::shared_ptr<IShadingMode> first =
      ShadingModeFactory::create(ViewportMode::Rendered);
  const std::shared_ptr<IShadingMode> second =
      ShadingModeFactory::create(ViewportMode::Rendered);

  EXPECT_NE(first, second);
}
