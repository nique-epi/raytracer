/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ShadingPool
*/

#include "rendering/shading/ShadingPool.hpp"

#include <utility>

#include "rendering/shading/IShadingMode.hpp"
#include "rendering/shading/ShadingException.hpp"

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

}  // namespace raytracer::shading
