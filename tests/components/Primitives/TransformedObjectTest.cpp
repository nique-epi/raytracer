/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** TransformedObjectTest
*/

#include <gtest/gtest.h>
#include <memory>
#include "../../fixtures/SphereFixture.hpp"
#include "components/Primitives/TransformedObject/TransformedObject.hpp"
#include "components/Transformations/Translation/Translation.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

using namespace raytracer::components::primitives;
using namespace raytracer::components::transformation;
using namespace raytracer::math;

TEST(TransformedObjectTest, TranslationMatchesDirectSphere) {
  auto primitive =
      std::make_shared<SphereFixture>(Vector3D(0.0, 0.0, 0.0), 0.5);
  TransformedObject transformed;
  transformed.setPrimitive(primitive);
  transformed.addTransform(
      std::make_shared<Translation>(Vector3D(2.0, 0.0, 0.0)));

  SphereFixture directSphere(Vector3D(2.0, 0.0, 0.0), 0.5);
  Ray ray(Vector3D(0.0, 0.0, 0.0), Vector3D(1.0, 0.0, 0.0));

  HitRecord transformedRec;
  HitRecord directRec;

  ASSERT_TRUE(transformed.hits(ray, 0.001, 1000.0, transformedRec));
  ASSERT_TRUE(directSphere.hits(ray, 0.001, 1000.0, directRec));

  EXPECT_NEAR(transformedRec.t, directRec.t, 1e-9);
  EXPECT_NEAR(transformedRec.point.x, directRec.point.x, 1e-9);
  EXPECT_NEAR(transformedRec.point.y, directRec.point.y, 1e-9);
  EXPECT_NEAR(transformedRec.point.z, directRec.point.z, 1e-9);
  EXPECT_NEAR(transformedRec.normal.x, directRec.normal.x, 1e-9);
  EXPECT_NEAR(transformedRec.normal.y, directRec.normal.y, 1e-9);
  EXPECT_NEAR(transformedRec.normal.z, directRec.normal.z, 1e-9);

  const AABB transformedBox = transformed.getBoundingBox();
  const AABB directBox = directSphere.getBoundingBox();

  EXPECT_NEAR(transformedBox.min.x, directBox.min.x, 1e-9);
  EXPECT_NEAR(transformedBox.min.y, directBox.min.y, 1e-9);
  EXPECT_NEAR(transformedBox.min.z, directBox.min.z, 1e-9);
  EXPECT_NEAR(transformedBox.max.x, directBox.max.x, 1e-9);
  EXPECT_NEAR(transformedBox.max.y, directBox.max.y, 1e-9);
  EXPECT_NEAR(transformedBox.max.z, directBox.max.z, 1e-9);
}
