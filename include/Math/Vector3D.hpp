/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Vector3D
*/

#ifndef MATH_VECTOR3D_HPP_
#define MATH_VECTOR3D_HPP_

namespace Math {

struct Vector3D {
  double x;
  double y;
  double z;

  Vector3D();
  Vector3D(double x_, double y_, double z_);

  Vector3D(const Vector3D&) = default;
  Vector3D& operator=(const Vector3D&) = default;
  Vector3D(Vector3D&&) = default;
  Vector3D& operator=(Vector3D&&) = default;
  ~Vector3D() = default;

  [[nodiscard]] double dot(const Vector3D& other) const;
  [[nodiscard]] Vector3D cross(const Vector3D& other) const;
  [[nodiscard]] double lengthSquared() const;
  [[nodiscard]] double length() const;
  [[nodiscard]] Vector3D normalize() const;

  [[nodiscard]] Vector3D operator+(const Vector3D& other) const;
  [[nodiscard]] Vector3D operator-(const Vector3D& other) const;
  [[nodiscard]] Vector3D operator-() const;
  [[nodiscard]] Vector3D operator*(double scalar) const;
  [[nodiscard]] Vector3D operator*(const Vector3D& other) const;
  [[nodiscard]] Vector3D operator/(double scalar) const;

  Vector3D& operator+=(const Vector3D& other);
  Vector3D& operator-=(const Vector3D& other);
  Vector3D& operator*=(double scalar);
  Vector3D& operator/=(double scalar);
};

[[nodiscard]] Vector3D operator*(double scalar, const Vector3D& v);

using Point3D = Vector3D;

}  // namespace Math

#endif  // MATH_VECTOR3D_HPP_
