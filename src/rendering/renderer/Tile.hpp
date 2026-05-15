/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Tile
*/

/**
 * @file Tile.hpp
 * @brief Axis-aligned image tile used by the multi-threaded renderer.
 *
 * A Tile describes a rectangular region of the output image in pixel
 * coordinates. The renderer subdivides the framebuffer into independent
 * tiles, then dispatches each tile to a worker thread of the
 * ThreadPool. Two workers writing to disjoint tiles touch disjoint
 * pixels, which is what makes the parallel writes to a shared `Image`
 * safe without locking.
 *
 * The struct is intentionally trivial-copyable: a Tile is just four
 * ints, cheap to move into a lambda capture.
 */

#pragma once

#include <vector>

namespace raytracer::core {

struct Tile {
  int x{0};
  int y{0};
  int width{0};
  int height{0};

  /**
   * @brief Partition an @p imageWidth x @p imageHeight image into tiles
   *        of nominal size @p tileWidth x @p tileHeight.
   *
   * Tiles on the right or bottom border are clipped to fit the image
   * exactly — no padding, no overlap. The returned vector iterates in
   * row-major order (left-to-right, top-to-bottom). This ordering is
   * an implementation detail: callers must not rely on it because the
   * tiles will be reshuffled across worker threads anyway.
   *
   * @pre imageWidth > 0, imageHeight > 0, tileWidth > 0, tileHeight > 0.
   *      Caller should ensure this via RenderSettings::validate().
   */
  static std::vector<Tile> subdivide(int imageWidth, int imageHeight,
                                     int tileWidth, int tileHeight);
};

}  // namespace raytracer::core
