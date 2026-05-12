/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** TransformsBuilderFixture
*/

#include "TransformsBuilderFixture.hpp"
#include <memory>
#include "../../fixtures/OrthoCameraFixture.hpp"
#include "../../fixtures/SphereFixture.hpp"
#include "NullLightFixture.hpp"
#include "components/Primitives/IObject.hpp"
#include "components/Transformations/ITransformation.hpp"
#include "components/camera/ICamera.hpp"
#include "components/light/ILight.hpp"
#include "factory/transformation/TransformationFactory.hpp"
#include "utils/math/Vector3D.hpp"

void TransformsBuilderFixture::SetUp() {
  factory_.registerPrimitive(
      "sphere", [](const libconfig::Setting&) -> std::shared_ptr<IObject> {
        return std::make_shared<SphereFixture>(
            raytracer::math::Vector3D{0.0, 0.0, 0.0}, 0.5);
      });
  factory_.registerTransformation(
      "translation",
      [](const libconfig::Setting& cfg) -> std::shared_ptr<ITransformation> {
        return raytracer::core::factory::TransformationFactory::create(
            "translation", cfg);
      });
  factory_.registerLight(
      "ambient", [](const libconfig::Setting&) -> std::shared_ptr<ILight> {
        return std::make_shared<NullLightFixture>();
      });
  factory_.registerCamera(
      "perspective",
      [](const libconfig::Setting&) -> std::shared_ptr<ICamera> {
        return std::make_shared<OrthoCameraFixture>();
      });
}
