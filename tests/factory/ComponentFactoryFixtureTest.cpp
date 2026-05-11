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
    cfg_.getRoot().add("stub", libconfig::Setting::TypeGroup);
    stubSetting_ = &cfg_.getRoot()["stub"];
  }

  ComponentFactoryFixture factory_;
  libconfig::Config cfg_;
  libconfig::Setting* stubSetting_ = nullptr;
};

TEST_F(ComponentFactoryFixtureTest, UnknownPrimitiveThrows) {
  EXPECT_THROW(
      static_cast<void>(factory_.createPrimitive("sphere", *stubSetting_)),
      raytracer::core::RaytracerException);
}

TEST_F(ComponentFactoryFixtureTest, RegisteredPrimitiveCreatorIsInvoked) {
  factory_.registerPrimitive(
      "sphere", [](const libconfig::Setting&) -> std::shared_ptr<IObject> {
        return std::make_shared<StubObject>();
      });
  auto primitive = factory_.createPrimitive("sphere", *stubSetting_);
  EXPECT_NE(primitive, nullptr);
}

TEST_F(ComponentFactoryFixtureTest, PolymorphicDispatchThroughInterface) {
  factory_.registerPrimitive(
      "sphere", [](const libconfig::Setting&) -> std::shared_ptr<IObject> {
        return std::make_shared<StubObject>();
      });
  raytracer::core::factory::IComponentFactory& base = factory_;
  EXPECT_NE(base.createPrimitive("sphere", *stubSetting_), nullptr);
}

TEST_F(ComponentFactoryFixtureTest, RegisterPrimitiveRejectsEmptyCreator) {
  EXPECT_THROW(factory_.registerPrimitive("sphere", {}),
               raytracer::core::RaytracerException);
}

TEST_F(ComponentFactoryFixtureTest, AllInterfacesThrowOnUnknownType) {
  EXPECT_THROW(static_cast<void>(factory_.createLight("ambient", *stubSetting_)),
               raytracer::core::RaytracerException);
  EXPECT_THROW(
      static_cast<void>(factory_.createMaterial("lambertian", *stubSetting_)),
      raytracer::core::RaytracerException);
  EXPECT_THROW(
      static_cast<void>(factory_.createCamera("perspective", *stubSetting_)),
      raytracer::core::RaytracerException);
  EXPECT_THROW(static_cast<void>(
                   factory_.createTransformation("translation", *stubSetting_)),
               raytracer::core::RaytracerException);
}

}  // namespace
