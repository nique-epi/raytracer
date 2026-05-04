/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** IRenderer
*/

#ifndef CORE_RENDERER_IRENDERER_HPP_
#define CORE_RENDERER_IRENDERER_HPP_

#include <functional>
#include "components/Primitives/Collection.hpp"
#include "components/camera/ICamera.hpp"
#include "components/image/Image.hpp"
#include "utils/math/RenderSettings.hpp"

namespace raytracer::core {

using Scene = components::Collection;

class IRenderer {
 public:
  IRenderer() = default;
  virtual ~IRenderer() = default;

  IRenderer(const IRenderer&) = delete;
  IRenderer& operator=(const IRenderer&) = delete;
  IRenderer(IRenderer&&) = delete;
  IRenderer& operator=(IRenderer&&) = delete;

  virtual components::Image render(const Scene& scene, const ICamera& camera,
                                   const math::RenderSettings& settings) = 0;
  virtual void setProgressCallback(std::function<void(double)> fn) = 0;
};

}  // namespace raytracer::core

#endif  // CORE_RENDERER_IRENDERER_HPP_
