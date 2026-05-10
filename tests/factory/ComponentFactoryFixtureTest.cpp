/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ComponentFactoryFixture unit tests
*/

#include <gtest/gtest.h>
#include <libconfig.h++>
#include <memory>
#include "components/Primitives/IObject.hpp"
#include "exceptions/Exceptions.hpp"
#include "factory/IComponentFactory.hpp"
#include "fixtures/ComponentFactoryFixture.hpp"

namespace {

class StubObject final : public IObject {
 public:
  bool hits(const raytracer::math::Ray&, double, double,
            raytracer::math::HitRecord&) const override {
    return false;
  }
  [[nodiscard]] raytracer::math::AABB getBoundingBox() const override {
    return {};
  }
  void applyTransformation(const ITransformation&) override {}
};

class ComponentFactoryFixtureTest : public ::testing::Test {
 protected:
  void SetUp() override {
    cfg.getRoot().add("stub", libconfig::Setting::TypeGroup);
    stubSetting = &cfg.getRoot()["stub"];
  }

  ComponentFactoryFixture factory;
  libconfig::Config cfg;
  libconfig::Setting* stubSetting = nullptr;
};

// Given: a fixture with no registrations.
// When:  createPrimitive is called for an unknown type.
// Then:  it throws RaytracerException.
TEST_F(ComponentFactoryFixtureTest, UnknownPrimitiveThrows) {
  EXPECT_THROW(factory.createPrimitive("sphere", *stubSetting),
               raytracer::core::RaytracerException);
}

// Given: a fixture with a registered primitive creator.
// When:  createPrimitive is called for that type.
// Then:  the registered creator is invoked and its result returned.
TEST_F(ComponentFactoryFixtureTest, RegisteredPrimitiveCreatorIsInvoked) {
  factory.registerPrimitive(
      "sphere", [](const libconfig::Setting&) -> std::shared_ptr<IObject> {
        return std::make_shared<StubObject>();
      });
  auto primitive = factory.createPrimitive("sphere", *stubSetting);
  EXPECT_NE(primitive, nullptr);
}

// Given: a fixture used through the IComponentFactory interface.
// When:  createPrimitive is called via the base reference.
// Then:  polymorphic dispatch reaches the fixture override.
TEST_F(ComponentFactoryFixtureTest, PolymorphicDispatchThroughInterface) {
  factory.registerPrimitive(
      "sphere", [](const libconfig::Setting&) -> std::shared_ptr<IObject> {
        return std::make_shared<StubObject>();
      });
  raytracer::core::factory::IComponentFactory& base = factory;
  EXPECT_NE(base.createPrimitive("sphere", *stubSetting), nullptr);
}

// Given: a fixture with no registrations.
// When:  createLight, createMaterial, createCamera, createTransformation
//        are called for unknown types.
// Then:  each one throws RaytracerException with a category-specific message.
TEST_F(ComponentFactoryFixtureTest, AllInterfacesThrowOnUnknownType) {
  EXPECT_THROW(factory.createLight("ambient", *stubSetting),
               raytracer::core::RaytracerException);
  EXPECT_THROW(factory.createMaterial("lambertian", *stubSetting),
               raytracer::core::RaytracerException);
  EXPECT_THROW(factory.createCamera("perspective", *stubSetting),
               raytracer::core::RaytracerException);
  EXPECT_THROW(factory.createTransformation("translation", *stubSetting),
               raytracer::core::RaytracerException);
}

}  // namespace
