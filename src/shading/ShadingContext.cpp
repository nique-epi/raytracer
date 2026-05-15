/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ShadingContext
*/

#include "shading/ShadingContext.hpp"

#include <utility>

#include "shading/IShadingMode.hpp"
#include "shading/ShadingException.hpp"

namespace raytracer::shading {

ShadingContext::ShadingContext(std::shared_ptr<IShadingMode> initial)
    : strategy_(std::move(initial)) {
  if (!strategy_) {
    throw ShadingException(
        "ShadingContext requires a non-null initial strategy");
  }
}

void ShadingContext::setStrategy(std::shared_ptr<IShadingMode> strategy) {
  if (!strategy) {
    throw ShadingException(
        "ShadingContext::setStrategy requires a non-null strategy");
  }
  const std::lock_guard<std::mutex> lock(mutex_);
  strategy_ = std::move(strategy);
}

std::shared_ptr<IShadingMode> ShadingContext::currentStrategy() const {
  const std::lock_guard<std::mutex> lock(mutex_);
  return strategy_;
}

}  // namespace raytracer::shading
