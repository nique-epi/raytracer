/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** World
*/

#pragma once

#include <cstdint>

namespace raytracer::scene {

inline constexpr int defaultAmbientOcclusionSamples = 16;
inline constexpr double defaultAmbientOcclusionRadius = 1.0;
inline constexpr double defaultAmbientOcclusionIntensity = 1.0;

/**
 * @brief Ambient-occlusion configuration applied as a post-shading
 *        multiplicative factor on primary rays.
 *
 * Ambient occlusion is a world-space approximation of how much of the
 * upper hemisphere above a shaded point is occluded by nearby geometry.
 * It is multiplied onto the primary-ray radiance so concave regions
 * darken naturally without paying for true global illumination.
 *
 * AO is intentionally limited to primary rays: applying it on every
 * recursive bounce would multiply the cost exponentially without an
 * equivalent visual gain.
 */
struct AmbientOcclusionSettings {
  /// Whether the AO pass runs. Defaults to false to preserve baseline.
  bool enabled{false};
  /// Number of hemisphere samples per shaded point (higher = less noise).
  int samples{defaultAmbientOcclusionSamples};
  /// Maximum distance an occlusion sample considers as "blocking".
  double radius{defaultAmbientOcclusionRadius};
  /// AO strength in [0, 1]: 0 disables darkening, 1 applies full AO.
  double intensity{defaultAmbientOcclusionIntensity};
};

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

  /// @returns The active ambient-occlusion settings (disabled by default).
  [[nodiscard]] const AmbientOcclusionSettings& ambientOcclusion() const;

  /// @brief Replace the ambient-occlusion settings wholesale.
  void setAmbientOcclusion(const AmbientOcclusionSettings& settings);

 private:
  ViewportMode viewportMode_;
  AmbientOcclusionSettings ambientOcclusion_;
};

}  // namespace raytracer::scene
