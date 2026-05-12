#include <gtest/gtest.h>
#include <memory>
#include "../../fixtures/SphereFixture.hpp"
#include "components/Primitives/Collection/Collection.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

using namespace raytracer::math;
using namespace raytracer::components;

TEST(CollectionTest, PicksNearestOfThreeSpheres) {
  Collection col;

  // place spheres along -Z axis at z = -1, -2, -3
  col.add(std::make_shared<SphereFixture>(Vector3D(0, 0, -3), 0.5));
  col.add(std::make_shared<SphereFixture>(Vector3D(0, 0, -2), 0.5));
  col.add(std::make_shared<SphereFixture>(Vector3D(0, 0, -1), 0.5));

  Ray r(Vector3D(0, 0, 0), Vector3D(0, 0, -1));
  HitRecord rec;
  const bool hit = col.hits(r, 0.001, 1000.0, rec);

  EXPECT_TRUE(hit);
  // nearest sphere is at z = -1 -> t should be close to 0.5 (origin to surface)
  EXPECT_NEAR(rec.t, 0.5, 1e-6);
}
