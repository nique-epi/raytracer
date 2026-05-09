/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SceneConfigurator
*/

#include "SceneConfigurator.hpp"
#include <libconfig.h++>
#include <memory>
#include <string>
#include "components/Primitives/cone/Cone.hpp"
#include "components/Primitives/cylinder/Cylinder.hpp"
#include "components/Primitives/plane/Plane.hpp"
#include "components/Primitives/sphere/Sphere.hpp"
#include "components/camera/perspective/PerspectiveCamera.hpp"
#include "components/light/ambient/AmbientLight.hpp"
#include "components/light/directional/Directional.hpp"
#include "components/material/IMaterial.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Ray.hpp"

namespace {

using Sphere      = raytracer::components::primitives::Sphere;
using Plane       = raytracer::components::primitives::Plane;
using Cylinder    = raytracer::components::primitives::Cylinder;
using Cone        = raytracer::components::primitives::Cone;
using Ambient     = raytracer::components::light::ambient::Ambient;
using Directional = raytracer::components::light::directional::Directional;
using Perspective = raytracer::components::camera::perspective::Perspective;

constexpr double rgbMaxValue = 255.0;

// Flat-color material used until the full material pipeline is wired.
class ColorMaterial : public IMaterial {
 public:
  explicit ColorMaterial(const raytracer::math::Color& albedo)
      : albedo_(albedo) {}
  bool scatter(const raytracer::math::Ray&,
               const raytracer::math::HitRecord&,
               raytracer::math::Color& attenuation,
               raytracer::math::Ray&) const override {
    attenuation = albedo_;
    return true;
  }
  raytracer::math::Color emitted() const override { return {0.0, 0.0, 0.0}; }
 private:
  raytracer::math::Color albedo_;
};

raytracer::math::Color colorFromBlock(const libconfig::Setting& colorBlock) {
  int red = 255, green = 255, blue = 255;
  colorBlock.lookupValue("r", red);
  colorBlock.lookupValue("g", green);
  colorBlock.lookupValue("b", blue);
  return {red / rgbMaxValue, green / rgbMaxValue, blue / rgbMaxValue};
}

std::shared_ptr<IMaterial> materialFromConfig(const libconfig::Setting& cfg) {
  if (!cfg.exists("color")) {
    return nullptr;
  }
  return std::make_shared<ColorMaterial>(colorFromBlock(cfg["color"]));
}

void configureSphere(std::shared_ptr<IObject> obj,
                     const libconfig::Setting& cfg) {
  auto sphere = std::static_pointer_cast<Sphere>(obj);
  double x = 0.0, y = 0.0, z = 0.0;
  if (cfg.exists("center")) {
    cfg["center"].lookupValue("x", x);
    cfg["center"].lookupValue("y", y);
    cfg["center"].lookupValue("z", z);
  }
  double radius = 1.0;
  cfg.lookupValue("radius", radius);
  sphere->setCenter({x, y, z});
  sphere->setRadius(radius);
  sphere->setMaterial(materialFromConfig(cfg));
}

void configurePlane(std::shared_ptr<IObject> obj,
                    const libconfig::Setting& cfg) {
  auto plane = std::static_pointer_cast<Plane>(obj);
  std::string axis = "Y";
  double position = 0.0;
  cfg.lookupValue("axis", axis);
  cfg.lookupValue("position", position);
  if (axis == "X") {
    plane->setPoint({position, 0.0, 0.0});
    plane->setNormal({1.0, 0.0, 0.0});
  } else if (axis == "Z") {
    plane->setPoint({0.0, 0.0, position});
    plane->setNormal({0.0, 0.0, 1.0});
  } else {
    plane->setPoint({0.0, position, 0.0});
    plane->setNormal({0.0, 1.0, 0.0});
  }
  plane->setMaterial(materialFromConfig(cfg));
}

void configureCylinder(std::shared_ptr<IObject> obj,
                       const libconfig::Setting& cfg) {
  auto cylinder = std::static_pointer_cast<Cylinder>(obj);
  if (cfg.exists("center")) {
    double x = 0.0, y = 0.0, z = 0.0;
    cfg["center"].lookupValue("x", x);
    cfg["center"].lookupValue("y", y);
    cfg["center"].lookupValue("z", z);
    cylinder->setCenter({x, y, z});
  }
  if (cfg.exists("axis")) {
    double x = 0.0, y = 1.0, z = 0.0;
    cfg["axis"].lookupValue("x", x);
    cfg["axis"].lookupValue("y", y);
    cfg["axis"].lookupValue("z", z);
    cylinder->setAxis({x, y, z});
  }
  double radius = 1.0, height = -1.0;
  cfg.lookupValue("radius", radius);
  cfg.lookupValue("height", height);
  cylinder->setRadius(radius);
  cylinder->setHeight(height);
  cylinder->setMaterial(materialFromConfig(cfg));
}

void configureCone(std::shared_ptr<IObject> obj,
                   const libconfig::Setting& cfg) {
  auto cone = std::static_pointer_cast<Cone>(obj);
  if (cfg.exists("apex")) {
    double x = 0.0, y = 0.0, z = 0.0;
    cfg["apex"].lookupValue("x", x);
    cfg["apex"].lookupValue("y", y);
    cfg["apex"].lookupValue("z", z);
    cone->setApex({x, y, z});
  }
  if (cfg.exists("axis")) {
    double x = 0.0, y = 1.0, z = 0.0;
    cfg["axis"].lookupValue("x", x);
    cfg["axis"].lookupValue("y", y);
    cfg["axis"].lookupValue("z", z);
    cone->setAxis({x, y, z});
  }
  double halfAngle = 0.785398, height = 1.0;
  cfg.lookupValue("angle", halfAngle);
  cfg.lookupValue("height", height);
  cone->setHalfAngle(halfAngle);
  cone->setHeight(height);
  cone->setMaterial(materialFromConfig(cfg));
}

void configureAmbient(std::shared_ptr<ILight> light,
                      const libconfig::Setting& cfg) {
  auto ambient = std::static_pointer_cast<Ambient>(light);
  if (cfg.exists("color")) {
    ambient->setColor(colorFromBlock(cfg["color"]));
  }
  double intensity = 1.0;
  cfg.lookupValue("intensity", intensity);
  ambient->setIntensity(intensity);
}

void configureDirectional(std::shared_ptr<ILight> light,
                          const libconfig::Setting& cfg) {
  auto directional = std::static_pointer_cast<Directional>(light);
  if (cfg.exists("direction")) {
    double x = 0.0, y = -1.0, z = 0.0;
    cfg["direction"].lookupValue("x", x);
    cfg["direction"].lookupValue("y", y);
    cfg["direction"].lookupValue("z", z);
    directional->setDirection({x, y, z});
  }
  if (cfg.exists("color")) {
    directional->setColor(colorFromBlock(cfg["color"]));
  }
  double intensity = 1.0;
  cfg.lookupValue("intensity", intensity);
  directional->setIntensity(intensity);
}

}  // namespace

