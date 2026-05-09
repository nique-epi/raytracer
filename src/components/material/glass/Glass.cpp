/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Glass
*/

#include "Glass.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Optics.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

namespace {
float discriminant(float niOverNt, float cosTheta) {
  return 1 - ((niOverNt * niOverNt) * (1 - (cosTheta * cosTheta)));
}
}  // namespace

namespace raytracer::components::material {

bool Glass::scatter(const raytracer::math::Ray& in,
                    const raytracer::math::HitRecord& rec,
                    raytracer::math::Color& attenuation,
                    raytracer::math::Ray& scattered) const {
  attenuation = raytracer::math::Color(1.0, 1.0, 1.0);
  double niOverNt = rec.frontFace ? (1.0 / refractionIndex_) : refractionIndex_;
  raytracer::math::Vector3D unitDirection = in.getDirection().normalize();
  float cosTheta = -unitDirection.dot(rec.normal);
  raytracer::math::Vector3D refracted =
      raytracer::math::Optics::refract(unitDirection, rec.normal, niOverNt);

}
}  // namespace raytracer::components::material
