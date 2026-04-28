/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Exceptions
*/

#include "Exceptions.hpp"
#include <utility>

namespace Core {

RaytracerException::RaytracerException(std::string msg) : _msg(std::move(msg)) {}

const char* RaytracerException::what() const noexcept { return _msg.c_str(); }

}  // namespace Core
