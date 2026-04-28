/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PluginException
*/

#include "PluginException.hpp"
#include <string>
#include "constants/Errors.hpp"

namespace raytracer::plugin {

PluginLoadException::PluginLoadException(const std::string& path,
                                         const std::string& reason)
    : PluginException(std::string(Constants::Errors::PLUGIN_LOAD_FAILED) +
                      " '" + path + "': " + reason) {}

PluginSymbolException::PluginSymbolException(const std::string& path,
                                             const std::string& symbol)
    : PluginException(std::string(Constants::Errors::PLUGIN_SYMBOL_NOT_FOUND) +
                      " '" + symbol + "' in plugin '" + path + "'") {}

}  // namespace raytracer::plugin
