/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PrincipledMaterial
*/

#include "PrincipledBSDF.hpp"
#include <algorithm>
#include <cmath>
#include <random>
#include "utils/math/Constants.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Optics.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

namespace {
constexpr double transparencyRayEpsilon = 1e-4;
constexpr double maximumSpecularBoost = 0.25;
constexpr double dielectricBaseReflectance = 0.04;
constexpr double maximumShininess = 255.0;
constexpr double smoothnessExponent = 4.0;

double generateRandomDouble() {
  thread_local std::mt19937 generator(std::random_device{}());
  thread_local std::uniform_real_distribution<double> distribution(0.0, 1.0);
  return distribution(generator);
}
}  // namespace

namespace raytracer::components::material {

PrincipledMaterial::PrincipledMaterial(const math::Color& baseColor,
                                       double metallic, double roughness,
                                       double ior, double alpha)
    : baseColor_(baseColor),
      metallic_(std::clamp(metallic, 0.0, 1.0)),
      roughness_(std::clamp(roughness, 0.0, 1.0)),
      ior_(ior),
      alpha_(std::clamp(alpha, 0.0, 1.0)) {}

bool PrincipledMaterial::scatter(const raytracer::math::Ray& in,
                                 const raytracer::math::HitRecord& rec,
                                 raytracer::math::Color& attenuation,
                                 raytracer::math::Ray& scattered) const {
  if (generateRandomDouble() > alpha_) {
    const math::Vector3D transparentDirection = in.getDirection().normalize();
    scattered =
        math::Ray(rec.point + (transparentDirection * transparencyRayEpsilon),
                  transparentDirection);
    attenuation = math::Color(1.0, 1.0, 1.0);
    return true;
  }

  const double randomValue = generateRandomDouble();

  if (randomValue < metallic_) {
    return scatterMetallic(in, rec, attenuation, scattered);
  }

  const math::Vector3D normalizedIncomingDirection =
      in.getDirection().normalize();
  const double cosineBetweenIncomingAndNormal =
      std::fmin(-normalizedIncomingDirection.dot(rec.normal), 1.0);

  const double refractiveIndexRatio = rec.frontFace ? (1.0 / ior_) : ior_;
  const double fresnelReflectance = math::Optics::schlick(
      cosineBetweenIncomingAndNormal, refractiveIndexRatio);
  const double specularBoost =
      maximumSpecularBoost * (1.0 - roughness_) * (1.0 - metallic_);
  const double specularProbability =
      std::clamp(fresnelReflectance + specularBoost, 0.0, 1.0);

  if (generateRandomDouble() < specularProbability) {
    return scatterSpecularReflection(in, rec, attenuation, scattered);
  }

  return scatterDiffuseReflection(rec, attenuation, scattered);
}

bool PrincipledMaterial::scatterMetallic(
    const raytracer::math::Ray& incomingRay,
    const raytracer::math::HitRecord& hitRecord,
    raytracer::math::Color& attenuation,
    raytracer::math::Ray& scatteredRay) const {
  const math::Vector3D normalizedIncomingDirection =
      incomingRay.getDirection().normalize();
  const math::Vector3D reflectedDirection =
      math::Optics::reflect(normalizedIncomingDirection, hitRecord.normal);

  const math::Vector3D roughnessOffset =
      roughness_ * math::Vector3D::randomInUnitSphere();
  const math::Vector3D scatteredDirection =
      reflectedDirection + roughnessOffset;

  if (scatteredDirection.dot(hitRecord.normal) <= 0) {
    return false;
  }

  scatteredRay = math::Ray(hitRecord.point, scatteredDirection);
  attenuation = baseColor_;
  return true;
}

bool PrincipledMaterial::scatterSpecularReflection(
    const raytracer::math::Ray& incomingRay,
    const raytracer::math::HitRecord& hitRecord,
    raytracer::math::Color& attenuation,
    raytracer::math::Ray& scatteredRay) const {
  const math::Vector3D normalizedIncomingDirection =
      incomingRay.getDirection().normalize();
  const math::Vector3D reflectedDirection =
      math::Optics::reflect(normalizedIncomingDirection, hitRecord.normal);

  const math::Vector3D roughnessOffset =
      roughness_ * math::Vector3D::randomInUnitSphere();
  const math::Vector3D scatteredDirection =
      reflectedDirection + roughnessOffset;

  if (scatteredDirection.dot(hitRecord.normal) <= 0) {
    return false;
  }

  scatteredRay = math::Ray(hitRecord.point, scatteredDirection);
  attenuation = math::Color(1.0, 1.0, 1.0);
  return true;
}

bool PrincipledMaterial::scatterDiffuseReflection(
    const raytracer::math::HitRecord& hitRecord,
    raytracer::math::Color& attenuation,
    raytracer::math::Ray& scatteredRay) const {
  math::Vector3D scatteredDirection =
      hitRecord.normal + math::Vector3D::randomInUnitSphere();

  if (scatteredDirection.isNearZero()) {
    scatteredDirection = hitRecord.normal;
  }

  scatteredRay = math::Ray(hitRecord.point, scatteredDirection);
  attenuation = baseColor_;
  return true;
}

math::Color PrincipledMaterial::diffuseAlbedo() const {
  return baseColor_ * (1.0 - metallic_) * alpha_;
}

math::Color PrincipledMaterial::specularAlbedo() const {
  const math::Color dielectricF0(dielectricBaseReflectance,
                                  dielectricBaseReflectance,
                                  dielectricBaseReflectance);
  return (dielectricF0 * (1.0 - metallic_)) + (baseColor_ * metallic_);
}

double PrincipledMaterial::shininess() const {
  const double smoothness = 1.0 - roughness_;
  return (std::pow(smoothness, smoothnessExponent) * maximumShininess) + 1.0;
}

math::Color PrincipledMaterial::brdf(
    const raytracer::math::Vector3D& incomingDirection,
    const raytracer::math::Vector3D& outgoingDirection,
    const raytracer::math::Vector3D& normal) const {
  const math::Color diffuse =
      baseColor_ * (1.0 - metallic_) * alpha_ / math::constants::PI;
  const math::Vector3D halfVector =
      (-incomingDirection + outgoingDirection).normalize();
  const double alignment = std::max(0.0, normal.dot(halfVector));
  const math::Color fresnelF0 = specularAlbedo();
  const math::Color specular =
      fresnelF0 * std::pow(alignment, shininess());
  return diffuse + specular;
}

}  // namespace raytracer::components::material
