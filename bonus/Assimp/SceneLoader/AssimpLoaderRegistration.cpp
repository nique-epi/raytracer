/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** AssimpLoaderRegistration
*/

#include "AssimpLoaderRegistration.hpp"
#include <memory>
#include "AssimpLoader.hpp"
#include "scene/SceneLoaderFactory.hpp"

namespace raytracer::bonus {

void registerAssimpLoader(raytracer::scene::SceneLoaderFactory& factory) {
  factory.registerLoader(std::make_shared<raytracer::scene::AssimpLoader>());
}

}  // namespace raytracer::bonus
