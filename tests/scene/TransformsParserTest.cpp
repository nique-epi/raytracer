/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** TransformsParser unit tests
*/

#include <gtest/gtest.h>
#include <libconfig.h++>
#include <memory>
#include "fixtures/ComponentFactoryFixture.hpp"
#include "fixtures/SphereFixture.hpp"
#include "components/Transformations/ITransformation.hpp"
#include "exceptions/Exceptions.hpp"
#include "factory/transformation/TransformationFactory.hpp"
#include "scene/SceneBuildException.hpp"
#include "scene/TransformsParser.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::scene::wrapWithTransforms;

namespace {

class TransformsParserFixture : public ::testing::Test {
 protected:
  void SetUp() override {
    factory_.registerTransformation(
        "translation",
        [](const libconfig::Setting& cfg) -> std::shared_ptr<ITransformation> {
          return raytracer::core::factory::TransformationFactory::create(
              "translation", cfg);
        });
  }

  ComponentFactoryFixture factory_;
};

libconfig::Setting& addVec3Group(libconfig::Setting& parent, const char* name,
                                  double x, double y, double z) {
  auto& group = parent.add(name, libconfig::Setting::TypeGroup);
  group.add("x", libconfig::Setting::TypeFloat) = x;
  group.add("y", libconfig::Setting::TypeFloat) = y;
  group.add("z", libconfig::Setting::TypeFloat) = z;
  return group;
}

libconfig::Setting& addTranslationEntry(libconfig::Setting& transformsList,
                                        double x, double y, double z) {
  auto& entry = transformsList.add(libconfig::Setting::TypeGroup);
  entry.add("type", libconfig::Setting::TypeString) = "translation";
  addVec3Group(entry, "offset", x, y, z);
  return entry;
}

std::shared_ptr<SphereFixture> makeUnitSphereAtOrigin() {
  return std::make_shared<SphereFixture>(
      raytracer::math::Vector3D{0.0, 0.0, 0.0}, 0.5);
}

}  // namespace

// Given: a single translation(10,0,0) on a sphere(center=0, r=0.5).
// When:  wrapWithTransforms builds the decorator.
// Then:  the decorator hits a forward ray identically to a sphere(10,0,0).
TEST_F(TransformsParserFixture, SingleTranslationMatchesOffsetSphere) {
  libconfig::Config config;
  auto& transforms =
      config.getRoot().add("transforms", libconfig::Setting::TypeList);
  addTranslationEntry(transforms, 10.0, 0.0, 0.0);

  auto wrapped = wrapWithTransforms(makeUnitSphereAtOrigin(), transforms,
                                    factory_);

  const raytracer::math::Ray ray(raytracer::math::Vector3D(0.0, 0.0, 0.0),
                                 raytracer::math::Vector3D(1.0, 0.0, 0.0));
  raytracer::math::HitRecord wrappedRec;
  ASSERT_TRUE(wrapped->hits(ray, 0.001, 1000.0, wrappedRec));

  SphereFixture direct(raytracer::math::Vector3D(10.0, 0.0, 0.0), 0.5);
  raytracer::math::HitRecord directRec;
  ASSERT_TRUE(direct.hits(ray, 0.001, 1000.0, directRec));

  EXPECT_NEAR(wrappedRec.t, directRec.t, 1e-9);
  EXPECT_NEAR(wrappedRec.point.x, directRec.point.x, 1e-9);
  EXPECT_NEAR(wrappedRec.point.y, directRec.point.y, 1e-9);
  EXPECT_NEAR(wrappedRec.point.z, directRec.point.z, 1e-9);
}

// Given: two translations (1,0,0) then (10,0,0) stacked in file order.
// When:  wrapWithTransforms builds the decorator.
// Then:  the composed offset is (11,0,0) along +X.
TEST_F(TransformsParserFixture, TransformsAreStackedInFileOrder) {
  libconfig::Config config;
  auto& transforms =
      config.getRoot().add("transforms", libconfig::Setting::TypeList);
  addTranslationEntry(transforms, 1.0, 0.0, 0.0);
  addTranslationEntry(transforms, 10.0, 0.0, 0.0);

  auto wrapped = wrapWithTransforms(makeUnitSphereAtOrigin(), transforms,
                                    factory_);

  const raytracer::math::Ray ray(raytracer::math::Vector3D(0.0, 0.0, 0.0),
                                 raytracer::math::Vector3D(1.0, 0.0, 0.0));
  raytracer::math::HitRecord rec;
  ASSERT_TRUE(wrapped->hits(ray, 0.001, 1000.0, rec));
  EXPECT_NEAR(rec.point.x, 10.5, 1e-9);
}

