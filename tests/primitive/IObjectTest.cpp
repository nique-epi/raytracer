/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** IObject interface tests
*/

#include <memory>
#include <gtest/gtest.h>
#include "primitive/IObject.hpp"
#include "utils/math/AABB.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

class ITransformation;

class StubObject : public IObject {
 public:
  StubObject() = default;
  bool hits(const Math::Ray&, double, double, Math::HitRecord&) const override {
    return false;
  }

  Math::AABB getBoundingBox() const override {
    return Math::AABB(Math::Vector3D(-1.0, -1.0, -1.0),
                      Math::Vector3D(1.0, 1.0, 1.0));
  }

  void applyTransformation(const ITransformation&) override {}
};

TEST(IObjectTest, HitsReturnsFalseWhenNoIntersection) {
  StubObject obj;
  IObject& ref = obj;
  Math::Ray ray(Math::Vector3D(0.0, 0.0, -5.0), Math::Vector3D(0.0, 0.0, 1.0));
  Math::HitRecord rec;
  EXPECT_FALSE(ref.hits(ray, 0.0, 100.0, rec));
}

TEST(IObjectTest, GetBoundingBoxReturnsCorrectBounds) {
  StubObject obj;
  Math::AABB bbox = obj.getBoundingBox();
  EXPECT_DOUBLE_EQ(bbox.min.x, -1.0);
  EXPECT_DOUBLE_EQ(bbox.min.y, -1.0);
  EXPECT_DOUBLE_EQ(bbox.min.z, -1.0);
  EXPECT_DOUBLE_EQ(bbox.max.x, 1.0);
  EXPECT_DOUBLE_EQ(bbox.max.y, 1.0);
  EXPECT_DOUBLE_EQ(bbox.max.z, 1.0);
}

TEST(IObjectTest, PolymorphicUsageViaBasePointer) {
  std::unique_ptr<IObject> obj = std::make_unique<StubObject>();
  Math::Ray ray(Math::Vector3D(0.0, 0.0, 0.0), Math::Vector3D(1.0, 0.0, 0.0));
  Math::HitRecord rec;
  EXPECT_FALSE(obj->hits(ray, 0.0, 100.0, rec));
}
