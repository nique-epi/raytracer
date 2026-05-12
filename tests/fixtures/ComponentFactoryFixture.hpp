/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ComponentFactoryFixture fixture for unit tests
*/

#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include "factory/IComponentFactory.hpp"

namespace libconfig {
class Setting;
}  // namespace libconfig

class IObject;
class ILight;
class IMaterial;
class ICamera;
class ITransformation;

/**
 * @brief Configurable in-memory `IComponentFactory` for unit tests.
 *
 * Each `createX(type, cfg)` call looks up @p type in the matching internal
 * map and invokes the registered creator with @p cfg. Tests register stub
 * creators via `registerX(type, creator)` before exercising the unit under
 * test.
 *
 * Calling `createX` with an unregistered type throws
 * `raytracer::core::RaytracerException`, mirroring the real factory's
 * unknown-type behaviour.
 */
class ComponentFactoryFixture
    : public raytracer::core::factory::IComponentFactory {
 public:
  using PrimitiveCreator =
      std::function<std::shared_ptr<IObject>(const libconfig::Setting&)>;
  using LightCreator =
      std::function<std::shared_ptr<ILight>(const libconfig::Setting&)>;
  using MaterialCreator =
      std::function<std::shared_ptr<IMaterial>(const libconfig::Setting&)>;
  using CameraCreator =
      std::function<std::shared_ptr<ICamera>(const libconfig::Setting&)>;
  using TransformationCreator =
      std::function<std::shared_ptr<ITransformation>(const libconfig::Setting&)>;

  ComponentFactoryFixture() = default;
  ~ComponentFactoryFixture() override = default;

  ComponentFactoryFixture(const ComponentFactoryFixture&) = delete;
  ComponentFactoryFixture& operator=(const ComponentFactoryFixture&) = delete;
  ComponentFactoryFixture(ComponentFactoryFixture&&) = delete;
  ComponentFactoryFixture& operator=(ComponentFactoryFixture&&) = delete;

  void registerPrimitive(const std::string& type, PrimitiveCreator creator);
  void registerLight(const std::string& type, LightCreator creator);
  void registerMaterial(const std::string& type, MaterialCreator creator);
  void registerCamera(const std::string& type, CameraCreator creator);
  void registerTransformation(const std::string& type,
                              TransformationCreator creator);

  std::shared_ptr<IObject> createPrimitive(
      const std::string& type, const libconfig::Setting& cfg) override;
  std::shared_ptr<ILight> createLight(const std::string& type,
                                      const libconfig::Setting& cfg) override;
  std::shared_ptr<IMaterial> createMaterial(
      const std::string& type, const libconfig::Setting& cfg) override;
  std::shared_ptr<ICamera> createCamera(const std::string& type,
                                        const libconfig::Setting& cfg) override;
  std::shared_ptr<ITransformation> createTransformation(
      const std::string& type, const libconfig::Setting& cfg) override;

 private:
  std::map<std::string, PrimitiveCreator> primitiveCreators_;
  std::map<std::string, LightCreator> lightCreators_;
  std::map<std::string, MaterialCreator> materialCreators_;
  std::map<std::string, CameraCreator> cameraCreators_;
  std::map<std::string, TransformationCreator> transformationCreators_;
};
