/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Translation
*/

#include "Translation.hpp"
#include "components/Transformations/ITransformation.hpp"
#include "utils/math/Vector3D.hpp"

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

namespace raytracer::components::transformation {
Translation::Translation(const raytracer::math::Vector3D& offset)
    : _offset(offset) {}

Translation::~Translation() = default;

void Translation::setOffset(const raytracer::math::Vector3D& offset) {
  _offset = offset;
}

raytracer::math::Vector3D Translation::apply(
    const raytracer::math::Vector3D& point) const {
  return point + _offset;
}

raytracer::math::Vector3D Translation::applyToNormal(
    const raytracer::math::Vector3D& normal) const {
  return normal;
}

std::shared_ptr<ITransformation> Translation::inverse() const {
  auto inverseTransform = std::make_shared<Translation>(-_offset);
  return inverseTransform;
}

}  // namespace raytracer::components::transformation

extern "C" gsl::owner<ITransformation*> createTransformations() {
  return new raytracer::components::transformation::Translation();
}

extern "C" void destroyTransformations(gsl::owner<ITransformation*> transform) {
  delete transform;
}
