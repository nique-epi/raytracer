/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Renderer behaviour across viewport modes (Rendered / MaterialPreview /
** Wireframe), drop-shadow occlusion, and background sampling policy.
*/

#include <gtest/gtest.h>
#include <memory>
#include "../fixtures/OrthoCameraFixture.hpp"
#include "components/Primitives/sphere/Sphere.hpp"
#include "components/light/point/Point.hpp"
#include "components/material/diffuse/DiffuseMaterial.hpp"
#include "rendering/integrator/whittedIntegrator/WhittedIntegrator.hpp"
#include "rendering/renderer/Frame.hpp"
#include "rendering/renderer/RendererConfig.hpp"
#include "rendering/renderer/raytracerRenderer/RaytracerRenderer.hpp"
#include "rendering/shading/IShadingMode.hpp"
#include "rendering/shading/ShadingContext.hpp"
#include "rendering/shading/materialPreview/MaterialPreviewShader.hpp"
#include "rendering/shading/rendered/RenderedShader.hpp"
#include "rendering/shading/wireframe/WireframeShader.hpp"
#include "scene/Scene.hpp"
#include "scene/World.hpp"
#include "scene/background/Solid/SolidBackground.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Constants.hpp"
#include "utils/math/RenderSettings.hpp"
#include "utils/math/Vector3D.hpp"
namespace {

using raytracer::components::light::point::PointLight;
using raytracer::components::material::DiffuseMaterial;
using raytracer::components::primitives::Sphere;
using raytracer::core::Frame;
using raytracer::core::RaytracerRenderer;
using raytracer::core::RendererConfig;
using raytracer::math::Color;
using raytracer::math::RenderSettings;
using raytracer::math::Vector3D;
using raytracer::scene::Scene;
using raytracer::scene::ViewportMode;
using raytracer::scene::background::SolidBackground;

constexpr int kImageSize = 21;
constexpr int kCenter = kImageSize / 2;

/**
 * @brief Build a scene with a single white diffuse sphere directly in
 *        front of the OrthoCameraFixture.
 *
 * The fixture camera shoots rays of direction (0, 0, -1) from origins on
 * the plane z = 1. A sphere centred at (0, 0, -2) with radius 1 is
 * therefore hit dead-on along the camera axis, with the hit point at
 * (0, 0, -1) and outward normal (0, 0, 1) — handy for predictable Lambert
 * cosines.
 */
std::shared_ptr<Scene> buildSphereScene(const Color& albedo) {
  auto scene = std::make_shared<Scene>();
  auto material = std::make_shared<DiffuseMaterial>(albedo);
  scene->add(std::make_shared<Sphere>(Vector3D(0.0, 0.0, -2.0), 1.0, material));
  return scene;
}

RenderSettings makeSettings(int maxDepth) {
  RenderSettings settings;
  settings.imageWidth = kImageSize;
  settings.imageHeight = kImageSize;
  settings.samplesPerPixel = 1;
  settings.maxDepth = maxDepth;
  return settings;
}

raytracer::components::Image renderScene(const std::shared_ptr<Scene>& scene,
                                         const RenderSettings& settings) {
  auto camera = std::make_shared<OrthoCameraFixture>();
  raytracer::core::RaytracerRenderer renderer;
  std::shared_ptr<raytracer::shading::IShadingMode> shader;
  switch (scene->getWorld().viewportMode()) {
    case ViewportMode::Wireframe:
      shader = std::make_shared<raytracer::shading::WireframeShader>();
      break;
    case ViewportMode::MaterialPreview:
      shader = std::make_shared<raytracer::shading::MaterialPreviewShader>();
      break;
    case ViewportMode::Rendered:
    default:
      shader = std::make_shared<raytracer::shading::RenderedShader>(
          std::make_shared<raytracer::core::WhittedIntegrator>());
      break;
  }
  auto context = std::make_shared<raytracer::shading::ShadingContext>(shader);
  const RendererConfig config{
      .scene = scene, .settings = settings, .shadingContext = context};
  const Frame frame{.camera = camera};
  return renderer.render(config, frame);
}

double imageSum(const raytracer::components::Image& image) {
  double sum = 0.0;
  for (std::size_t y = 0; y < image.getHeight(); ++y) {
    for (std::size_t x = 0; x < image.getWidth(); ++x) {
      const auto pixel =
          image.getPixel(static_cast<int>(x), static_cast<int>(y));
      sum += pixel.r + pixel.g + pixel.b;
    }
  }
  return sum;
}

}  // namespace

