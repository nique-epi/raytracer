/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** IRenderer
*/

#pragma once

#include <functional>
#include "components/image/Image.hpp"
#include "renderer/Frame.hpp"
#include "renderer/RendererConfig.hpp"

namespace raytracer::core {

class IRenderer {
 public:
  IRenderer() = default;
  virtual ~IRenderer() = default;
  IRenderer(const IRenderer&) = delete;
  IRenderer& operator=(const IRenderer&) = delete;
  IRenderer(IRenderer&&) = delete;
  IRenderer& operator=(IRenderer&&) = delete;

  virtual components::Image render(const RendererConfig& config,
                                   const Frame& frame) = 0;
  virtual void setProgressCallback(std::function<void(double)> fn) = 0;
};

}  // namespace raytracer::core
