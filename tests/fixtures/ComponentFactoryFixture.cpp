/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ComponentFactoryFixture fixture for unit tests
*/

#include "ComponentFactoryFixture.hpp"
#include <utility>
#include "exceptions/Exceptions.hpp"

namespace {

template <typename Map>
auto lookupOrThrow(const Map& map, const std::string& type,
                   const char* category) -> const typename Map::mapped_type& {
  const auto it = map.find(type);
  if (it == map.end()) {
    throw raytracer::core::RaytracerException(
        std::string("ComponentFactoryFixture: unknown ") + category +
        " type '" + type + "'");
  }
  return it->second;
}

template <typename Creator>
void rejectEmptyCreator(const std::string& type, const Creator& creator,
                        const char* category) {
  if (!creator) {
    throw raytracer::core::RaytracerException(
        std::string("ComponentFactoryFixture: empty ") + category +
        " creator for type '" + type + "'");
  }
}

}  // namespace

void ComponentFactoryFixture::registerPrimitive(const std::string& type,
                                                PrimitiveCreator creator) {
  rejectEmptyCreator(type, creator, "primitive");
  primitiveCreators_[type] = std::move(creator);
}

void ComponentFactoryFixture::registerLight(const std::string& type,
                                            LightCreator creator) {
  rejectEmptyCreator(type, creator, "light");
  lightCreators_[type] = std::move(creator);
}

void ComponentFactoryFixture::registerMaterial(const std::string& type,
                                               MaterialCreator creator) {
  rejectEmptyCreator(type, creator, "material");
  materialCreators_[type] = std::move(creator);
}

void ComponentFactoryFixture::registerCamera(const std::string& type,
                                             CameraCreator creator) {
  rejectEmptyCreator(type, creator, "camera");
  cameraCreators_[type] = std::move(creator);
}

void ComponentFactoryFixture::registerTransformation(
    const std::string& type, TransformationCreator creator) {
  rejectEmptyCreator(type, creator, "transformation");
  transformationCreators_[type] = std::move(creator);
}

std::shared_ptr<IObject> ComponentFactoryFixture::createPrimitive(
    const std::string& type, const libconfig::Setting& cfg) {
  return lookupOrThrow(primitiveCreators_, type, "primitive")(cfg);
}

std::shared_ptr<ILight> ComponentFactoryFixture::createLight(
    const std::string& type, const libconfig::Setting& cfg) {
  return lookupOrThrow(lightCreators_, type, "light")(cfg);
}

std::shared_ptr<IMaterial> ComponentFactoryFixture::createMaterial(
    const std::string& type, const libconfig::Setting& cfg) {
  return lookupOrThrow(materialCreators_, type, "material")(cfg);
}

std::shared_ptr<ICamera> ComponentFactoryFixture::createCamera(
    const std::string& type, const libconfig::Setting& cfg) {
  return lookupOrThrow(cameraCreators_, type, "camera")(cfg);
}

std::shared_ptr<ITransformation> ComponentFactoryFixture::createTransformation(
    const std::string& type, const libconfig::Setting& cfg) {
  return lookupOrThrow(transformationCreators_, type, "transformation")(cfg);
}
