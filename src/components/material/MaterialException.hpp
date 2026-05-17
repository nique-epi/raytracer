/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MaterialException
*/

#ifndef MATERIAL_MATERIALEXCEPTION_HPP_
#define MATERIAL_MATERIALEXCEPTION_HPP_

#include "exceptions/Exceptions.hpp"

namespace raytracer::components::material {

/**
 * @brief Base exception for the material layer.
 *
 * Thrown when a material is constructed with parameters that violate
 * the @c IMaterial contract — typically a non-positive Phong
 * @c shininess reaching @c AMaterial::AMaterial, or any other
 * invariant violation a concrete material's constructor wants to
 * surface. Derives from the project-wide @c RaytracerException so all
 * domain failures share a single catch site at the application
 * boundary.
 */
class MaterialException : public raytracer::core::RaytracerException {
 public:
  using raytracer::core::RaytracerException::RaytracerException;
};

}  // namespace raytracer::components::material

#endif  // MATERIAL_MATERIALEXCEPTION_HPP_
