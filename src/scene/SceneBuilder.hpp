/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SceneBuilder
*/

#pragma once
#include <libconfig.h++>
#include <string>
#include <vector>

namespace raytracer::scene {

class SceneBuilder {
 public:
  struct Entry {
    std::string type;
  };

  void addObject(const std::string& type, const libconfig::Setting& cfg);

  [[nodiscard]] const std::vector<Entry>& entries() const;
  [[nodiscard]] std::size_t count(const std::string& type) const;

 private:
  std::vector<Entry> _entries;
};

}  // namespace raytracer::scene