// Given: an empty transforms list.
// When:  wrapWithTransforms is called.
// Then:  the returned wrapper hits identically to the bare primitive.
TEST_F(TransformsParserFixture, EmptyListReturnsWrapperWithoutTransforms) {
  libconfig::Config config;
  auto& transforms =
      config.getRoot().add("transforms", libconfig::Setting::TypeList);

  auto primitive = makeUnitSphereAtOrigin();
  auto wrapped = wrapWithTransforms(primitive, transforms, factory_);

  const raytracer::math::Ray ray(raytracer::math::Vector3D(-2.0, 0.0, 0.0),
                                 raytracer::math::Vector3D(1.0, 0.0, 0.0));
  raytracer::math::HitRecord wrappedRec;
  raytracer::math::HitRecord directRec;
  ASSERT_TRUE(wrapped->hits(ray, 0.001, 1000.0, wrappedRec));
  ASSERT_TRUE(primitive->hits(ray, 0.001, 1000.0, directRec));
  EXPECT_NEAR(wrappedRec.t, directRec.t, 1e-9);
}

// Given: a transform entry without a 'type' field.
// When:  wrapWithTransforms is called.
// Then:  SceneBuildException is thrown and the message points at the missing
//        field on the failing entry's index.
TEST_F(TransformsParserFixture, EntryWithoutTypeThrowsSceneBuildException) {
  libconfig::Config config;
  auto& transforms =
      config.getRoot().add("transforms", libconfig::Setting::TypeList);
  auto& entry = transforms.add(libconfig::Setting::TypeGroup);
  addVec3Group(entry, "offset", 1.0, 0.0, 0.0);

  try {
    static_cast<void>(
        wrapWithTransforms(makeUnitSphereAtOrigin(), transforms, factory_));
    FAIL() << "expected SceneBuildException";
  } catch (const raytracer::scene::SceneBuildException& e) {
    const std::string message = e.what();
    EXPECT_NE(message.find("missing 'type' field"), std::string::npos);
    EXPECT_NE(message.find("#0"), std::string::npos);
  }
}

// Given: a transform entry whose 'type' field exists but is not a string.
// When:  wrapWithTransforms is called.
// Then:  SceneBuildException is thrown and the message flags a non-string
//        'type' field rather than a missing one.
TEST_F(TransformsParserFixture, EntryWithNonStringTypeThrowsSceneBuildException) {
  libconfig::Config config;
  auto& transforms =
      config.getRoot().add("transforms", libconfig::Setting::TypeList);
  auto& entry = transforms.add(libconfig::Setting::TypeGroup);
  entry.add("type", libconfig::Setting::TypeInt) = 42;

  try {
    static_cast<void>(
        wrapWithTransforms(makeUnitSphereAtOrigin(), transforms, factory_));
    FAIL() << "expected SceneBuildException";
  } catch (const raytracer::scene::SceneBuildException& e) {
    const std::string message = e.what();
    EXPECT_NE(message.find("non-string 'type' field"), std::string::npos);
    EXPECT_NE(message.find("#0"), std::string::npos);
  }
}

// Given: a transform entry referencing an unregistered type.
// When:  wrapWithTransforms is called.
// Then:  the underlying factory throws a RaytracerException, propagated as-is.
TEST_F(TransformsParserFixture, UnknownTypePropagatesFactoryException) {
  libconfig::Config config;
  auto& transforms =
      config.getRoot().add("transforms", libconfig::Setting::TypeList);
  auto& entry = transforms.add(libconfig::Setting::TypeGroup);
  entry.add("type", libconfig::Setting::TypeString) = "warp";

  EXPECT_THROW(
      static_cast<void>(
          wrapWithTransforms(makeUnitSphereAtOrigin(), transforms, factory_)),
      raytracer::core::RaytracerException);
}
