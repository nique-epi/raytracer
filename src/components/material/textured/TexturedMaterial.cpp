/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** TexturedMaterial
*/

#include "TexturedMaterial.hpp"
#include <cmath>
#include <memory>
#include <random>
#include <utility>
#include "components/material/Textures/ITexture.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

constexpr double nearZeroEps = 1e-8;

static raytracer::math::Vector3D randomUnitVector() {
  thread_local std::mt19937 gen(std::random_device{}());
  thread_local std::uniform_real_distribution<double> dist(-1.0, 1.0);

  while (true) {
    const raytracer::math::Vector3D candidate(dist(gen), dist(gen), dist(gen));
    const double lenSq = candidate.lengthSquared();
    if (lenSq > 0.0 && lenSq < 1.0) {
      return candidate.normalize();
    }
  }
}

static bool nearZero(const raytracer::math::Vector3D& vector) {
  return std::fabs(vector.x) < nearZeroEps &&
         std::fabs(vector.y) < nearZeroEps && std::fabs(vector.z) < nearZeroEps;
}

namespace raytracer::components::material {

TexturedMaterial::TexturedMaterial(
    std::shared_ptr<raytracer::materials::ITexture> texture,
    const raytracer::math::Color& albedo)
    : texture_(std::move(texture)), albedo_(albedo) {}

bool TexturedMaterial::scatter(const raytracer::math::Ray& /*in*/,
                               const raytracer::math::HitRecord& rec,
                               raytracer::math::Color& attenuation,
                               raytracer::math::Ray& scattered) const {
  raytracer::math::Vector3D scatterDir = rec.normal + randomUnitVector();
  if (nearZero(scatterDir)) {
    scatterDir = rec.normal;
  }
  scattered = raytracer::math::Ray(rec.point, scatterDir);
  attenuation = texture_->sample(rec.u, rec.v, rec.point) * albedo_;
  return true;
}

}  // namespace raytracer::components::material
