/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** GLTFUtils
*/

#include "GLTFUtils.hpp"
#include <filesystem>

namespace {

constexpr double HALF = 0.5;
constexpr double QUARTER = 0.25;
constexpr double TWO = 2.0;

raytracer::math::Vector3D extractTranslation(
    const raytracer::math::Matrix4x4& m) {
  return {m.get(0, 3), m.get(1, 3), m.get(2, 3)};
}

raytracer::math::Vector3D extractScale(const raytracer::math::Matrix4x4& m) {
  double sx =
      raytracer::math::Vector3D(m.get(0, 0), m.get(1, 0), m.get(2, 0)).length();
  double sy =
      raytracer::math::Vector3D(m.get(0, 1), m.get(1, 1), m.get(2, 1)).length();
  double sz =
      raytracer::math::Vector3D(m.get(0, 2), m.get(1, 2), m.get(2, 2)).length();
  return {sx, sy, sz};
}

raytracer::scene::gltf::Quaternion extractRotation(
    const raytracer::math::Matrix4x4& m,
    const raytracer::math::Vector3D& scale) {
  double r00 = m.get(0, 0) / scale.x;
  double r10 = m.get(1, 0) / scale.x;
  double r20 = m.get(2, 0) / scale.x;
  double r01 = m.get(0, 1) / scale.y;
  double r11 = m.get(1, 1) / scale.y;
  double r21 = m.get(2, 1) / scale.y;
  double r02 = m.get(0, 2) / scale.z;
  double r12 = m.get(1, 2) / scale.z;
  double r22 = m.get(2, 2) / scale.z;
  double trace = r00 + r11 + r22;
  double qw = 0.0;
  double qx = 0.0;
  double qy = 0.0;
  double qz = 0.0;
  if (trace > 0.0) {
    double s = HALF / std::sqrt(trace + 1.0);
    qw = QUARTER / s;
    qx = (r21 - r12) * s;
    qy = (r02 - r20) * s;
    qz = (r10 - r01) * s;
  } else if (r00 > r11 && r00 > r22) {
    double s = TWO * std::sqrt(1.0 + r00 - r11 - r22);
    qw = (r21 - r12) / s;
    qx = QUARTER * s;
    qy = (r01 + r10) / s;
    qz = (r02 + r20) / s;
  } else if (r11 > r22) {
    double s = TWO * std::sqrt(1.0 + r11 - r00 - r22);
    qw = (r02 - r20) / s;
    qx = (r01 + r10) / s;
    qy = QUARTER * s;
    qz = (r12 + r21) / s;
  } else {
    double s = TWO * std::sqrt(1.0 + r22 - r00 - r11);
    qw = (r10 - r01) / s;
    qx = (r02 + r20) / s;
    qy = (r12 + r21) / s;
    qz = QUARTER * s;
  }
  return {qx, qy, qz, qw};
}

raytracer::math::Matrix4x4 MatrixRotation(
    const raytracer::scene::gltf::Quaternion& q) {
  double x = q.x;
  double y = q.y;
  double z = q.z;
  double w = q.w;
  raytracer::math::Matrix4x4 m;
  m.set(0, 0, 1 - 2 * (y * y + z * z));
  m.set(0, 1, 2 * (x * y - z * w));
  m.set(0, 2, 2 * (x * z + y * w));
  m.set(0, 3, 0);
  m.set(1, 0, 2 * (x * y + z * w));
  m.set(1, 1, 1 - 2 * (x * x + z * z));
  m.set(1, 2, 2 * (y * z - x * w));
  m.set(1, 3, 0);
  m.set(2, 0, 2 * (x * z - y * w));
  m.set(2, 1, 2 * (y * z + x * w));
  m.set(2, 2, 1 - 2 * (x * x + y * y));
  m.set(2, 3, 0);
  m.set(3, 0, 0);
  m.set(3, 1, 0);
  m.set(3, 2, 0);
  m.set(3, 3, 1);
  return m;
}
}  // namespace

namespace raytracer::scene::gltf::utils {

std::vector<float> accessorToFloatVector(const AccessorData& accessor) {
  return accessor.floatData;
}

std::vector<uint32_t> accessorToIndices(const AccessorData& accessor) {
  return accessor.indexData;
}

void decomposeTransform(const Matrix4x4& matrix, Vector3D& translation,
                        Quaternion& rotation, Vector3D& scale) {
  translation = extractTranslation(matrix);
  scale = extractScale(matrix);

  if (scale.x == 0.0 || scale.y == 0.0 || scale.z == 0.0) {
    rotation = Quaternion(0.0, 0.0, 0.0, 1.0);
    return;
  }

  rotation = extractRotation(matrix, scale);
}

Matrix4x4 composeTransform(const Vector3D& translation,
                           const Quaternion& rotation, const Vector3D& scale) {
  return Matrix4x4::translation(translation) * MatrixRotation(rotation) *
         Matrix4x4::scaling(scale);
}

std::string resolveTexturePath(const std::string& gltfDir,
                               const std::string& texturePath) {
  std::filesystem::path basePath(gltfDir);
  std::filesystem::path result = basePath / texturePath;
  return result.string();
}

std::string getDirectory(const std::string& filePath) {
  std::filesystem::path path(filePath);
  return path.parent_path().string();
}

}  // namespace raytracer::scene::gltf::utils
