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

TEST_F(ComponentFactoryFixtureTest, UnknownPrimitiveThrows) {
  EXPECT_THROW(
      static_cast<void>(factory.createPrimitive("sphere", *stubSetting)),
      raytracer::core::RaytracerException);
}

TEST_F(ComponentFactoryFixtureTest, RegisteredPrimitiveCreatorIsInvoked) {
  factory.registerPrimitive(
      "sphere", [](const libconfig::Setting&) -> std::shared_ptr<IObject> {
        return std::make_shared<StubObject>();
      });
  auto primitive = factory.createPrimitive("sphere", *stubSetting);
  EXPECT_NE(primitive, nullptr);
}

TEST_F(ComponentFactoryFixtureTest, PolymorphicDispatchThroughInterface) {
  factory.registerPrimitive(
      "sphere", [](const libconfig::Setting&) -> std::shared_ptr<IObject> {
        return std::make_shared<StubObject>();
      });
  raytracer::core::factory::IComponentFactory& base = factory;
  EXPECT_NE(base.createPrimitive("sphere", *stubSetting), nullptr);
}

TEST_F(ComponentFactoryFixtureTest, RegisterPrimitiveRejectsEmptyCreator) {
  EXPECT_THROW(factory.registerPrimitive("sphere", {}),
               raytracer::core::RaytracerException);
}

TEST_F(ComponentFactoryFixtureTest, AllInterfacesThrowOnUnknownType) {
  EXPECT_THROW(
      static_cast<void>(factory.createLight("ambient", *stubSetting)),
      raytracer::core::RaytracerException);
  EXPECT_THROW(
      static_cast<void>(factory.createMaterial("lambertian", *stubSetting)),
      raytracer::core::RaytracerException);
  EXPECT_THROW(
      static_cast<void>(factory.createCamera("perspective", *stubSetting)),
      raytracer::core::RaytracerException);
  EXPECT_THROW(
      static_cast<void>(factory.createTransformation("translation", *stubSetting)),
      raytracer::core::RaytracerException);
}

}  // namespace
