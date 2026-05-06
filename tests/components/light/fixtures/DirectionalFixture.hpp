/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** DirectionalFixture
*/

#pragma once

#include "LightFixture.hpp"

class DirectionalLightFixture : public LightFixture {
 protected:
  void SetUp() override { LightFixture::SetUp(); }
  void TearDown() override { LightFixture::TearDown(); }
  std::string pluginFileName() const override {
    return "raytracer_directional_light.so";
  }
};
