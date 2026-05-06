/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** DiffuseMaterial
*/

#include "DiffuseMaterial.hpp"
#include <cmath>
#include <random>
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

namespace {

constexpr double nearZeroEps = 1e-8;

raytracer::math::Vector3D randomUnitVector() {
  thread_local std::mt19937 gen(std::random_device{}());
  thread_local std::uniform_real_distribution<double> dist(-1.0, 1.0);

  while (true) {
    raytracer::math::Vector3D p(dist(gen), dist(gen), dist(gen));
    double lenSq = p.lengthSquared();
    if (lenSq > 0.0 && lenSq < 1.0) {
      return p.normalize();
    }
  }
}

bool nearZero(const raytracer::math::Vector3D& v) {
  return std::fabs(v.x) < nearZeroEps && std::fabs(v.y) < nearZeroEps &&
         std::fabs(v.z) < nearZeroEps;
}

}  // namespace

DiffuseMaterial::DiffuseMaterial(const raytracer::math::Color& albedo)
    : albedo(albedo) {}

bool DiffuseMaterial::scatter(const raytracer::math::Ray& /*in*/,
                              const raytracer::math::HitRecord& rec,
                              raytracer::math::Color& attenuation,
                              raytracer::math::Ray& scattered) const {
  raytracer::math::Vector3D scatterDir = rec.normal + randomUnitVector();
  if (nearZero(scatterDir)) {
    scatterDir = rec.normal;
  }
  scattered = raytracer::math::Ray(rec.point, scatterDir);
  attenuation = albedo;
  return true;
}

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

extern "C" gsl::owner<IMaterial*> createMaterial() {
  return new DiffuseMaterial(raytracer::math::Color(1.0, 1.0, 1.0));
}

extern "C" void destroyMaterial(gsl::owner<IMaterial*> mat) { delete mat; }
