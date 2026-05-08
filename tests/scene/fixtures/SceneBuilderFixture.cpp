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
  objectRegistry.registerType(
      "sphere", [](const libconfig::Setting&) -> std::shared_ptr<IObject> {
        return std::make_shared<DummyObject>();
      });
  lightRegistry.registerType(
      "ambient", [](const libconfig::Setting&) -> std::shared_ptr<ILight> {
        return std::make_shared<NullLightFixture>();
      });
  cameraRegistry.registerType(
      "perspective",
      [](const libconfig::Setting&) -> std::shared_ptr<ICamera> {
        return std::make_shared<OrthoCameraFixture>();
      });

  cfg.getRoot().add("stub", libconfig::Setting::TypeGroup);
  stubSetting = &cfg.getRoot()["stub"];
}
