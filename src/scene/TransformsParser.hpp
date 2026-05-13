/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** TransformsParser
*/

#pragma once

#include <libconfig.h++>
#include <memory>
#include "components/Primitives/IObject.hpp"
#include "factory/IComponentFactory.hpp"

namespace raytracer::scene {

/**
 * @brief Wrap @p primitive into a `TransformedObject` driven by @p transformsCfg.
 *
 * Iterates @p transformsCfg in file order. Each entry must be a libconfig
 * group exposing a `type` string field; the remaining fields are forwarded
 * as-is to `IComponentFactory::createTransformation`. The resulting
 * transformations are stacked on a fresh `TransformedObject` in the same
 * order — first inserted is the first transformation applied to the local
 * point, matching the top-down semantics of the configuration file.
 *
 * @param [in] primitive     The primitive to decorate. Must not be null.
 * @param [in] transformsCfg The libconfig list of transformation entries.
 * @param [in] factory       Factory used to resolve each transformation type.
 *
 * @returns A `TransformedObject` wrapping @p primitive with every parsed
 *          transformation. If @p transformsCfg is empty, the returned wrapper
 *          carries the primitive with no transformation attached.
 *
 * @throws SceneBuildException If an entry's `type` field is absent or not a
 *         string. The thrown message reports the failing entry's index.
 * @throws raytracer::core::RaytracerException If a transformation type is
 *         unknown (propagated from the factory).
 */
[[nodiscard]] std::shared_ptr<IObject> wrapWithTransforms(
    const std::shared_ptr<IObject>& primitive,
    const libconfig::Setting& transformsCfg,
    raytracer::core::factory::IComponentFactory& factory);

}  // namespace raytracer::scene
