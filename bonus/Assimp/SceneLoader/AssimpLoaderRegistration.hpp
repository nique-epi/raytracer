/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** AssimpLoaderRegistration
*/

#pragma once

namespace raytracer::scene {
class SceneLoaderFactory;
}  // namespace raytracer::scene

namespace raytracer::bonus {

void registerAssimpLoader(raytracer::scene::SceneLoaderFactory& factory);

}  // namespace raytracer::bonus
