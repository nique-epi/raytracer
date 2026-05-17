/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ShadingPool
*/

#include "rendering/shading/ShadingPool.hpp"

#include <utility>

#include "rendering/integrator/whittedIntegrator/WhittedIntegrator.hpp"
#include "rendering/shading/IShadingMode.hpp"
#include "rendering/shading/ShadingException.hpp"
#include "rendering/shading/materialPreview/MaterialPreviewShader.hpp"
#include "rendering/shading/rendered/RenderedShader.hpp"
#include "rendering/shading/wireframe/WireframeShader.hpp"

namespace raytracer::shading {

ShadingPool::ShadingPool(std::shared_ptr<IShadingMode> wireframe,
                         std::shared_ptr<IShadingMode> materialPreview,
                         std::shared_ptr<IShadingMode> rendered)
    : wireframe_(std::move(wireframe)),
      materialPreview_(std::move(materialPreview)),
      rendered_(std::move(rendered)) {
  if (!wireframe_ || !materialPreview_ || !rendered_) {
    throw ShadingException(
        "ShadingPool requires three non-null IShadingMode strategies");
  }
}

std::shared_ptr<IShadingMode> ShadingPool::get(
    scene::ViewportMode mode) const {
  switch (mode) {
    case scene::ViewportMode::Wireframe:
      return wireframe_;
    case scene::ViewportMode::MaterialPreview:
      return materialPreview_;
    case scene::ViewportMode::Rendered:
      return rendered_;
  }
  throw ShadingException("ShadingPool::get: unknown ViewportMode");
}

std::shared_ptr<ShadingPool> ShadingPool::create() {
  return std::make_shared<ShadingPool>(
      std::make_shared<WireframeShader>(),
      std::make_shared<MaterialPreviewShader>(),
      std::make_shared<RenderedShader>(
          std::make_shared<raytracer::core::WhittedIntegrator>()));
}

}  // namespace raytracer::shading
