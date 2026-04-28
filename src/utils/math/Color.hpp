/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Color
*/

#ifndef MATH_COLOR_HPP_
#define MATH_COLOR_HPP_

#include <tuple>

namespace Math {

class Color {
 public:
  double r;
  double g;
  double b;

  Color();
  Color(double r_, double g_, double b_);

  Color(const Color&) = default;
  Color& operator=(const Color&) = default;
  Color(Color&&) = default;
  Color& operator=(Color&&) = default;
  ~Color() = default;

  [[nodiscard]] Color clamp() const;
  [[nodiscard]] Color gammaCorrect(double gamma) const;
  [[nodiscard]] std::tuple<int, int, int> toRGB() const;

  [[nodiscard]] Color operator+(const Color& other) const;
  [[nodiscard]] Color operator-(const Color& other) const;
  [[nodiscard]] Color operator*(const Color& other) const;
  [[nodiscard]] Color operator*(double scalar) const;
  [[nodiscard]] Color operator/(double scalar) const;

  Color& operator+=(const Color& other);
  Color& operator*=(double scalar);
};

[[nodiscard]] Color operator*(double scalar, const Color& c);

}  // namespace Math

#endif  // MATH_COLOR_HPP_
