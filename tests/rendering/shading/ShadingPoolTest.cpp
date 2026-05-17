/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ShadingPool viewport-mode to pre-built shader lookup
*/

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>

#include "rendering/integrator/whittedIntegrator/WhittedIntegrator.hpp"
#include "rendering/shading/IShadingMode.hpp"
#include "rendering/shading/ShadingException.hpp"
#include "rendering/shading/ShadingPool.hpp"
#include "rendering/shading/materialPreview/MaterialPreviewShader.hpp"
#include "rendering/shading/rendered/RenderedShader.hpp"
#include "rendering/shading/wireframe/WireframeShader.hpp"
#include "scene/World.hpp"

using raytracer::scene::ViewportMode;
using raytracer::shading::IShadingMode;
using raytracer::shading::MaterialPreviewShader;
using raytracer::shading::RenderedShader;
using raytracer::shading::ShadingException;
using raytracer::shading::ShadingPool;
using raytracer::shading::WireframeShader;

namespace {

std::shared_ptr<ShadingPool> buildPool(
    std::shared_ptr<IShadingMode>& outWireframe,
    std::shared_ptr<IShadingMode>& outMaterialPreview,
    std::shared_ptr<IShadingMode>& outRendered) {
  outWireframe = std::make_shared<WireframeShader>();
  outMaterialPreview = std::make_shared<MaterialPreviewShader>();
  outRendered = std::make_shared<RenderedShader>(
      std::make_shared<raytracer::core::WhittedIntegrator>());
  return std::make_shared<ShadingPool>(outWireframe, outMaterialPreview,
                                       outRendered);
}

}  // namespace

TEST(ShadingPoolTest, WireframeModeReturnsTheWireframeShader) {
  std::shared_ptr<IShadingMode> wireframe;
  std::shared_ptr<IShadingMode> materialPreview;
  std::shared_ptr<IShadingMode> rendered;
  const auto pool = buildPool(wireframe, materialPreview, rendered);

  EXPECT_EQ(pool->get(ViewportMode::Wireframe), wireframe);
}

TEST(ShadingPoolTest, MaterialPreviewModeReturnsTheMaterialPreviewShader) {
  std::shared_ptr<IShadingMode> wireframe;
  std::shared_ptr<IShadingMode> materialPreview;
  std::shared_ptr<IShadingMode> rendered;
  const auto pool = buildPool(wireframe, materialPreview, rendered);

  EXPECT_EQ(pool->get(ViewportMode::MaterialPreview), materialPreview);
}

TEST(ShadingPoolTest, RenderedModeReturnsTheRenderedShader) {
  std::shared_ptr<IShadingMode> wireframe;
  std::shared_ptr<IShadingMode> materialPreview;
  std::shared_ptr<IShadingMode> rendered;
  const auto pool = buildPool(wireframe, materialPreview, rendered);

  EXPECT_EQ(pool->get(ViewportMode::Rendered), rendered);
}

TEST(ShadingPoolTest, RepeatedLookupsReturnTheSameInstance) {
  std::shared_ptr<IShadingMode> wireframe;
  std::shared_ptr<IShadingMode> materialPreview;
  std::shared_ptr<IShadingMode> rendered;
  const auto pool = buildPool(wireframe, materialPreview, rendered);

  EXPECT_EQ(pool->get(ViewportMode::Rendered),
            pool->get(ViewportMode::Rendered));
}

TEST(ShadingPoolTest, UnknownModeThrowsShadingException) {
  std::shared_ptr<IShadingMode> wireframe;
  std::shared_ptr<IShadingMode> materialPreview;
  std::shared_ptr<IShadingMode> rendered;
  const auto pool = buildPool(wireframe, materialPreview, rendered);
  const auto unknownMode = static_cast<ViewportMode>(std::uint8_t{99});

  EXPECT_THROW(static_cast<void>(pool->get(unknownMode)), ShadingException);
}

TEST(ShadingPoolTest, NullStrategyInConstructorThrows) {
  EXPECT_THROW(
      ShadingPool(nullptr, std::make_shared<MaterialPreviewShader>(),
                  std::make_shared<RenderedShader>(
                      std::make_shared<raytracer::core::WhittedIntegrator>())),
      ShadingException);
}
