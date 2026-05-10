/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ComponentFactory
*/

#include "ComponentFactory.hpp"
#include "CameraFactory.hpp"
#include "LightFactory.hpp"
#include "MaterialFactory.hpp"
#include "PrimitiveFactory.hpp"
#include "TransformationFactory.hpp"

namespace raytracer::core::factory {

std::shared_ptr<IObject> ComponentFactory::createPrimitive(
    const std::string& type, const libconfig::Setting& cfg) {
  return PrimitiveFactory::create(type, cfg);
}

std::shared_ptr<ILight> ComponentFactory::createLight(
    const std::string& type, const libconfig::Setting& cfg) {
  return LightFactory::create(type, cfg);
}

std::shared_ptr<IMaterial> ComponentFactory::createMaterial(
    const std::string& type, const libconfig::Setting& cfg) {
  return MaterialFactory::create(type, cfg);
}

std::shared_ptr<ICamera> ComponentFactory::createCamera(
    const std::string& type, const libconfig::Setting& cfg) {
  return CameraFactory::create(type, cfg);
}

std::shared_ptr<ITransformation> ComponentFactory::createTransformation(
    const std::string& type, const libconfig::Setting& cfg) {
  return TransformationFactory::create(type, cfg);
}

}  // namespace raytracer::core::factory
