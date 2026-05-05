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
Translation::~Translation() = default;

raytracer::math::Vector3D Translation::apply(
    const raytracer::math::Vector3D& point) const {
  return point + _offset;
}

raytracer::math::Vector3D Translation::applyToNormal(
    const raytracer::math::Vector3D& normal) const {
  return normal;
}

std::shared_ptr<ITransformation> Translation::inverse() const {
  auto inverseTransform = std::make_shared<Translation>();
  inverseTransform->_offset = -_offset;
  return inverseTransform;
}

}  // namespace raytracer::components::transformation

extern "C" gsl::owner<ITransformation*> createTransformations() {
  return new raytracer::components::transformation::Translation();
}

extern "C" void DestroyTransformations(gsl::owner<ITransformation*> transform) {
  delete transform;
}
