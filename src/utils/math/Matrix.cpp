/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Matrix
*/

#include "Matrix.hpp"
#include <array>
#include <cmath>
#include <string_view>
#include <utility>

namespace {
constexpr double EPSILON = 1e-12;
}  // namespace

namespace raytracer::math {
Matrix4x4::Matrix4x4() : Matrix4x4(4, 4) {}

Matrix4x4::Matrix4x4(int rows, int cols) : _rows(rows), _cols(cols) {
  if (rows <= 0 || cols <= 0) {
    return;
  }
  _matrixData.resize(_rows, std::vector<double>(_cols, 0.0));
}

Matrix4x4::Matrix4x4(const Matrix4x4& other) = default;

Matrix4x4& Matrix4x4::operator=(const Matrix4x4& other) {
  if (this == &other) {
    return *this;
  }
  _rows = other._rows;
  _cols = other._cols;
  _matrixData = other._matrixData;
  return *this;
}

Matrix4x4::Matrix4x4(Matrix4x4&& other) noexcept
    : _rows(other._rows),
      _cols(other._cols),
      _matrixData(std::move(other._matrixData)) {
  other._rows = 0;
  other._cols = 0;
}

Matrix4x4& Matrix4x4::operator=(Matrix4x4&& other) noexcept {
  if (this == &other) {
    return *this;
  }
  _rows = other._rows;
  _cols = other._cols;
  _matrixData = std::move(other._matrixData);
  other._rows = 0;
  other._cols = 0;
  return *this;
}

Matrix4x4 Matrix4x4::operator+(const Matrix4x4& other) const {
  if (_rows != other._rows || _cols != other._cols) {
    // NOLINTNEXTLINE(modernize-return-braced-init-list)
    return Matrix4x4();
  }
  Matrix4x4 result(_rows, _cols);
  for (int i = 0; i < _rows; ++i) {
    for (int j = 0; j < _cols; ++j) {
      result._matrixData[i][j] = _matrixData[i][j] + other._matrixData[i][j];
    }
  }
  return result;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const {
  if (_cols != other._rows || _rows != 4 || other._cols != 4) {
    // NOLINTNEXTLINE(modernize-return-braced-init-list)
    return Matrix4x4();
  }
  Matrix4x4 result(4, 4);
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      double sum = 0.0;
      for (int k = 0; k < 4; ++k) {
        sum += _matrixData[i][k] * other._matrixData[k][j];
      }
      result._matrixData[i][j] = sum;
    }
  }
  return result;
}

bool Matrix4x4::operator==(const Matrix4x4& other) const {
  if (_rows != other._rows || _cols != other._cols) {
    return false;
  }
  for (int i = 0; i < _rows; ++i) {
    for (int j = 0; j < _cols; ++j) {
      if (std::abs(_matrixData[i][j] - other._matrixData[i][j]) > EPSILON) {
        return false;
      }
    }
  }
  return true;
}

Vector3D Matrix4x4::transformPoint(const Vector3D& point) const {
  if (_rows != 4 || _cols != 4) {
    // NOLINTNEXTLINE(modernize-return-braced-init-list)
    return Vector3D(0, 0, 0);
  }

  const double x = (_matrixData[0][0] * point.x) +
                   (_matrixData[0][1] * point.y) +
                   (_matrixData[0][2] * point.z) + _matrixData[0][3];
  const double y = (_matrixData[1][0] * point.x) +
                   (_matrixData[1][1] * point.y) +
                   (_matrixData[1][2] * point.z) + _matrixData[1][3];
  const double z = (_matrixData[2][0] * point.x) +
                   (_matrixData[2][1] * point.y) +
                   (_matrixData[2][2] * point.z) + _matrixData[2][3];
  const double w = (_matrixData[3][0] * point.x) +
                   (_matrixData[3][1] * point.y) +
                   (_matrixData[3][2] * point.z) + _matrixData[3][3];

  if (std::abs(w) > EPSILON && std::abs(w - 1.0) > EPSILON) {
    return {x / w, y / w, z / w};
  }
  return {x, y, z};
}

Vector3D Matrix4x4::transformDirection(const Vector3D& direction) const {
  if (_rows != 4 || _cols != 4) {
    // NOLINTNEXTLINE(modernize-return-braced-init-list)
    return Vector3D(0, 0, 0);
  }

  return {
      (_matrixData[0][0] * direction.x) + (_matrixData[0][1] * direction.y) +
          (_matrixData[0][2] * direction.z),
      (_matrixData[1][0] * direction.x) + (_matrixData[1][1] * direction.y) +
          (_matrixData[1][2] * direction.z),
      (_matrixData[2][0] * direction.x) + (_matrixData[2][1] * direction.y) +
          (_matrixData[2][2] * direction.z)};
}

