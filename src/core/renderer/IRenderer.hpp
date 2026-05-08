/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** IRenderer
*/

#pragma once

#include <functional>
#include "components/Primitives/Collection.hpp"
#include "components/camera/ICamera.hpp"
#include "components/image/Image.hpp"
#include "scene/Scene.hpp"
#include "utils/math/RenderSettings.hpp"

namespace raytracer::core {

using Scene = raytracer::scene::Scene;

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