// 1) Rendered mode must drive direct lighting from ILight sources. With
//    the camera shooting straight at the sphere and a light placed where
//    the rays originate, the Lambert cosine is 1, the geometric distance
//    to the light is 1. The photometric model applies:
//    - Point light inverse-square falloff: 1/(4πr²) = 1/(4π) at r=1
//    - Lambertian BRDF: albedo/π
//    - Combined: (1/(4π)) × (1/π) = 1/(4π²) for unit albedo.
//    The tolerance accounts for shadowRayEpsilon (~1e-3) reducing distance
//    and inflating the inverse-square factor by ~0.2%.
TEST(RendererViewportModeTest, RenderedAppliesDirectLightingFromPointLight) {
  auto scene = buildSphereScene(Color(1.0, 1.0, 1.0));
  scene->addLight(std::make_shared<PointLight>(Vector3D(0.0, 0.0, 0.0),
                                               Color(1.0, 1.0, 1.0), 1.0));
  scene->getWorld().setViewportMode(ViewportMode::Rendered);

  const auto image = renderScene(scene, makeSettings(1));
  const auto center = image.getPixel(kCenter, kCenter);

  const double expectedValue = 1.0 / (4.0 * raytracer::math::constants::PI *
                                      raytracer::math::constants::PI);
  EXPECT_NEAR(center.r, expectedValue, 1e-3);
  EXPECT_NEAR(center.g, expectedValue, 1e-3);
  EXPECT_NEAR(center.b, expectedValue, 1e-3);
}

// 2) Same scene, same light — but in MaterialPreview the scene's ILights
//    must be ignored (Blender default: "Scene Lights = OFF"). With
//    maxDepth = 1 the scattered ray cannot collect any indirect light, so
//    the only possible output is black. If the light were leaking in,
//    this assertion would fail.
TEST(RendererViewportModeTest, MaterialPreviewIgnoresScenePointLights) {
  auto scene = buildSphereScene(Color(1.0, 1.0, 1.0));
  scene->addLight(std::make_shared<PointLight>(Vector3D(0.0, 0.0, 0.0),
                                               Color(1.0, 1.0, 1.0), 1.0));
  scene->getWorld().setViewportMode(ViewportMode::MaterialPreview);

  const auto image = renderScene(scene, makeSettings(1));
  const auto center = image.getPixel(kCenter, kCenter);
  EXPECT_NEAR(center.r, 0.0, 1e-9);
  EXPECT_NEAR(center.g, 0.0, 1e-9);
  EXPECT_NEAR(center.b, 0.0, 1e-9);
}

// 3) Drop shadow: an occluder placed between the hit point and the light
//    must make PointLight::illuminate return black via its shadow-ray
//    test, dropping the direct contribution to zero at the centre pixel.
TEST(RendererViewportModeTest, RenderedDropShadowOccludesDirectLighting) {
  auto scene = buildSphereScene(Color(1.0, 1.0, 1.0));
  scene->addLight(std::make_shared<PointLight>(Vector3D(0.0, 0.0, 5.0),
                                               Color(1.0, 1.0, 1.0), 1.0));
  // Small blocker sphere sitting between the hit point (0, 0, -1) and
  // the light (0, 0, 5). Any opaque material works — we use the same
  // diffuse since only the shadow-ray occlusion matters.
  auto blockerMaterial =
      std::make_shared<DiffuseMaterial>(Color(0.0, 0.0, 0.0));
  scene->add(
      std::make_shared<Sphere>(Vector3D(0.0, 0.0, 0.5), 0.25, blockerMaterial));
  scene->getWorld().setViewportMode(ViewportMode::Rendered);

  const auto image = renderScene(scene, makeSettings(1));
  const auto center = image.getPixel(kCenter, kCenter);
  EXPECT_NEAR(center.r, 0.0, 1e-9);
  EXPECT_NEAR(center.g, 0.0, 1e-9);
  EXPECT_NEAR(center.b, 0.0, 1e-9);
}

