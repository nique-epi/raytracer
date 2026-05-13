/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** TransformsParser
*/

#include "TransformsParser.hpp"
#include <string>
#include "SceneBuildException.hpp"
#include "components/Primitives/TransformedObject/TransformedObject.hpp"

namespace raytracer::scene {

std::shared_ptr<IObject> wrapWithTransforms(
    const std::shared_ptr<IObject>& primitive,
    const libconfig::Setting& transformsCfg,
    raytracer::core::factory::IComponentFactory& factory) {
  auto wrapper =
      std::make_shared<raytracer::components::primitives::TransformedObject>();
  wrapper->setPrimitive(primitive);
  for (int i = 0; i < transformsCfg.getLength(); ++i) {
    const auto& entry = transformsCfg[i];
    std::string transformType;
    if (!entry.lookupValue("type", transformType)) {
      const std::string detail = entry.exists("type")
                                     ? "non-string 'type' field"
                                     : "missing 'type' field";
      throw SceneBuildException("TransformsParser: transform entry #" +
                                std::to_string(i) + " has " + detail);
    }
    auto transform = factory.createTransformation(transformType, entry);
    if (transform) {
      wrapper->addTransform(transform);
    }
  }
  return wrapper;
}

}  // namespace raytracer::scene
