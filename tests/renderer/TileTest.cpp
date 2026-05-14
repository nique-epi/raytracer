/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Tile unit tests
*/

#include <gtest/gtest.h>
#include <set>
#include <utility>

#include "renderer/Tile.hpp"

using raytracer::core::Tile;

TEST(TileTest, ExactDivisionProducesUniformTiles) {
  const auto tiles = Tile::subdivide(128, 64, 32, 32);
  ASSERT_EQ(tiles.size(), 8u);  // 4 columns x 2 rows
  for (const auto& tile : tiles) {
    EXPECT_EQ(tile.width, 32);
    EXPECT_EQ(tile.height, 32);
  }
}

TEST(TileTest, RightAndBottomBordersAreClipped) {
  const auto tiles = Tile::subdivide(70, 50, 32, 32);
  ASSERT_EQ(tiles.size(), 6u);  // 3 columns x 2 rows

  int totalArea = 0;
  for (const auto& tile : tiles) {
    EXPECT_GT(tile.width, 0);
    EXPECT_GT(tile.height, 0);
    EXPECT_LE(tile.x + tile.width, 70);
    EXPECT_LE(tile.y + tile.height, 50);
    totalArea += tile.width * tile.height;
  }
  EXPECT_EQ(totalArea, 70 * 50);
}

TEST(TileTest, TilesCoverEveryPixelExactlyOnce) {
  constexpr int width = 97;
  constexpr int height = 53;
  const auto tiles = Tile::subdivide(width, height, 16, 16);

  std::set<std::pair<int, int>> seen;
  for (const auto& tile : tiles) {
    for (int y = tile.y; y < tile.y + tile.height; ++y) {
      for (int x = tile.x; x < tile.x + tile.width; ++x) {
        const auto inserted = seen.emplace(x, y);
        ASSERT_TRUE(inserted.second)
            << "pixel (" << x << "," << y << ") covered twice";
      }
    }
  }
  EXPECT_EQ(static_cast<int>(seen.size()), width * height);
}

TEST(TileTest, SingleTileWhenImageSmallerThanTile) {
  const auto tiles = Tile::subdivide(8, 4, 32, 32);
  ASSERT_EQ(tiles.size(), 1u);
  EXPECT_EQ(tiles[0].x, 0);
  EXPECT_EQ(tiles[0].y, 0);
  EXPECT_EQ(tiles[0].width, 8);
  EXPECT_EQ(tiles[0].height, 4);
}

TEST(TileTest, InvalidArgumentsReturnEmpty) {
  EXPECT_TRUE(Tile::subdivide(0, 10, 4, 4).empty());
  EXPECT_TRUE(Tile::subdivide(10, 0, 4, 4).empty());
  EXPECT_TRUE(Tile::subdivide(10, 10, 0, 4).empty());
  EXPECT_TRUE(Tile::subdivide(10, 10, 4, -1).empty());
}
