/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Hemisphere sampling helpers (cosine-weighted)
*/

/**
 * @file HemisphereSampler.hpp
 * @brief Cosine-weighted hemisphere sampling around an arbitrary normal,
 *        backed by a thread-local Mersenne Twister so multi-threaded
 *        renderers do not contend on a shared RNG.
 */

#ifndef RENDERING_HELPER_HEMISPHERESAMPLER_HPP_
#define RENDERING_HELPER_HEMISPHERESAMPLER_HPP_

#include <cstdint>

#include "utils/math/Vector3D.hpp"

namespace raytracer::rendering::helper {

/**
 * @brief Draw a unit vector cosine-weighted around @p normal.
 *
 * The returned direction is concentrated near the normal (where cos(theta)
 * is high). This matches the integrand for Lambertian-style ambient
 * occlusion: contributions weighted by cos(theta) are more important, so
 * sampling them more often reduces variance.
 *
 * The RNG is a thread-local @c std::mt19937 — each worker thread owns its
 * own state and there is no synchronisation on the sampler. Determinism
 * is not provided: the seed is set from @c std::random_device the first
 * time the sampler runs in a given thread.
 *
 * @param [in] normal Unit surface normal at the shaded point.
 * @returns Unit-length sample direction in the upper hemisphere around
 *          @p normal (always satisfies @c result.dot(normal) >= 0).
 */
math::Vector3D sampleCosineHemisphere(const math::Vector3D& normal);

/**
 * @brief Re-seed the calling thread's hemisphere sampler RNG.
 *
 * Provided for deterministic unit tests: by seeding the sampler with a
 * known value before generating samples, tests can compare exact AO
 * results without relying on @c std::random_device output.
 *
 * @param [in] seed Seed value forwarded to @c std::mt19937::seed().
 */
void seedHemisphereSampler(std::uint64_t seed);

}  // namespace raytracer::rendering::helper

#endif  // RENDERING_HELPER_HEMISPHERESAMPLER_HPP_
