/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ImageException
*/

#include "ImageException.hpp"
#include <string>
#include "constants/Errors.hpp"

namespace raytracer::components {

ImageIOException::ImageIOException(const std::string& path,
                                   const std::string& reason)
    : ImageException(std::string(raytracer::constants::errors::IMAGE_IO_ERROR) + " on '" +
                     path + "': " + reason) {}

}  // namespace raytracer::components
