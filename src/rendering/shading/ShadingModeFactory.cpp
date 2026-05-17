/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ShadingModeFactory
*/

#include "rendering/shading/ShadingModeFactory.hpp"

#include <memory>

#include "rendering/integrator/whittedIntegrator/WhittedIntegrator.hpp"
#include "rendering/shading/IShadingMode.hpp"
#include "rendering/shading/ShadingException.hpp"
#include "rendering/shading/materialPreview/MaterialPreviewShader.hpp"
#include "rendering/shading/rendered/RenderedShader.hpp"
#include "rendering/shading/wireframe/WireframeShader.hpp"

namespace raytracer::shading {

std::shared_ptr<IShadingMode> ShadingModeFactory::create(
    scene::ViewportMode mode) {
  switch (mode) {
    case scene::ViewportMode::Wireframe:
      return std::make_shared<WireframeShader>();
    case scene::ViewportMode::MaterialPreview:
      return std::make_shared<MaterialPreviewShader>();
    case scene::ViewportMode::Rendered:
      return std::make_shared<RenderedShader>(
          std::make_shared<core::WhittedIntegrator>());
  }
  throw ShadingException("ShadingModeFactory::create: unknown ViewportMode");
}

}  // namespace raytracer::shading
