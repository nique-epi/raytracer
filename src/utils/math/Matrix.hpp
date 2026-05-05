/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Matrix
*/

#pragma once

#include <array>
#include <vector>
#include "utils/math/Vector3D.hpp"
namespace raytracer::math {
class Matrix4x4 {
 public:
  Matrix4x4();
  Matrix4x4(int rows, int cols);
  ~Matrix4x4() = default;

  Matrix4x4(const Matrix4x4& other);
  Matrix4x4& operator=(const Matrix4x4& other);
  Matrix4x4(Matrix4x4&& other) noexcept;
  Matrix4x4& operator=(Matrix4x4&& other) = delete;

  Matrix4x4 operator+(const Matrix4x4& other) const;
  [[nodiscard]] Vector3D transformPoint(const Vector3D& point) const;
  [[nodiscard]] Vector3D transformDirection(const Vector3D& direction) const;
  [[nodiscard]] Matrix4x4 inverse() const;

  static Matrix4x4 identity();
  static Matrix4x4 translation(const Vector3D& vector);
  static Matrix4x4 scaling(const Vector3D& vector);
  static Matrix4x4 rotation(float angle, const Vector3D& axis);

 protected:
 private:
  static constexpr std::size_t MATRIX_SIZE = 4;
  static constexpr std::size_t AUGMENTED_COLUMNS = 8;
  using AugmentedMatrix =
      std::array<std::array<double, AUGMENTED_COLUMNS>, MATRIX_SIZE>;

  static AugmentedMatrix makeAugmentedMatrix(const Matrix4x4& matrix);
  static std::size_t findPivotRow(const AugmentedMatrix& matrix,
                                  std::size_t pivot);
  static void swapRows(AugmentedMatrix& matrix, std::size_t lhs,
                       std::size_t rhs);
  static void normalizePivotRow(AugmentedMatrix& matrix, std::size_t pivot);
  static void eliminatePivotColumn(AugmentedMatrix& matrix, std::size_t pivot);
  static Matrix4x4 extractInverse(const AugmentedMatrix& matrix);

  int _rows;
  int _cols;
  std::vector<std::vector<double>> _matrixData;
};
}  // namespace raytracer::math
