/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Ray
*/

#include "Ray.hpp"
#include "Vector3D.hpp"

namespace raytracer::math {

Ray::Ray(const Vector3D& origin, const Vector3D& direction)
    : _origin(origin),
      _direction(direction),
      _inverseDirection(1.0 / direction.x, 1.0 / direction.y,
                        1.0 / direction.z) {}

const Vector3D& Ray::getOrigin() const { return _origin; }

const Vector3D& Ray::getDirection() const { return _direction; }

const Vector3D& Ray::getInverseDirection() const { return _inverseDirection; }

Vector3D Ray::at(double t) const { return _origin + _direction * t; }

}  // namespace raytracer::math
