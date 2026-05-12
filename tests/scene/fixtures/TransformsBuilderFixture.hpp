/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** TransformsBuilderFixture
*/

#pragma once

#include <gtest/gtest.h>
#include "../../fixtures/ComponentFactoryFixture.hpp"

/**
 * @brief End-to-end fixture for `SceneBuilder` tests that exercise transforms.
 *
 * Pre-registers, on the shared `ComponentFactoryFixture`:
 *  - primitive `"sphere"`     → real `SphereFixture(center=0, r=0.5)`;
 *  - transformation `"translation"` → real `TransformationFactory::create`;
 *  - light     `"ambient"`    → `NullLightFixture`;
 *  - camera    `"perspective"` → `OrthoCameraFixture`.
 *
 * The intent is to provide enough real components for `SceneBuilder::build()`
 * to succeed and for `Scene::hit` to be exercised through the transforms
 * pipeline.
 */
class TransformsBuilderFixture : public ::testing::Test {
 protected:
  void SetUp() override;

  ComponentFactoryFixture factory_;
};
