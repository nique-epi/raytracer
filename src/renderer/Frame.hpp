/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Frame
*/

#pragma once

#include "components/camera/ICamera.hpp"

namespace raytracer::core {

/**
 * @brief Per-frame inputs (and future mutable state) for the renderer.
 *
 * Built fresh at each `render()` call. Today only carries the camera, which
 * may change between frames in a realtime loop. Future fields will include
 * `frameIndex`, an accumulator pointer for progressive rendering, and any
 * temporal-history state. When mutable fields land here, `IRenderer::render`
 * should switch from `const Frame&` to `Frame&`.
 */
struct Frame {
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-const-or-ref-data-members)
  const ICamera& camera;
};

}  // namespace raytracer::core
