/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Ambient occlusion behaviour through the Whitted integrator
*/

#include <gtest/gtest.h>
#include <memory>
#include "components/Primitives/plane/Plane.hpp"
#include "components/Primitives/sphere/Sphere.hpp"
#include "components/light/point/Point.hpp"
#include "components/material/diffuse/DiffuseMaterial.hpp"
#include "rendering/helper/HemisphereSampler.hpp"
#include "rendering/integrator/whittedIntegrator/WhittedIntegrator.hpp"
#include "scene/Scene.hpp"
#include "scene/World.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::components::light::point::PointLight;
using raytracer::components::material::DiffuseMaterial;
using raytracer::components::primitives::Plane;
using raytracer::components::primitives::Sphere;
using raytracer::core::WhittedIntegrator;
using raytracer::math::Color;
using raytracer::math::Ray;
using raytracer::math::Vector3D;
using raytracer::rendering::helper::seedHemisphereSampler;
using raytracer::scene::AmbientOcclusionSettings;
using raytracer::scene::Scene;

namespace {

constexpr int aoSampleCount = 32;
constexpr int maxBounceDepth = 4;

std::shared_ptr<Scene> buildOpenSphereScene() {
  auto scene = std::make_shared<Scene>();
  auto material = std::make_shared<DiffuseMaterial>(Color(1.0, 1.0, 1.0));
  scene->add(std::make_shared<Sphere>(Vector3D(0.0, 0.0, -2.0), 1.0, material));
  scene->addLight(std::make_shared<PointLight>(Vector3D(0.0, 0.0, 0.0),
                                                Color(1.0, 1.0, 1.0), 50.0));
  return scene;
}

std::shared_ptr<Scene> buildEnclosedSphereScene() {
  auto scene = std::make_shared<Scene>();
  auto material = std::make_shared<DiffuseMaterial>(Color(1.0, 1.0, 1.0));
  scene->add(std::make_shared<Sphere>(Vector3D(0.0, 0.0, -2.0), 1.0, material));
  scene->add(std::make_shared<Plane>(Vector3D(0.1, 0.0, -1.0),
                                      Vector3D(-1.0, 0.0, 0.0), material));
  scene->add(std::make_shared<Plane>(Vector3D(-0.1, 0.0, -1.0),
                                      Vector3D(1.0, 0.0, 0.0), material));
  scene->add(std::make_shared<Plane>(Vector3D(0.0, 0.1, -1.0),
                                      Vector3D(0.0, -1.0, 0.0), material));
  scene->add(std::make_shared<Plane>(Vector3D(0.0, -0.1, -1.0),
                                      Vector3D(0.0, 1.0, 0.0), material));
  scene->addLight(std::make_shared<PointLight>(Vector3D(0.0, 0.0, 0.0),
                                                Color(1.0, 1.0, 1.0), 50.0));
  return scene;
}

double averageRadianceMagnitude(const Color& color) {
  return (color.r + color.g + color.b) / 3.0;
}

}  // namespace

TEST(AmbientOcclusionTest, DisabledSettingsLeaveRadianceUnchanged) {
  auto scene = buildOpenSphereScene();
  WhittedIntegrator integrator;
  const Ray primaryRay(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 0.0, -1.0));

  scene->getWorld().setAmbientOcclusion(AmbientOcclusionSettings{});
  const Color baseline =
      integrator.computeRadiance(primaryRay, *scene, maxBounceDepth);

  AmbientOcclusionSettings settings;
  settings.enabled = false;
  settings.samples = aoSampleCount;
  scene->getWorld().setAmbientOcclusion(settings);
  seedHemisphereSampler(1);
  const Color withDisabledAo =
      integrator.computeRadiance(primaryRay, *scene, maxBounceDepth);

  EXPECT_DOUBLE_EQ(baseline.r, withDisabledAo.r);
  EXPECT_DOUBLE_EQ(baseline.g, withDisabledAo.g);
  EXPECT_DOUBLE_EQ(baseline.b, withDisabledAo.b);
}

TEST(AmbientOcclusionTest, OpenSceneIsNearlyFullyLit) {
  auto scene = buildOpenSphereScene();
  WhittedIntegrator integrator;
  const Ray primaryRay(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 0.0, -1.0));

  AmbientOcclusionSettings settings;
  settings.enabled = true;
  settings.samples = aoSampleCount;
  settings.radius = 0.5;
  settings.intensity = 1.0;
  scene->getWorld().setAmbientOcclusion(settings);
  seedHemisphereSampler(2);
  const Color withAo =
      integrator.computeRadiance(primaryRay, *scene, maxBounceDepth);

  scene->getWorld().setAmbientOcclusion(AmbientOcclusionSettings{});
  const Color baseline =
      integrator.computeRadiance(primaryRay, *scene, maxBounceDepth);

  EXPECT_GT(averageRadianceMagnitude(withAo),
            averageRadianceMagnitude(baseline) * 0.9);
}

TEST(AmbientOcclusionTest, EnclosedSceneIsSignificantlyDarkened) {
  auto enclosedScene = buildEnclosedSphereScene();
  auto openScene = buildOpenSphereScene();
  WhittedIntegrator integrator;
  const Ray primaryRay(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 0.0, -1.0));

  AmbientOcclusionSettings settings;
  settings.enabled = true;
  settings.samples = aoSampleCount;
  settings.radius = 2.0;
  settings.intensity = 1.0;
  enclosedScene->getWorld().setAmbientOcclusion(settings);
  openScene->getWorld().setAmbientOcclusion(settings);

  seedHemisphereSampler(3);
  const Color enclosed =
      integrator.computeRadiance(primaryRay, *enclosedScene, maxBounceDepth);
  seedHemisphereSampler(3);
  const Color open =
      integrator.computeRadiance(primaryRay, *openScene, maxBounceDepth);

  EXPECT_LT(averageRadianceMagnitude(enclosed),
            averageRadianceMagnitude(open) * 0.9);
}

TEST(AmbientOcclusionTest, IntensityZeroDisablesDarkeningEvenWhenOccluded) {
  auto enclosedScene = buildEnclosedSphereScene();
  WhittedIntegrator integrator;
  const Ray primaryRay(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 0.0, -1.0));

  AmbientOcclusionSettings settings;
  settings.enabled = true;
  settings.samples = aoSampleCount;
  settings.radius = 2.0;

  settings.intensity = 0.0;
  enclosedScene->getWorld().setAmbientOcclusion(settings);
  seedHemisphereSampler(4);
  const Color zeroIntensity =
      integrator.computeRadiance(primaryRay, *enclosedScene, maxBounceDepth);

  settings.intensity = 1.0;
  enclosedScene->getWorld().setAmbientOcclusion(settings);
  seedHemisphereSampler(4);
  const Color fullIntensity =
      integrator.computeRadiance(primaryRay, *enclosedScene, maxBounceDepth);

  EXPECT_GT(averageRadianceMagnitude(zeroIntensity),
            averageRadianceMagnitude(fullIntensity));
}
