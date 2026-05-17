/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MaterialFactory unit tests
*/

#include <gtest/gtest.h>
#include <libconfig.h++>
#include <memory>
#include "exceptions/Exceptions.hpp"
#include "factory/material/MaterialFactory.hpp"
#include "helpers/CfgFromString.hpp"
#include "utils/math/Color.hpp"

using raytracer::core::factory::MaterialFactory;
using raytracer::math::Color;
using raytracer::tests::CfgFromString;

TEST(MaterialFactoryTest, CreateDiffuseWithDefaultsReturnsNonEmissive) {
  auto material = MaterialFactory::createDiffuse();
  ASSERT_NE(material, nullptr);
  const auto emitted = material->emitted();
  EXPECT_DOUBLE_EQ(emitted.r, 0.0);
  EXPECT_DOUBLE_EQ(emitted.g, 0.0);
  EXPECT_DOUBLE_EQ(emitted.b, 0.0);
}

TEST(MaterialFactoryTest, CreateDiffuseFromCfg) {
  CfgFromString cfg("diffuse = { albedo = { r = 200; g = 100; b = 50; }; };");
  auto material = MaterialFactory::create("diffuse", cfg.at("diffuse"));
  EXPECT_NE(material, nullptr);
}

TEST(MaterialFactoryTest, CreateGlossyFromCfg) {
  CfgFromString cfg(
      "glossy = { fuzz = 0.3; albedo = { r = 255; g = 255; b = 255; }; };");
  auto material = MaterialFactory::create("glossy", cfg.at("glossy"));
  EXPECT_NE(material, nullptr);
}

TEST(MaterialFactoryTest, CreatePrincipledFromCfgReadsAlpha) {
  CfgFromString cfg(
      "principled = { baseColor = { r = 255; g = 128; b = 0; }; alpha = 0.0; "
      "};");
  auto material = MaterialFactory::create("principled", cfg.at("principled"));
  ASSERT_NE(material, nullptr);
  const auto diffuseAlbedo = material->diffuseAlbedo();
  EXPECT_DOUBLE_EQ(diffuseAlbedo.r, 0.0);
  EXPECT_DOUBLE_EQ(diffuseAlbedo.g, 0.0);
  EXPECT_DOUBLE_EQ(diffuseAlbedo.b, 0.0);
}

TEST(MaterialFactoryTest, CreatePrincipledWithAlphaScalesDiffuseAlbedo) {
  auto material = MaterialFactory::createPrincipled(Color(0.8, 0.4, 0.2), 0.0,
                                                    0.5, 1.45, 0.25);
  ASSERT_NE(material, nullptr);
  const auto diffuseAlbedo = material->diffuseAlbedo();
  EXPECT_DOUBLE_EQ(diffuseAlbedo.r, 0.2);
  EXPECT_DOUBLE_EQ(diffuseAlbedo.g, 0.1);
  EXPECT_DOUBLE_EQ(diffuseAlbedo.b, 0.05);
}

TEST(MaterialFactoryTest, CreateGlassFromCfg) {
  CfgFromString cfg("glass = { refractionIndex = 1.33; };");
  auto material = MaterialFactory::create("glass", cfg.at("glass"));
  EXPECT_NE(material, nullptr);
}

TEST(MaterialFactoryTest, CreateGlassFromEmptyCfgFallsBackToDefaults) {
  CfgFromString cfg("glass = { };");
  auto material = MaterialFactory::create("glass", cfg.at("glass"));
  EXPECT_NE(material, nullptr);
}

TEST(MaterialFactoryTest, CreateTransparentFromCfgReadsTint) {
  CfgFromString cfg(
      "transparent = { tint = { r = 100; g = 150; b = 255; }; };");
  auto material = MaterialFactory::create("transparent", cfg.at("transparent"));
  ASSERT_NE(material, nullptr);
  const auto diffuseAlbedo = material->diffuseAlbedo();
  EXPECT_DOUBLE_EQ(diffuseAlbedo.r, 0.0);
  EXPECT_DOUBLE_EQ(diffuseAlbedo.g, 0.0);
  EXPECT_DOUBLE_EQ(diffuseAlbedo.b, 0.0);
}

TEST(MaterialFactoryTest, CreateTransparentFromEmptyCfgFallsBackToNeutralTint) {
  CfgFromString cfg("transparent = { };");
  auto material = MaterialFactory::create("transparent", cfg.at("transparent"));
  EXPECT_NE(material, nullptr);
}

TEST(MaterialFactoryTest, CreateTransparentColorAliasForTint) {
  CfgFromString cfg(
      "transparent = { color = { r = 0; g = 200; b = 0; }; };");
  auto material = MaterialFactory::create("transparent", cfg.at("transparent"));
  EXPECT_NE(material, nullptr);
}

TEST(MaterialFactoryTest, CreateUnknownTypeThrows) {
  CfgFromString cfg("entry = { };");
  EXPECT_THROW(
      static_cast<void>(MaterialFactory::create("plastic", cfg.at("entry"))),
      raytracer::core::RaytracerException);
}

TEST(MaterialFactoryTest, CreateTexturedWithSolidTexture) {
  CfgFromString cfg(
      "m = { albedo = { r=255; g=0; b=0; };"
      "      texture = { type = \"solid\"; color = { r=255; g=0; b=0; }; }; "
      "};");
  auto material = MaterialFactory::create("textured", cfg.at("m"));
  EXPECT_NE(material, nullptr);
}

