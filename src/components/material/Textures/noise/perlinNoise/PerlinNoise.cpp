/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PerlinNoise
*/

#include "PerlinNoise.hpp"
#include <array>
#include <random>

namespace raytracer::materials::textures::utils {
PerlinNoise::PerlinNoise()
    : permX_(generatePermutation()),
      permY_(generatePermutation()),
      permZ_(generatePermutation()) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> dis(-1.0, 1.0);

  for (int i = 0; i < kPointCount; ++i) {
    double x = dis(gen);
    double y = dis(gen);
    double z = dis(gen);
    gradients_[i] = math::Vector3D(x, y, z).normalize();
  }
}

std::array<int, PerlinNoise::kPointCount> PerlinNoise::generatePermutation() {
  std::array<int, kPointCount> perm{};
  for (int i = 0; i < kPointCount; ++i) {
    perm[i] = i;
  }
  std::random_device rd;
  std::mt19937 gen(rd());
  std::shuffle(perm.begin(), perm.end(), gen);
  return perm;
}

double PerlinNoise::noise(const math::Vector3D& point) const {
  double u = point.x - std::floor(point.x);
  double v = point.y - std::floor(point.y);
  double w = point.z - std::floor(point.z);
  double uu = u * u * (3 - 2 * u);
  double vv = v * v * (3 - 2 * v);
  double ww = w * w * (3 - 2 * w);
  unsigned i = static_cast<unsigned>(std::floor(point.x)) & (kPointCount - 1U);
  unsigned j = static_cast<unsigned>(std::floor(point.y)) & (kPointCount - 1U);
  unsigned k = static_cast<unsigned>(std::floor(point.z)) & (kPointCount - 1U);

  std::array<std::array<std::array<math::Vector3D, 2>, 2>, 2> c;
  for (int di = 0; di < 2; di++) {
    for (int dj = 0; dj < 2; dj++) {
      for (int dk = 0; dk < 2; dk++) {
        auto idxX =
            static_cast<unsigned>(permX_[(i + di) & (kPointCount - 1U)]);
        auto idxY =
            static_cast<unsigned>(permY_[(j + dj) & (kPointCount - 1U)]);
        auto idxZ =
            static_cast<unsigned>(permZ_[(k + dk) & (kPointCount - 1U)]);
        auto combined = idxX ^ idxY ^ idxZ;
        c[di][dj][dk] = gradients_[combined];
      }
    }
  }
  double accum = 0.0;
  for (int di = 0; di < 2; di++) {
    for (int dj = 0; dj < 2; dj++) {
      for (int dk = 0; dk < 2; dk++) {
        math::Vector3D weight(u - di, v - dj, w - dk);
        accum += (di * uu + (1 - di) * (1 - uu)) *
                 (dj * vv + (1 - dj) * (1 - vv)) *
                 (dk * ww + (1 - dk) * (1 - ww)) * c[di][dj][dk].dot(weight);
      }
    }
  }

  return accum;
}

}  // namespace raytracer::materials::textures::utils
