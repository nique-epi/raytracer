#include "Glass.hpp"
#include <cmath>
#include <random>
#include "utils/math/HitRecord.hpp"
#include "utils/math/Optics.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

namespace {
double discriminant(double niOverNt, double cosTheta) {
  return 1.0 - ((niOverNt * niOverNt) * (1.0 - (cosTheta * cosTheta)));
}

double randomDouble() {
  thread_local std::mt19937 generator(std::random_device{}());
  thread_local std::uniform_real_distribution<double> distribution(0.0, 1.0);

  return distribution(generator);
}

}  // namespace

namespace raytracer::components::material {

bool Glass::scatter(const raytracer::math::Ray& in,
                    const raytracer::math::HitRecord& rec,
                    raytracer::math::Color& attenuation,
                    raytracer::math::Ray& scattered) const {
  attenuation = raytracer::math::Color(1.0, 1.0, 1.0);

  double niOverNt = rec.frontFace ? (1.0 / refractionIndex_) : refractionIndex_;
  raytracer::math::Vector3D unitDir = in.getDirection().normalize();
  double cosTheta = -unitDir.dot(rec.normal);

  if (discriminant(niOverNt, cosTheta) < 0) {
    scattered = raytracer::math::Ray(
        rec.point, raytracer::math::Optics::reflect(unitDir, rec.normal));
    return true;
  }

  double reflectance = raytracer::math::Optics::schlick(cosTheta, niOverNt);

  if (randomDouble() < reflectance) {
    scattered = raytracer::math::Ray(
        rec.point, raytracer::math::Optics::reflect(unitDir, rec.normal));
  } else {
    scattered = raytracer::math::Ray(
        rec.point,
        raytracer::math::Optics::refract(unitDir, rec.normal, niOverNt));
  }
  return true;
}

}  // namespace raytracer::components::material

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

extern "C" gsl::owner<IMaterial*> createMaterial(double refractionIndex) {
  return new raytracer::components::material::Glass(refractionIndex);
}

extern "C" void destroyMaterial(gsl::owner<IMaterial*> mat) { delete mat; }
