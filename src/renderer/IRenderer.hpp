/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** IRenderer
*/

#pragma once

#include <functional>
#include "components/camera/ICamera.hpp"
#include "components/image/Image.hpp"
#include "integrator/IIntegrator.hpp"
#include "scene/Scene.hpp"
#include "utils/math/RenderSettings.hpp"

namespace raytracer::core {

using Scene = raytracer::scene::Scene;
using IIntegrator = raytracer::core::IIntegrator;

class IRenderer {
 public:
  virtual ~IRenderer() = default;

  virtual components::Image render(const Scene& scene, const ICamera& camera,
                                   const math::RenderSettings& settings) = 0;
  virtual void setProgressCallback(std::function<void(double)> fn) = 0;

 private:
  std::shared_ptr<IIntegrator> _integrator;
};

}  // namespace raytracer::core
