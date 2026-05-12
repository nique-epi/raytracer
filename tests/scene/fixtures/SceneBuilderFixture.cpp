/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SceneBuilder test fixture
*/

#include "SceneBuilderFixture.hpp"
#include <memory>
#include "../../fixtures/OrthoCameraFixture.hpp"
#include "NullLightFixture.hpp"

bool DummyObject::hits(const raytracer::math::Ray&, double, double,
                        raytracer::math::HitRecord&) const {
  return false;
}

raytracer::math::AABB DummyObject::getBoundingBox() const {
  return {};
}

void DummyObject::applyTransformation(const ITransformation&) {}

void SceneBuilderFixture::SetUp() {
  factory_.registerPrimitive(
      "sphere", [](const libconfig::Setting&) -> std::shared_ptr<IObject> {
        return std::make_shared<DummyObject>();
      });
  factory_.registerPrimitive(
      "plane", [](const libconfig::Setting&) -> std::shared_ptr<IObject> {
        return std::make_shared<DummyObject>();
      });
  factory_.registerLight(
      "ambient", [](const libconfig::Setting&) -> std::shared_ptr<ILight> {
        return std::make_shared<NullLightFixture>();
      });
  factory_.registerLight(
      "directional", [](const libconfig::Setting&) -> std::shared_ptr<ILight> {
        return std::make_shared<NullLightFixture>();
      });
  factory_.registerCamera(
      "perspective",
      [](const libconfig::Setting&) -> std::shared_ptr<ICamera> {
        return std::make_shared<OrthoCameraFixture>();
      });

  cfg_.getRoot().add("stub", libconfig::Setting::TypeGroup);
  stubSetting_ = &cfg_.getRoot()["stub"];
}
