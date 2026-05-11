/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** GLTFUtils
*/

#ifndef GLTFUTILS_HPP_
#define GLTFUTILS_HPP_

#include <string>
#include <vector>
#include "../types/GLTFTypes.hpp"
#include "utils/math/Matrix.hpp"
#include "utils/math/Vector3D.hpp"

namespace raytracer::scene::gltf::utils {

using ::raytracer::math::Matrix4x4;
using ::raytracer::math::Vector3D;
using raytracer::scene::gltf::AccessorData;
using raytracer::scene::gltf::Quaternion;

// ============================================================================
// Accessor utilities (float*, matrix TRS)
// ============================================================================

/**
 * @brief Converts accessor data to float vector
 */
std::vector<float> accessorToFloatVector(const AccessorData& accessor);

/**
 * @brief Converts accessor data to uint32_t indices
 */
std::vector<uint32_t> accessorToIndices(const AccessorData& accessor);

/**
 * @brief Extracts TRS (Translation, Rotation, Scale) from transform matrix
 */
void decomposeTransform(const Matrix4x4& matrix, Vector3D& translation,
                        Quaternion& rotation, Vector3D& scale);

/**
 * @brief Composes TRS into transformation matrix
 */
Matrix4x4 composeTransform(const Vector3D& translation,
                           const Quaternion& rotation, const Vector3D& scale);

// ============================================================================
// Texture path resolution
// ============================================================================

/**
 * @brief Resolves relative texture paths from GLTF base directory
 */
std::string resolveTexturePath(const std::string& gltfDir,
                               const std::string& texturePath);

/**
 * @brief Extracts directory from file path
 */
std::string getDirectory(const std::string& filePath);

}  // namespace raytracer::scene::gltf::utils

#endif  // GLTFUTILS_HPP_
