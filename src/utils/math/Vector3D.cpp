/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Vector3D
*/

#include "Vector3D.hpp"
#include <cmath>
#include <random>
#include "constants/Errors.hpp"
#include "exceptions/Exceptions.hpp"

namespace raytracer::math {

Vector3D::Vector3D() : x(0.0), y(0.0), z(0.0) {}

Vector3D::Vector3D(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

double Vector3D::dot(const Vector3D& other) const {
  return (x * other.x) + (y * other.y) + (z * other.z);
}

Vector3D Vector3D::cross(const Vector3D& other) const {
  return {(y * other.z) - (z * other.y), (z * other.x) - (x * other.z),
          (x * other.y) - (y * other.x)};
}

double Vector3D::lengthSquared() const { return (x * x) + (y * y) + (z * z); }

double Vector3D::length() const { return std::sqrt(lengthSquared()); }

Vector3D Vector3D::normalize() const {
  const double len = length();
  if (len == 0.0) {
    throw raytracer::core::RaytracerException(
        raytracer::constants::errors::MATH_VECTOR_ZERO_NORMALIZE);
  }
  return {x / len, y / len, z / len};
}

Vector3D Vector3D::operator+(const Vector3D& other) const {
  return {x + other.x, y + other.y, z + other.z};
}

Vector3D Vector3D::operator-(const Vector3D& other) const {
  return {x - other.x, y - other.y, z - other.z};
}

Vector3D Vector3D::operator-() const { return {-x, -y, -z}; }

Vector3D Vector3D::operator*(double scalar) const {
  return {x * scalar, y * scalar, z * scalar};
}

Vector3D Vector3D::operator*(const Vector3D& other) const {
  return {x * other.x, y * other.y, z * other.z};
}

Vector3D Vector3D::operator/(double scalar) const {
  return {x / scalar, y / scalar, z / scalar};
}

Vector3D& Vector3D::operator+=(const Vector3D& other) {
  x += other.x;
  y += other.y;
  z += other.z;
  return *this;
}

Vector3D& Vector3D::operator-=(const Vector3D& other) {
  x -= other.x;
  y -= other.y;
  z -= other.z;
  return *this;
}

Vector3D& Vector3D::operator*=(double scalar) {
  x *= scalar;
  y *= scalar;
  z *= scalar;
  return *this;
}

Vector3D& Vector3D::operator/=(double scalar) {
  x /= scalar;
  y /= scalar;
  z /= scalar;
  return *this;
}

Vector3D operator*(double scalar, const Vector3D& v) { return v * scalar; }

Vector3D Vector3D::randomInUnitSphere() {
  thread_local std::mt19937 gen(std::random_device{}());
  thread_local std::uniform_real_distribution<double> dist(-1.0, 1.0);

  while (true) {
    Vector3D p(dist(gen), dist(gen), dist(gen));
    if (p.lengthSquared() < 1.0) {
      return p;
    }
  }
}

}  // namespace raytracer::math
