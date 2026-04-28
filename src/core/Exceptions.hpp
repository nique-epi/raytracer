/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Exceptions
*/

#ifndef CORE_EXCEPTIONS_HPP_
#define CORE_EXCEPTIONS_HPP_

#include <exception>
#include <string>

namespace raytracer::core {

class RaytracerException : public std::exception {
 public:
  explicit RaytracerException(std::string msg);
  [[nodiscard]] const char* what() const noexcept override;

 private:
  std::string _msg;
};

}  // namespace raytracer::core

#endif  // CORE_EXCEPTIONS_HPP_
