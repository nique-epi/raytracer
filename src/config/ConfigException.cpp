/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ConfigException
*/

#include "ConfigException.hpp"
#include <string>
#include "constants/Errors.hpp"

namespace raytracer::config {

ConfigException::ConfigException(const std::string& field)
    : raytracer::core::RaytracerException(
          std::string(raytracer::constants::errors::CONFIG_FIELD_MISSING) +
          ": '" + field + "'") {}

ConfigException::ConfigException(const std::string& field,
                                 const std::string& reason)
    : raytracer::core::RaytracerException(
          std::string(raytracer::constants::errors::CONFIG_TYPE_INVALID) +
          " for field '" + field + "': " + reason) {}

}  // namespace raytracer::config
