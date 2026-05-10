/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ConfigException
*/

#ifndef CONFIG_CONFIGEXCEPTION_HPP_
#define CONFIG_CONFIGEXCEPTION_HPP_

#include <string>
#include "exceptions/Exceptions.hpp"

namespace raytracer::config {

class ConfigException : public raytracer::core::RaytracerException {
 public:
  explicit ConfigException(const std::string& field);
  ConfigException(const std::string& field, const std::string& reason);
};

}  // namespace raytracer::config

#endif  // CONFIG_CONFIGEXCEPTION_HPP_
