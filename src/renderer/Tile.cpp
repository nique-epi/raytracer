/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Tile
*/

#include "Tile.hpp"

#include <algorithm>

namespace raytracer::core {

std::vector<Tile> Tile::subdivide(int imageWidth, int imageHeight,
                                  int tileWidth, int tileHeight) {
  std::vector<Tile> tiles;
  if (imageWidth <= 0 || imageHeight <= 0 || tileWidth <= 0 ||
      tileHeight <= 0) {
    return tiles;
  }

  const int columnCount = (imageWidth + tileWidth - 1) / tileWidth;
  const int rowCount = (imageHeight + tileHeight - 1) / tileHeight;
  tiles.reserve(static_cast<std::size_t>(columnCount) *
                static_cast<std::size_t>(rowCount));

  for (int row = 0; row < rowCount; ++row) {
    const int originY = row * tileHeight;
    const int rowHeight = std::min(tileHeight, imageHeight - originY);
    for (int column = 0; column < columnCount; ++column) {
      const int originX = column * tileWidth;
      const int columnWidth = std::min(tileWidth, imageWidth - originX);
      tiles.push_back(Tile{.x = originX,
                           .y = originY,
                           .width = columnWidth,
                           .height = rowHeight});
    }
  }
  return tiles;
}

}  // namespace raytracer::core
