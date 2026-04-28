/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Ray
*/

#include "Ray.hpp"

namespace Math {

Ray::Ray(const Vector3D& origin, const Vector3D& direction)
    : _origin(origin), _direction(direction) {}

const Vector3D& Ray::getOrigin() const { return _origin; }

const Vector3D& Ray::getDirection() const { return _direction; }

Vector3D Ray::at(double t) const { return _origin + _direction * t; }

}  // namespace Math
