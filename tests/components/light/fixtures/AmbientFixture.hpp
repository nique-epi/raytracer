/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** AmbientFixture
*/

#pragma once
#include "LightFixture.hpp"

class AmbientFixture : public LightFixture {
 protected:
  void SetUp() override { LightFixture::SetUp(); }
  void TearDown() override { LightFixture::TearDown(); }
  std::string pluginFileName() const override {
    return "raytracer_ambient_light.so";
  }
};
