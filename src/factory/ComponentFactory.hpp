/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ComponentFactory
*/

#pragma once

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

namespace raytracer::core::factory {

/**
 * @brief Concrete implementation of `IComponentFactory`.
 *
 * Each `createX(type, cfg)` call delegates to the matching sub-factory
 * (`PrimitiveFactory`, `LightFactory`, `MaterialFactory`, `CameraFactory`,
 * `TransformationFactory`). The sub-factories themselves are stateless
 * static dispatchers, so this class holds no member state — but it is the
 * unique injection point for `Application` and `SceneBuilder`.
 */
class ComponentFactory : public IComponentFactory {
 public:
  ComponentFactory() = default;
  ~ComponentFactory() override = default;

  ComponentFactory(const ComponentFactory&) = delete;
  ComponentFactory& operator=(const ComponentFactory&) = delete;
  ComponentFactory(ComponentFactory&&) = delete;
  ComponentFactory& operator=(ComponentFactory&&) = delete;

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
};

}  // namespace raytracer::core::factory
