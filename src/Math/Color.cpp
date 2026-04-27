/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Color
*/

#include "Math/Color.hpp"
#include <algorithm>
#include <cmath>

namespace {
constexpr double kRgbScale = 255.0;
}  // namespace

namespace Math {

Color::Color() : r(0.0), g(0.0), b(0.0) {}

Color::Color(double r_, double g_, double b_) : r(r_), g(g_), b(b_) {}

Color Color::clamp() const {
  return {std::clamp(r, 0.0, 1.0), std::clamp(g, 0.0, 1.0),
          std::clamp(b, 0.0, 1.0)};
}

Color Color::gammaCorrect(double gamma) const {
  if (gamma <= 0.0)
    throw std::domain_error("gamma must be > 0");
  double inv = 1.0 / gamma;
  return {std::pow(r, inv), std::pow(g, inv), std::pow(b, inv)};
}

std::tuple<int, int, int> Color::toRGB() const {
  Color c = clamp();
  return {static_cast<int>(c.r * kRgbScale), static_cast<int>(c.g * kRgbScale),
          static_cast<int>(c.b * kRgbScale)};
}

Color Color::operator+(const Color& other) const {
  return {r + other.r, g + other.g, b + other.b};
}

Color Color::operator-(const Color& other) const {
  return {r - other.r, g - other.g, b - other.b};
}

Color Color::operator*(const Color& other) const {
  return {r * other.r, g * other.g, b * other.b};
}

Color Color::operator*(double scalar) const {
  return {r * scalar, g * scalar, b * scalar};
}

Color Color::operator/(double scalar) const {
  return {r / scalar, g / scalar, b / scalar};
}

Color& Color::operator+=(const Color& other) {
  r += other.r;
  g += other.g;
  b += other.b;
  return *this;
}

Color& Color::operator*=(double scalar) {
  r *= scalar;
  g *= scalar;
  b *= scalar;
  return *this;
}

Color operator*(double scalar, const Color& c) { return c * scalar; }

}  // namespace Math
