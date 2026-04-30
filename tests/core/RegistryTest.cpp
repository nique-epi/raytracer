/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Registry unit tests
*/

#include <gtest/gtest.h>
#include <libconfig.h++>
#include <memory>
#include <string>
#include "core/registry/registry.hpp"

using namespace raytracer::core::registry;

namespace {

class DummyObject final : public IObject {
 public:
  explicit DummyObject(int v) : value(v) {}

  bool hits(const raytracer::math::Ray& /*ray*/, double /*tMin*/,
            double /*tMax*/,
            raytracer::math::HitRecord& /*rec*/) const override {
    return false;
  }

  [[nodiscard]] raytracer::math::AABB getBoundingBox() const override {
    return {};
  }

  void applyTransformation(const ITransformation& /*transform*/) override {}

  int value;
};

}  // namespace

class RegistryTest : public ::testing::Test {
 protected:
  void SetUp() override {
    cfg.getRoot().add("stub", libconfig::Setting::TypeGroup);
    stubSetting = &cfg.getRoot()["stub"];
  }

  libconfig::Config cfg;
  libconfig::Setting* stubSetting = nullptr;
};

TEST_F(RegistryTest, RegisterAndCreateReturnsCorrectObject) {
  ObjectRegistry registry;
  registry.registerType(
      "dummy", [](const libconfig::Setting& /*s*/) -> std::shared_ptr<IObject> {
        return std::make_shared<DummyObject>(42);
      });

  auto obj = registry.create("dummy", *stubSetting);
  ASSERT_NE(obj, nullptr);

  auto* d = dynamic_cast<DummyObject*>(obj.get());
  ASSERT_NE(d, nullptr);
  EXPECT_EQ(d->value, 42);
}

TEST_F(RegistryTest, RegisterMultipleTypesCreatesCorrectOnes) {
  ObjectRegistry registry;
  registry.registerType(
      "alpha", [](const libconfig::Setting& /*s*/) -> std::shared_ptr<IObject> {
        return std::make_shared<DummyObject>(1);
      });
  registry.registerType(
      "beta", [](const libconfig::Setting& /*s*/) -> std::shared_ptr<IObject> {
        return std::make_shared<DummyObject>(2);
      });

  auto a = registry.create("alpha", *stubSetting);
  auto b = registry.create("beta", *stubSetting);

  ASSERT_NE(a, nullptr);
  ASSERT_NE(b, nullptr);
  EXPECT_EQ(dynamic_cast<DummyObject*>(a.get())->value, 1);
  EXPECT_EQ(dynamic_cast<DummyObject*>(b.get())->value, 2);
}

TEST_F(RegistryTest, OverwriteTypeReplacesCreator) {
  ObjectRegistry registry;
  registry.registerType(
      "item", [](const libconfig::Setting& /*s*/) -> std::shared_ptr<IObject> {
        return std::make_shared<DummyObject>(10);
      });
  registry.registerType(
      "item", [](const libconfig::Setting& /*s*/) -> std::shared_ptr<IObject> {
        return std::make_shared<DummyObject>(20);
      });

  auto obj = registry.create("item", *stubSetting);
  ASSERT_NE(obj, nullptr);
  EXPECT_EQ(dynamic_cast<DummyObject*>(obj.get())->value, 20);
}

TEST_F(RegistryTest, CreatorReceivesSettingReference) {
  ObjectRegistry registry;

  // Add a real integer value to the stub group so the creator can read it.
  stubSetting->add("val", libconfig::Setting::TypeInt) = 99;

  registry.registerType(
      "reader", [](const libconfig::Setting& s) -> std::shared_ptr<IObject> {
        int v = s.lookup("val");
        return std::make_shared<DummyObject>(v);
      });

  auto obj = registry.create("reader", *stubSetting);
  ASSERT_NE(obj, nullptr);
  EXPECT_EQ(dynamic_cast<DummyObject*>(obj.get())->value, 99);
}

TEST_F(RegistryTest, CreateReturnsNullptrWhenCreatorReturnsNullptr) {
  ObjectRegistry registry;
  registry.registerType(
      "null_creator",
      [](const libconfig::Setting& /*s*/) -> std::shared_ptr<IObject> {
        return nullptr;
      });

  auto obj = registry.create("null_creator", *stubSetting);
  EXPECT_EQ(obj, nullptr);
}

TEST_F(RegistryTest, LoadPluginWithInvalidPathDoesNotCrash) {
  ObjectRegistry registry;
  EXPECT_NO_THROW(registry.loadPlugin("/nonexistent/path/plugin.so"));
}
