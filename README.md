# raytracer

Moteur de raytracing C++20 modulaire (Epitech, G-OOP-400). Plugins
chargés dynamiquement, scènes décrites en `.cfg` (libconfig).

## Prérequis

- CMake **≥ 3.28.3**
- Compilateur C++20 (clang ≥ 14, gcc ≥ 11)
- `make` (ou tout autre générateur supporté par CMake)
- (optionnel) `clang-format-20` / `clang-tidy-20` pour les targets `format`
  et `tidy`
- (optionnel) SFML 2 pour le bonus interactif

`libconfig` et `googletest` sont récupérés automatiquement par
`FetchContent` au premier `cmake configure`.

## Build rapide

```bash
cmake --workflow build       # configure + build, target par défaut
./build/src/raytracer scenes/example.cfg > image.ppm
```

## Presets CMake

Le projet utilise [CMake Presets](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html)
(`CMakePresets.json` à la racine). Trois familles de presets :

### Configure presets

| Preset    | Effet                                        |
| --------- | -------------------------------------------- |
| `default` | Génère `build/` avec Unix Makefiles          |
| `test`    | Hérite de `default`, active `BUILD_TESTS=ON` |

```bash
cmake --preset default       # build de prod, sans tests
cmake --preset test          # build avec les tests gtest
```

### Build presets

```bash
cmake --build --preset default   # construit raytracer + plugins
cmake --build --preset test      # construit raytracer + plugins + tests
```

### Test preset

```bash
ctest --preset test          # lance la suite de tests, output-on-failure
```

### Workflow presets (raccourcis combinés)

| Workflow | Étapes                                            |
| -------- | ------------------------------------------------- |
| `build`  | configure(`default`) → build(`default`)           |
| `test`   | configure(`test`) → build(`test`) → ctest(`test`) |

```bash
cmake --workflow build       # bootstrap complet sans tests
cmake --workflow test        # bootstrap + run all tests
```

## Options CMake

| Option        | Défaut | Description                                  |
| ------------- | ------ | -------------------------------------------- |
| `BUILD_TESTS` | `OFF`  | Compile la suite gtest (`tests/`)            |
| `BUILD_BONUS` | `OFF`  | Compile le viewer SFML temps réel (`bonus/`) |

Activation manuelle (sans preset) :

```bash
cmake -B build -DBUILD_TESTS=ON -DBUILD_BONUS=ON
cmake --build build
```

## Lancement

```bash
./build/src/raytracer <scene.cfg> > image.ppm
```

Le raytracer écrit l'image PPM sur `stdout`. Redirige vers un fichier ou
pipe vers un viewer.

Une scène d'exemple est fournie : [`scenes/example.cfg`](scenes/example.cfg).

## Plugins

Les `.so` sont produits dans `plugins/` à la racine du dépôt (chemin
absolu, défini par `LIBRARY_OUTPUT_DIRECTORY` dans
[`plugins/CMakeLists.txt`](plugins/CMakeLists.txt)). Le binaire les
charge dynamiquement à l'exécution.

Plugins actuels :

| Fichier                                   | Type                            |
| ----------------------------------------- | ------------------------------- |
| `raytracer_perspective_camera.so`         | caméra perspective              |
| `ambient.so`                              | lumière ambiante                |
| `directional.so`                          | lumière directionnelle (soleil) |
| `Sphere.so`, `Cone.so`                    | primitives                      |
| `DiffuseMaterial.so`                      | matériau diffus                 |
| `raytracer_rotation_transformation.so`    | rotation                        |
| `raytracer_translation_transformation.so` | translation                     |

## Tests

```bash
cmake --workflow test
```

ou en deux temps après une config `test` :

```bash
cmake --build --preset test
ctest --preset test
```

## Format & lint

Targets auto-détectés si les outils sont dans le `PATH` :

```bash
cmake --build build --target format        # applique clang-format -i
cmake --build build --target format-check  # CI-friendly, échec si diff
cmake --build build --target tidy          # clang-tidy avec compile_commands.json
```

Le projet cible `clang-format-20` / `clang-tidy-20` en priorité, et
fallback sur les binaires sans suffixe.

## Layout du dépôt

```
src/
  main.cpp               point d'entrée
  core/                  exceptions, registry, application
  utils/math/            Vector3D, Color, Ray, AABB, …
  plugin/                PluginLoader (dlopen wrapper)
  scene/                 loaders .cfg
  components/            interfaces + concrets (camera, light, primitives, …)
  output/                writers PPM, …
plugins/                  CMakeLists construit les .so depuis src/
scenes/                   scènes .cfg de test
tests/                    suite gtest
bonus/                    viewer SFML (optionnel)
```
