/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** IComponentFactory
*/

#ifndef FACTORY_ICOMPONENTFACTORY_HPP_
#define FACTORY_ICOMPONENTFACTORY_HPP_

#include <memory>
#include <string>

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
 * @brief Pure virtual façade above the per-interface sub-factories.
 *
 * `IComponentFactory` is the single dependency injected into
 * @ref raytracer::core::Application and @ref raytracer::scene::SceneBuilder.
 * It exposes one creation method per component interface; each takes the
 * type discriminant (string) and a libconfig setting, and returns a freshly
 * built shared pointer to the requested concrete instance.
 *
 * Implementations route each call to the matching sub-factory
 * (PrimitiveFactory, LightFactory, MaterialFactory, CameraFactory,
 * TransformationFactory) which owns the type-to-creator dispatch and the
 * libconfig parsing into the per-type `XxxParams` struct.
 *
 * @note The interface deliberately exposes only the string-based dispatch
 *       API. Typed creators (`createSphere(SphereParams)`, etc.) live on
 *       the concrete sub-factories and are not part of the contract used
 *       by Application/SceneBuilder.
 */
class IComponentFactory {
 public:
  IComponentFactory() = default;
  virtual ~IComponentFactory() = default;

  IComponentFactory(const IComponentFactory&) = delete;
  IComponentFactory& operator=(const IComponentFactory&) = delete;
  IComponentFactory(IComponentFactory&&) = delete;
  IComponentFactory& operator=(IComponentFactory&&) = delete;

  /**
   * @brief Create a primitive (`IObject`) by registered type name.
   *
   * @param [in] type Name registered with the primitive sub-factory.
   * @param [in] cfg  libconfig setting parsed into the matching Params.
   * @returns Shared pointer to the freshly constructed primitive.
   *
   * @throws raytracer::core::RaytracerException If @p type is unknown.
   */
  virtual std::shared_ptr<IObject> createPrimitive(
      const std::string& type, const libconfig::Setting& cfg) = 0;

  /**
   * @brief Create a light (`ILight`) by registered type name.
   *
   * @param [in] type Name registered with the light sub-factory.
   * @param [in] cfg  libconfig setting parsed into the matching Params.
   * @returns Shared pointer to the freshly constructed light.
   *
   * @throws raytracer::core::RaytracerException If @p type is unknown.
   */
  virtual std::shared_ptr<ILight> createLight(
      const std::string& type, const libconfig::Setting& cfg) = 0;

  /**
   * @brief Create a material (`IMaterial`) by registered type name.
   *
   * @param [in] type Name registered with the material sub-factory.
   * @param [in] cfg  libconfig setting parsed into the matching Params.
   * @returns Shared pointer to the freshly constructed material.
   *
   * @throws raytracer::core::RaytracerException If @p type is unknown.
   */
  virtual std::shared_ptr<IMaterial> createMaterial(
      const std::string& type, const libconfig::Setting& cfg) = 0;

  /**
   * @brief Create a camera (`ICamera`) by registered type name.
   *
   * @param [in] type Name registered with the camera sub-factory.
   * @param [in] cfg  libconfig setting parsed into the matching Params.
   * @returns Shared pointer to the freshly constructed camera.
   *
   * @throws raytracer::core::RaytracerException If @p type is unknown.
   */
  virtual std::shared_ptr<ICamera> createCamera(
      const std::string& type, const libconfig::Setting& cfg) = 0;

  /**
   * @brief Create a transformation (`ITransformation`) by registered type name.
   *
   * @param [in] type Name registered with the transformation sub-factory.
   * @param [in] cfg  libconfig setting parsed into the matching Params.
   * @returns Shared pointer to the freshly constructed transformation.
   *
   * @throws raytracer::core::RaytracerException If @p type is unknown.
   */
  virtual std::shared_ptr<ITransformation> createTransformation(
      const std::string& type, const libconfig::Setting& cfg) = 0;
};

}  // namespace raytracer::core::factory

#endif  // FACTORY_ICOMPONENTFACTORY_HPP_
