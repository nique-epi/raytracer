/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** World
*/

#pragma once

#include <cstdint>

namespace raytracer::scene {

/**
 * @brief Shading strategy used to render the scene, mirroring Blender's
 *        3D viewport shading dropdown.
 *
 * Each mode is a complete, self-contained pipeline. They are not flags:
 * picking one disables the behaviour of the others.
 *
 *  - Wireframe: geometry preview only. No material colour, no lighting,
 *    no shadows. The surface normal is mapped to RGB so the silhouette
 *    and curvature of every primitive is immediately readable. Useful
 *    for verifying object placement and transforms without paying for
 *    shading work.
 *
 *  - MaterialPreview: a hands-off preview that ignores every scene
 *    ILight, mirroring Blender where "Scene Lights" defaults to OFF in
 *    this mode. The scene background is used as an infinite environment
 *    light (sampled on both primary and secondary rays). A dedicated
 *    issue will later substitute this by a built-in studio HDRI to
 *    match Blender's behaviour where "Scene World" is also OFF and a
 *    bundled forest.exr is used instead.
 *
 *  - Rendered: full Whitted-style shading with all scene ILights
 *    contributing direct lighting and drop shadows. Scatter rebounds
 *    feed indirect light, and the background is only visible to primary
 *    rays (it does not contribute radiance to indirect bounces). This
 *    is the final-image path.
 */
enum class ViewportMode : std::uint8_t {
  Wireframe,
  MaterialPreview,
  Rendered,
};

/**
 * @brief Scene-level environment settings, mirroring Blender's World
 *        data block.
 *
 * The World currently holds the active viewport mode that drives the
 * renderer's shading strategy. It lives on Scene (next to background,
 * camera, lights) rather than in RenderSettings, because the shading
 * strategy is a property of the scene description — it belongs in the
 * scene .cfg file, not in the output image settings.
 */
class World {
 public:
  World();
  ~World() = default;

  World(const World&) = default;
  World& operator=(const World&) = default;
  World(World&&) = default;
  World& operator=(World&&) = default;

  [[nodiscard]] ViewportMode viewportMode() const;
  void setViewportMode(ViewportMode mode);

 private:
  ViewportMode viewportMode_;
};

}  // namespace raytracer::scene