Matrix4x4 Matrix4x4::inverse() const {
  if (_rows != 4 || _cols != 4) {
    // NOLINTNEXTLINE(modernize-return-braced-init-list)
    return Matrix4x4();
  }
  AugmentedMatrix augmented = makeAugmentedMatrix(*this);
  for (std::size_t pivot = 0; pivot < MATRIX_SIZE; ++pivot) {
    const std::size_t bestRow = findPivotRow(augmented, pivot);
    swapRows(augmented, pivot, bestRow);
    normalizePivotRow(augmented, pivot);
    eliminatePivotColumn(augmented, pivot);
  }
  return extractInverse(augmented);
}

Matrix4x4::AugmentedMatrix Matrix4x4::makeAugmentedMatrix(
    const Matrix4x4& matrix) {
  AugmentedMatrix augmented{};
  for (std::size_t row = 0; row < MATRIX_SIZE; ++row) {
    for (std::size_t col = 0; col < MATRIX_SIZE; ++col) {
      augmented[row][col] = matrix._matrixData[row][col];
      augmented[row][col + MATRIX_SIZE] = (row == col) ? 1.0 : 0.0;
    }
  }
  return augmented;
}

std::size_t Matrix4x4::findPivotRow(const AugmentedMatrix& matrix,
                                    std::size_t pivot) {
  std::size_t bestRow = pivot;
  double bestAbs = std::abs(matrix[pivot][pivot]);
  for (std::size_t row = pivot + 1; row < MATRIX_SIZE; ++row) {
    const double candidateAbs = std::abs(matrix[row][pivot]);
    if (candidateAbs > bestAbs) {
      bestAbs = candidateAbs;
      bestRow = row;
    }
  }
  if (bestAbs <= EPSILON) {
    return pivot;
  }
  return bestRow;
}

void Matrix4x4::swapRows(AugmentedMatrix& matrix, std::size_t lhs,
                         std::size_t rhs) {
  if (lhs == rhs) {
    return;
  }
  for (std::size_t col = 0; col < AUGMENTED_COLUMNS; ++col) {
    std::swap(matrix[lhs][col], matrix[rhs][col]);
  }
}

void Matrix4x4::normalizePivotRow(AugmentedMatrix& matrix, std::size_t pivot) {
  const double pivotValue = matrix[pivot][pivot];
  for (std::size_t col = 0; col < AUGMENTED_COLUMNS; ++col) {
    matrix[pivot][col] /= pivotValue;
  }
}

void Matrix4x4::eliminatePivotColumn(AugmentedMatrix& matrix,
                                     std::size_t pivot) {
  for (std::size_t row = 0; row < MATRIX_SIZE; ++row) {
    if (row == pivot) {
      continue;
    }
    const double factor = matrix[row][pivot];
    if (std::abs(factor) <= EPSILON) {
      continue;
    }
    for (std::size_t col = 0; col < AUGMENTED_COLUMNS; ++col) {
      matrix[row][col] -= factor * matrix[pivot][col];
    }
  }
}

Matrix4x4 Matrix4x4::extractInverse(const AugmentedMatrix& matrix) {
  Matrix4x4 result(4, 4);
  for (std::size_t row = 0; row < MATRIX_SIZE; ++row) {
    for (std::size_t col = 0; col < MATRIX_SIZE; ++col) {
      result._matrixData[row][col] = matrix[row][col + MATRIX_SIZE];
    }
  }
  return result;
}

Matrix4x4 Matrix4x4::identity() {
  Matrix4x4 result(4, 4);
  for (int i = 0; i < 4; ++i) {
    result._matrixData[i][i] = 1.0;
  }
  return result;
}

Matrix4x4 Matrix4x4::translation(const Vector3D& vector) {
  Matrix4x4 result = identity();
  result._matrixData[0][3] = vector.x;
  result._matrixData[1][3] = vector.y;
  result._matrixData[2][3] = vector.z;
  return result;
}

Matrix4x4 Matrix4x4::scaling(const Vector3D& vector) {
  Matrix4x4 result(4, 4);
  result._matrixData[0][0] = vector.x;
  result._matrixData[1][1] = vector.y;
  result._matrixData[2][2] = vector.z;
  result._matrixData[3][3] = 1.0;
  return result;
}

Matrix4x4 Matrix4x4::rotation(double angle, const Vector3D& axis) {
  const Vector3D normalizedAxis = axis.normalize();
  const double x = normalizedAxis.x;
  const double y = normalizedAxis.y;
  const double z = normalizedAxis.z;
  const auto radians = (angle);
  const double c = std::cos(radians);
  const double s = std::sin(radians);
  const double t = 1.0 - c;

  Matrix4x4 result = identity();
  result._matrixData[0][0] = (t * x * x) + c;
  result._matrixData[0][1] = (t * x * y) - (s * z);
  result._matrixData[0][2] = (t * x * z) + (s * y);

  result._matrixData[1][0] = (t * x * y) + (s * z);
  result._matrixData[1][1] = (t * y * y) + c;
  result._matrixData[1][2] = (t * y * z) - (s * x);

  result._matrixData[2][0] = (t * x * z) - (s * y);
  result._matrixData[2][1] = (t * y * z) + (s * x);
  result._matrixData[2][2] = (t * z * z) + c;

  return result;
}
}  // namespace raytracer::math
