/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Helpers shared between every integrator and every viewport shader
*/

/**
 * @file ShadingHelpers.hpp
 * @brief Small kernel of helpers used by every concrete `IIntegrator`
 *        and every concrete `IShadingMode`.
 *
 * Lives outside both `integrator/` and `shading/` so it can be linked
 * by both without creating a cycle. Depends only on `raytracer_math`
 * for the geometric types it operates on; no knowledge of the
 * integrator or shading interfaces themselves.
 */

#ifndef SHADING_COMMON_SHADINGHELPERS_HPP_
#define SHADING_COMMON_SHADINGHELPERS_HPP_

#include "utils/math/Color.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Vector3D.hpp"

namespace raytracer::shading::common {

/**
 * @brief Minimum @c t value used when intersecting primary or secondary
 *        rays against the scene, to avoid self-intersection caused by
 *        floating-point imprecision at the ray origin.
 */
inline constexpr double primaryRayTMin = 1e-3;

/**
 * @brief Return @p record.normal scaled to unit length.
 *
 * Defensive against a degenerate (zero-length) normal: in that case
 * the raw normal is returned as-is rather than throwing, so the
 * shading pipeline can still produce a sensible debug value instead
 * of crashing on a malformed hit.
 *
 * @param[in] record Hit record produced by the scene intersection.
 * @returns A unit-length copy of @p record.normal, or the raw normal
 *          if its squared length is non-positive.
 */
math::Vector3D unitShadingNormal(const math::HitRecord& record);

/**
 * @brief Map a unit normal to an RGB colour for debug visualisation.
 *
 * Each component is remapped from `[-1, 1]` to `[0, 1]` using
 * `c = n * 0.5 + 0.5`, the standard "normals-as-colour" convention.
 *
 * @param[in] normal Unit-length normal to visualise.
 * @returns Linear-space RGB colour with components in `[0, 1]`.
 */
math::Color normalAsColor(const math::Vector3D& normal);

}  // namespace raytracer::shading::common

#endif  // SHADING_COMMON_SHADINGHELPERS_HPP_