namespace raytracer::scene {

void configureObject(const std::string& type, std::shared_ptr<IObject> obj,
                     const libconfig::Setting& cfg) {
  if (type == "sphere")   { configureSphere(obj, cfg);   return; }
  if (type == "plane")    { configurePlane(obj, cfg);    return; }
  if (type == "cylinder") { configureCylinder(obj, cfg); return; }
  if (type == "cone")     { configureCone(obj, cfg);     return; }
}

void configureLight(const std::string& type, std::shared_ptr<ILight> light,
                    const libconfig::Setting& cfg) {
  if (type == "ambient")     { configureAmbient(light, cfg);     return; }
  if (type == "directional") { configureDirectional(light, cfg); return; }
}

void configurePerspectiveCamera(std::shared_ptr<ICamera> camera,
                                const libconfig::Setting& cfg) {
  auto perspective = std::static_pointer_cast<Perspective>(camera);
  if (cfg.exists("position")) {
    double x = 0.0, y = 0.0, z = 0.0;
    cfg["position"].lookupValue("x", x);
    cfg["position"].lookupValue("y", y);
    cfg["position"].lookupValue("z", z);
    perspective->setPosition({x, y, z});
  }
  if (cfg.exists("direction")) {
    double x = 0.0, y = 0.0, z = -1.0;
    cfg["direction"].lookupValue("x", x);
    cfg["direction"].lookupValue("y", y);
    cfg["direction"].lookupValue("z", z);
    perspective->setDirection({x, y, z});
  }
  double fieldOfView = 90.0;
  cfg.lookupValue("fov", fieldOfView);
  perspective->setFieldOfView(fieldOfView);
}

}  // namespace raytracer::scene