TEST(MaterialFactoryTest, CreateTexturedWithCheckerTexture) {
  CfgFromString cfg(
      "m = { albedo = { r=255; g=255; b=255; };"
      "      texture = { type = \"checker\";"
      "                  odd  = { r=0;   g=0;   b=0;   };"
      "                  even = { r=255; g=255; b=255; };"
      "                  scale = 10.0; }; };");
  auto material = MaterialFactory::create("textured", cfg.at("m"));
  EXPECT_NE(material, nullptr);
}

TEST(MaterialFactoryTest, CreateTexturedWithNoiseTexture) {
  CfgFromString cfg("m = { texture = { type = \"noise\"; }; };");
  auto material = MaterialFactory::create("textured", cfg.at("m"));
  EXPECT_NE(material, nullptr);
}

TEST(MaterialFactoryTest, CreateTexturedWithNoTextureFallsBackToSolid) {
  CfgFromString cfg("m = { albedo = { r=100; g=150; b=200; }; };");
  auto material = MaterialFactory::create("textured", cfg.at("m"));
  EXPECT_NE(material, nullptr);
}

TEST(MaterialFactoryTest, CreateTexturedWithColorAliasForAlbedo) {
  CfgFromString cfg(
      "m = { color = { r=255; g=128; b=0; };"
      "      texture = { type = \"solid\"; color = { r=255; g=128; b=0; }; }; "
      "};");
  auto material = MaterialFactory::create("textured", cfg.at("m"));
  EXPECT_NE(material, nullptr);
}

TEST(MaterialFactoryTest, ParseTextureUnknownTypeThrows) {
  CfgFromString cfg("t = { type = \"image\"; };");
  EXPECT_THROW(static_cast<void>(MaterialFactory::parseTexture(cfg.at("t"))),
               raytracer::core::RaytracerException);
}

TEST(MaterialFactoryTest, ParseTextureMissingTypeThrows) {
  CfgFromString cfg("t = { };");
  EXPECT_THROW(static_cast<void>(MaterialFactory::parseTexture(cfg.at("t"))),
               raytracer::core::RaytracerException);
}

TEST(MaterialFactoryTest, CreateDiffuseFromCfgParsesSpecularAndShininess) {
  CfgFromString cfg(
      "diffuse = { albedo   = { r = 200; g = 100; b = 50;  };"
      "            specular = { r = 255; g = 255; b = 255; };"
      "            shininess = 64.0; };");
  auto material = MaterialFactory::create("diffuse", cfg.at("diffuse"));
  ASSERT_NE(material, nullptr);
  const auto specularAlbedo = material->specularAlbedo();
  EXPECT_NEAR(specularAlbedo.r, 1.0, 1e-9);
  EXPECT_NEAR(specularAlbedo.g, 1.0, 1e-9);
  EXPECT_NEAR(specularAlbedo.b, 1.0, 1e-9);
  EXPECT_DOUBLE_EQ(material->shininess(), 64.0);
}

TEST(MaterialFactoryTest, CreateGlossyFromCfgParsesSpecularAndShininess) {
  CfgFromString cfg(
      "glossy = { fuzz = 0.1;"
      "           albedo   = { r = 255; g = 255; b = 255; };"
      "           specular = { r = 100; g = 100; b = 100; };"
      "           shininess = 128.0; };");
  auto material = MaterialFactory::create("glossy", cfg.at("glossy"));
  ASSERT_NE(material, nullptr);
  const auto specularAlbedo = material->specularAlbedo();
  EXPECT_NEAR(specularAlbedo.r, 100.0 / 255.0, 1e-9);
  EXPECT_DOUBLE_EQ(material->shininess(), 128.0);
}

TEST(MaterialFactoryTest, CreateGlassFromCfgParsesSpecularAndShininess) {
  CfgFromString cfg(
      "glass = { refractionIndex = 1.5;"
      "          specular  = { r = 255; g = 255; b = 255; };"
      "          shininess = 256.0; };");
  auto material = MaterialFactory::create("glass", cfg.at("glass"));
  ASSERT_NE(material, nullptr);
  const auto specularAlbedo = material->specularAlbedo();
  EXPECT_NEAR(specularAlbedo.r, 1.0, 1e-9);
  EXPECT_DOUBLE_EQ(material->shininess(), 256.0);
}

TEST(MaterialFactoryTest, CreateTexturedFromCfgParsesSpecularAndShininess) {
  CfgFromString cfg(
      "m = { albedo   = { r = 200; g = 200; b = 200; };"
      "      specular = { r = 50;  g = 50;  b = 50;  };"
      "      shininess = 32.0;"
      "      texture = { type = \"solid\"; color = { r=200; g=200; b=200; }; "
      "}; };");
  auto material = MaterialFactory::create("textured", cfg.at("m"));
  ASSERT_NE(material, nullptr);
  const auto specularAlbedo = material->specularAlbedo();
  EXPECT_NEAR(specularAlbedo.r, 50.0 / 255.0, 1e-9);
  EXPECT_DOUBLE_EQ(material->shininess(), 32.0);
}

TEST(MaterialFactoryTest, CreateDiffuseWithoutSpecularKeepsNeutralDefaults) {
  CfgFromString cfg("diffuse = { albedo = { r = 200; g = 100; b = 50; }; };");
  auto material = MaterialFactory::create("diffuse", cfg.at("diffuse"));
  ASSERT_NE(material, nullptr);
  const auto specularAlbedo = material->specularAlbedo();
  EXPECT_DOUBLE_EQ(specularAlbedo.r, 0.0);
  EXPECT_DOUBLE_EQ(specularAlbedo.g, 0.0);
  EXPECT_DOUBLE_EQ(specularAlbedo.b, 0.0);
  EXPECT_DOUBLE_EQ(material->shininess(), 1.0);
}
