/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** HitRecord unit tests
*/

#include <gtest/gtest.h>
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

using Math::HitRecord;
using Math::Ray;
using Math::Vector3D;

TEST(HitRecordTest, DefaultValues) {
  HitRecord rec;
  EXPECT_DOUBLE_EQ(rec.t, 0.0);
  EXPECT_FALSE(rec.frontFace);
  EXPECT_DOUBLE_EQ(rec.u, 0.0);
  EXPECT_DOUBLE_EQ(rec.v, 0.0);
  EXPECT_EQ(rec.material, nullptr);
}

TEST(HitRecordTest, SetFaceNormalFrontFace) {
  Ray ray(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 0.0, 1.0));
  Vector3D outNormal(0.0, 0.0, -1.0);
  HitRecord rec;
  rec.setFaceNormal(ray, outNormal);
  EXPECT_TRUE(rec.frontFace);
  EXPECT_DOUBLE_EQ(rec.normal.z, -1.0);
}

TEST(HitRecordTest, SetFaceNormalBackFace) {
  Ray ray(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 0.0, 1.0));
  Vector3D outNormal(0.0, 0.0, 1.0);
  HitRecord rec;
  rec.setFaceNormal(ray, outNormal);
  EXPECT_FALSE(rec.frontFace);
  EXPECT_DOUBLE_EQ(rec.normal.z, -1.0);
}

TEST(HitRecordTest, SetFaceNormalFrontFaceFlippedNormal) {
  Ray ray(Vector3D(0.0, 0.0, 0.0), Vector3D(1.0, 0.0, 0.0));
  Vector3D outNormal(-1.0, 0.0, 0.0);
  HitRecord rec;
  rec.setFaceNormal(ray, outNormal);
  EXPECT_TRUE(rec.frontFace);
  EXPECT_DOUBLE_EQ(rec.normal.x, -1.0);
}
