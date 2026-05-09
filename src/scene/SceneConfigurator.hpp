/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SceneConfigurator
*/

#pragma once

#include <libconfig.h++>
#include <memory>
#include <string>

class IObject;
class ILight;
class ICamera;

namespace raytracer::scene {

/**
 * @brief Apply a libconfig block to the matching concrete object type.
 *
 * @param [in]     type Registered primitive type key (e.g. "sphere").
 * @param [in,out] obj  Object created by the registry — setters are called on it.
 * @param [in]     cfg  Config block for this primitive entry.
 */
void configureObject(const std::string& type, std::shared_ptr<IObject> obj,
                     const libconfig::Setting& cfg);

/**
 * @brief Apply a libconfig block to the matching concrete light type.
 *
 * @param [in]     type  Registered light type key (e.g. "ambient").
 * @param [in,out] light Light created by the registry — setters are called on it.
 * @param [in]     cfg   Config block for this light entry.
 */
void configureLight(const std::string& type, std::shared_ptr<ILight> light,
                    const libconfig::Setting& cfg);

/**
 * @brief Apply a libconfig block to a perspective camera.
 *
 * @param [in,out] camera Camera created by the registry — setters are called on it.
 * @param [in]     cfg    Config block for the camera entry.
 */
void configurePerspectiveCamera(std::shared_ptr<ICamera> camera,
                                const libconfig::Setting& cfg);

}  // namespace raytracer::scene
