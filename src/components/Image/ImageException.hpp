/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ImageException
*/

#ifndef COMPONENTS_IMAGE_IMAGEEXCEPTION_HPP_
#define COMPONENTS_IMAGE_IMAGEEXCEPTION_HPP_

#include <string>
#include "core/Exceptions.hpp"

namespace Components {

class ImageException : public Core::RaytracerException {
 public:
  using Core::RaytracerException::RaytracerException;
};

class ImageIOException : public ImageException {
 public:
  ImageIOException(const std::string& path, const std::string& reason);
};

}  // namespace Components

#endif  // COMPONENTS_IMAGE_IMAGEEXCEPTION_HPP_