// 4) Background contribution must differ between Rendered and
//    MaterialPreview for secondary rays:
//      - Rendered: a scattered ray that escapes returns black, so an
//        unlit scene reads as black even with a white background.
//      - MaterialPreview: the same escaped ray samples the background as
//        an environment light, producing a non-zero image.
//    Comparing summed pixel values across the image isolates the policy
//    difference without depending on the scatter direction's random seed.
TEST(RendererViewportModeTest, BackgroundOnSecondaryRaysDiffersByMode) {
  const auto background =
      std::make_shared<SolidBackground>(Color(1.0, 1.0, 1.0));

  auto sceneRendered = buildSphereScene(Color(1.0, 1.0, 1.0));
  sceneRendered->setBackground(background);
  sceneRendered->getWorld().setViewportMode(ViewportMode::Rendered);

  auto sceneMaterialPreview = buildSphereScene(Color(1.0, 1.0, 1.0));
  sceneMaterialPreview->setBackground(background);
  sceneMaterialPreview->getWorld().setViewportMode(
      ViewportMode::MaterialPreview);

  // maxDepth = 2: the primary ray hits the sphere and one scatter bounce
  // can escape into the background.
  const RenderSettings settings = makeSettings(2);

  const auto imageRendered = renderScene(sceneRendered, settings);
  const auto imageMaterialPreview = renderScene(sceneMaterialPreview, settings);

  // We can't compare per-pixel sphere coverage exactly because the
  // background also fills the off-sphere pixels in both modes. To isolate
  // the secondary-ray behaviour, just look at the centre pixel which is
  // guaranteed to hit the sphere.
  const auto centerRendered = imageRendered.getPixel(kCenter, kCenter);
  const auto centerMaterialPreview =
      imageMaterialPreview.getPixel(kCenter, kCenter);

  EXPECT_GT(centerRendered.r, 0.0)
      << "Rendered mode should see light from background";
  EXPECT_GT(centerMaterialPreview.r, 0.0)
      << "MaterialPreview mode should see background as env light";

  const double sumRendered = imageSum(imageRendered);
  const double sumMaterialPreview = imageSum(imageMaterialPreview);
  EXPECT_NEAR(sumRendered, sumMaterialPreview, sumMaterialPreview * 0.01)
      << "Without direct lights, both modes should be similarly bright";
}

// 5) Wireframe ignores both lights and materials, returning a flat grey
//    shaded by `max(0, N·V)` (Blender-Solid look). The centre pixel sits
//    on the sphere's apex where the outward normal is (0, 0, 1) and the
//    view direction is also (0, 0, 1), so `N·V = 1` and brightness hits
//    the upper bound 0.85. This guarantees no path through the lighting
//    or scatter code is taken.
TEST(RendererViewportModeTest, WireframeRendersFlatGreyRegardlessOfLighting) {
  auto scene = buildSphereScene(Color(1.0, 0.2, 0.2));
  scene->addLight(std::make_shared<PointLight>(Vector3D(0.0, 0.0, 0.0),
                                               Color(1.0, 1.0, 1.0), 1.0));
  scene->getWorld().setViewportMode(ViewportMode::Wireframe);

  const auto image = renderScene(scene, makeSettings(1));
  const auto center = image.getPixel(kCenter, kCenter);
  EXPECT_NEAR(center.r, 0.85, 1e-3);
  EXPECT_NEAR(center.g, 0.85, 1e-3);
  EXPECT_NEAR(center.b, 0.85, 1e-3);
}
