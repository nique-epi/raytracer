/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PluginException
*/

#ifndef PLUGIN_PLUGINEXCEPTION_HPP_
#define PLUGIN_PLUGINEXCEPTION_HPP_

#include <string>
#include "exceptions/Exceptions.hpp"

namespace raytracer::plugin {

class PluginException : public raytracer::core::RaytracerException {
 public:
  using raytracer::core::RaytracerException::RaytracerException;
};

class PluginLoadException : public PluginException {
 public:
  PluginLoadException(const std::string& path, const std::string& reason);
};

class PluginSymbolException : public PluginException {
 public:
  PluginSymbolException(const std::string& path, const std::string& symbol);
};

}  // namespace raytracer::plugin

#endif  // PLUGIN_PLUGINEXCEPTION_HPP_
