/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ComponentFactory integration tests
*/

#include <gtest/gtest.h>
#include <libconfig.h++>
#include <memory>
#include "exceptions/Exceptions.hpp"
#include "factory/ComponentFactory.hpp"
#include "factory/IComponentFactory.hpp"

using raytracer::core::factory::ComponentFactory;
using raytracer::core::factory::IComponentFactory;

namespace {

class CfgFromString {
 public:
  explicit CfgFromString(const char* source) {
    config_.readString(source);
  }

  [[nodiscard]] const libconfig::Setting& at(const char* path) const {
    return config_.lookup(path);
  }

 private:
  libconfig::Config config_;
};

}  // namespace

// Given: a ComponentFactory used through the IComponentFactory interface.
// When:  each createX is invoked with a known type and a libconfig setting.
// Then:  the call routes to the matching sub-factory and returns a non-null
//        component.
TEST(ComponentFactoryTest, AllInterfacesDispatchThroughBase) {
  ComponentFactory concrete;
  IComponentFactory& factory = concrete;

  CfgFromString sphereCfg(
      "entry = { center = { x = 0.0; y = 0.0; z = 0.0; }; radius = 1.0; };");
  CfgFromString lightCfg("entry = { intensity = 0.5; };");
  CfgFromString matCfg("entry = { };");
  CfgFromString cameraCfg("entry = { };");
  CfgFromString translationCfg(
      "entry = { offset = { x = 1.0; y = 0.0; z = 0.0; }; };");

  EXPECT_NE(factory.createPrimitive("sphere", sphereCfg.at("entry")), nullptr);
  EXPECT_NE(factory.createLight("ambient", lightCfg.at("entry")), nullptr);
  EXPECT_NE(factory.createMaterial("diffuse", matCfg.at("entry")), nullptr);
  EXPECT_NE(factory.createCamera("perspective", cameraCfg.at("entry")),
            nullptr);
  EXPECT_NE(factory.createTransformation("translation",
                                          translationCfg.at("entry")),
            nullptr);
}

// Given: an unknown type name on every interface.
// When:  each createX is called.
// Then:  each one throws RaytracerException.
TEST(ComponentFactoryTest, UnknownTypeOnEachInterfaceThrows) {
  ComponentFactory factory;
  CfgFromString cfg("entry = { };");
  EXPECT_THROW(
      static_cast<void>(factory.createPrimitive("nope", cfg.at("entry"))),
      raytracer::core::RaytracerException);
  EXPECT_THROW(static_cast<void>(factory.createLight("nope", cfg.at("entry"))),
               raytracer::core::RaytracerException);
  EXPECT_THROW(
      static_cast<void>(factory.createMaterial("nope", cfg.at("entry"))),
      raytracer::core::RaytracerException);
  EXPECT_THROW(static_cast<void>(factory.createCamera("nope", cfg.at("entry"))),
               raytracer::core::RaytracerException);
  EXPECT_THROW(
      static_cast<void>(factory.createTransformation("nope", cfg.at("entry"))),
      raytracer::core::RaytracerException);
}
