/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Cylinder unit tests (orphan — no CMake target, see project_test_system.md)
*/

#include <gtest/gtest.h>
#include <memory>
#include "components/Primitives/cylinder/Cylinder.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::components::primitives::Cylinder;
using raytracer::math::HitRecord;
using raytracer::math::Ray;
using raytracer::math::Vector3D;

namespace {
constexpr double defaultRadius = 1.0;
constexpr double defaultHeight = 1.0;
constexpr double infiniteHeight = -1.0;
}  // namespace

TEST(CylinderTest, InfiniteCylinderHit) {
  Cylinder cyl(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 1.0, 0.0), defaultRadius,
               infiniteHeight, nullptr);
  Ray ray(Vector3D(2.0, 100.0, 0.0), Vector3D(-1.0, 0.0, 0.0));
  HitRecord rec;
  EXPECT_TRUE(cyl.hits(ray, 0.001, 1000.0, rec));
}

TEST(CylinderTest, FiniteCylinderRejectsOutOfHeight) {
  Cylinder cyl(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 1.0, 0.0), defaultRadius,
               defaultHeight, nullptr);
  Ray ray(Vector3D(2.0, 100.0, 0.0), Vector3D(-1.0, 0.0, 0.0));
  HitRecord rec;
  EXPECT_FALSE(cyl.hits(ray, 0.001, 1000.0, rec));
}
