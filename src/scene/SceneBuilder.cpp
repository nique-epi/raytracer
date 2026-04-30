/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SceneBuilder
*/

#include "SceneBuilder.hpp"
#include <algorithm>

namespace raytracer::scene {

void SceneBuilder::addObject(const std::string& type,
                             const libconfig::Setting& cfg) {
  (void)cfg;
  _entries.push_back({type});
}

const std::vector<SceneBuilder::Entry>& SceneBuilder::entries() const {
  return _entries;
}

std::size_t SceneBuilder::count(const std::string& type) const {
  return static_cast<std::size_t>(
      // NOLINTNEXTLINE(boost-use-ranges)
      std::count_if(_entries.begin(), _entries.end(),
                    [&](const Entry& e) { return e.type == type; }));
}

}  // namespace raytracer::scene
