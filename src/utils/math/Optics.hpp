/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Optics
*/

#ifndef MATH_OPTICS_HPP_
#define MATH_OPTICS_HPP_

#include "Vector3D.hpp"

namespace raytracer::math::Optics {

[[nodiscard]] Vector3D reflect(const Vector3D& v, const Vector3D& n);
[[nodiscard]] Vector3D refract(const Vector3D& v, const Vector3D& n,
                               double ni_nt);
[[nodiscard]] double schlick(double cosine, double refIdx);

}  // namespace raytracer::math::Optics

#endif  // MATH_OPTICS_HPP_
