/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** WhittedIntegrator highlight regression for specular-only materials
*/

#include <gtest/gtest.h>
#include <memory>
#include "components/Primitives/sphere/Sphere.hpp"
#include "components/light/point/Point.hpp"
#include "components/material/IMaterial.hpp"
#include "components/material/glass/Glass.hpp"
#include "components/material/glossy/Glossy.hpp"
#include "components/material/principledBSDF/PrincipledBSDF.hpp"
#include "rendering/integrator/whittedIntegrator/WhittedIntegrator.hpp"
#include "scene/Scene.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::components::light::point::PointLight;
using raytracer::components::material::Glass;
using raytracer::components::material::Glossy;
using raytracer::components::material::PrincipledMaterial;
using raytracer::components::primitives::Sphere;
using raytracer::core::WhittedIntegrator;
using raytracer::math::Color;
using raytracer::math::Ray;
using raytracer::math::Vector3D;
using raytracer::scene::Scene;

namespace {

constexpr int maxBounceDepth = 4;
constexpr double highlightThreshold = 0.01;

double averageChannel(const Color& color) {
  return (color.r + color.g + color.b) / 3.0;
}

Color renderApexRadiance(const std::shared_ptr<Scene>& scene) {
  WhittedIntegrator integrator;
  const Ray primaryRay(Vector3D(0.0, 0.0, 5.0), Vector3D(0.0, 0.0, -1.0));
  return integrator.computeRadiance(primaryRay, *scene, maxBounceDepth);
}

std::shared_ptr<Scene> sceneWith(const std::shared_ptr<IMaterial>& material) {
  auto scene = std::make_shared<Scene>();
  scene->add(std::make_shared<Sphere>(Vector3D(0.0, 0.0, 0.0), 1.0, material));
  scene->addLight(std::make_shared<PointLight>(
      Vector3D(0.0, 0.0, 3.0), Color(1.0, 1.0, 1.0), 100.0));
  return scene;
}

}  // namespace

// Camera at (0,0,5), sphere of radius 1 at origin → apex hit is (0,0,1) with
// normal (0,0,1). PointLight at (0,0,3) makes the incoming light direction
// (0,0,-1), so the Phong reflection vector R = (0,0,1) is aligned with the
// view direction V = (0,0,1) → R·V = 1 and the highlight fires at peak
// intensity. Same alignment makes the Blinn-Phong half-vector H = (0,0,1)
// align with the normal for PrincipledMaterial.

TEST(WhittedIntegratorHighlightTest, GlossyWithSpecularProducesHighlight) {
  auto material =
      std::make_shared<Glossy>(/*fuzz=*/0.0,
                                /*albedo=*/Color(0.0, 0.0, 0.0),
                                /*specularAlbedo=*/Color(1.0, 1.0, 1.0),
                                /*shininess=*/64.0);
  const Color radiance = renderApexRadiance(sceneWith(material));

  EXPECT_GT(averageChannel(radiance), highlightThreshold)
      << "Glossy with specular params must receive direct PointLight; "
         "before this refactor it was invisible under PointLight.";
}

TEST(WhittedIntegratorHighlightTest, GlassWithSpecularProducesHighlight) {
  auto material = std::make_shared<Glass>(
      /*refractionIndex=*/1.5,
      /*tint=*/Color(1.0, 1.0, 1.0),
      /*specularAlbedo=*/Color(1.0, 1.0, 1.0),
      /*shininess=*/96.0);
  const Color radiance = renderApexRadiance(sceneWith(material));

  EXPECT_GT(averageChannel(radiance), highlightThreshold)
      << "Glass with specular params must receive direct PointLight; "
         "before this refactor it was invisible under PointLight.";
}

TEST(WhittedIntegratorHighlightTest,
     PrincipledMetallicProducesHighlightWithoutCfg) {
  auto material = std::make_shared<PrincipledMaterial>(
      /*baseColor=*/Color(0.8, 0.4, 0.2), /*metallic=*/1.0,
      /*roughness=*/0.0, /*ior=*/1.5, /*alpha=*/1.0);
  const Color radiance = renderApexRadiance(sceneWith(material));

  EXPECT_GT(averageChannel(radiance), highlightThreshold)
      << "Principled metallic must receive deterministic highlight via its "
         "brdf() override; no CFG specular keys required.";
  EXPECT_GT(radiance.r, radiance.b)
      << "Schlick F0 = baseColor; highlight should be tinted (r > b).";
}

TEST(WhittedIntegratorHighlightTest,
     SpecularOnlyMaterialContributesEvenWithoutDiffuse) {
  auto material =
      std::make_shared<Glossy>(/*fuzz=*/0.0,
                                /*albedo=*/Color(0.0, 0.0, 0.0),
                                /*specularAlbedo=*/Color(1.0, 1.0, 1.0),
                                /*shininess=*/64.0);
  auto scene = sceneWith(material);
  const Color radiance = renderApexRadiance(scene);

  EXPECT_GT(averageChannel(radiance), highlightThreshold)
      << "Integrator gate must fire on specularAlbedo > 0 even when "
         "diffuseAlbedo is black; otherwise the direct-lighting loop is "
         "skipped and the regression returns.";
}
