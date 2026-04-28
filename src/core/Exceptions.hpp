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

namespace Core {

class RaytracerException : public std::exception {
 public:
  explicit RaytracerException(std::string msg);
  [[nodiscard]] const char* what() const noexcept override;

 private:
  std::string _msg;
};

}  // namespace Core

#endif  // CORE_EXCEPTIONS_HPP_
