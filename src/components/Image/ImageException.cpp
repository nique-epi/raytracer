/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ImageException
*/

#include "ImageException.hpp"
#include <string>
#include "constants/Errors.hpp"

namespace Components {

ImageIOException::ImageIOException(const std::string& path,
                                   const std::string& reason)
    : ImageException(std::string(Constants::Errors::IMAGE_IO_ERROR) + " on '" +
                     path + "': " + reason) {}

}  // namespace Components
