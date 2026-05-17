# raytracer

Moteur de raytracing C++20 modulaire. Décrit une
scène dans un fichier `.cfg` (libconfig), trace les rayons primaires et
secondaires, et produit une image PPM  soit en direct dans une fenêtre
SFML, soit en rendu *headless* dans un fichier.

Le projet est conçu pour être **étendu sans réécriture** : chaque famille
de composants (primitives, lumières, matériaux, caméras, transformations,
loaders de scène) est définie par une interface et instanciée par une
*factory* qui dispatche sur un nom de type. Ajouter un composant revient
à écrire une classe et à l'enregistrer, voir [Étendre le moteur](#étendre-le-moteur).

## Sommaire

- [Prérequis](#prérequis)
- [Build](#build)
- [Usage](#usage)
- [Format de scène](#format-de-scène)
- [Exemple de scène commenté](#exemple-de-scène-commenté)
- [Scènes d'exemple](#scènes-dexemple)
- [Fonctionnalités](#fonctionnalités)
- [Patterns utilisés](#patterns-utilisés)
- [Architecture du dépôt](#architecture-du-dépôt)
- [Étendre le moteur](#étendre-le-moteur)
- [Bonus](#bonus)
- [Tests](#tests)
- [Format & lint](#format--lint)
- [Documentation](#documentation)

## Prérequis

- CMake **≥ 3.28.3**
- Un compilateur C++20 (clang ≥ 14, gcc ≥ 11)
- **SFML 2** (`graphics`, `window`, `system`)  dépendance **obligatoire** :
  la fenêtre de rendu temps réel est liée au binaire principal
- `make` (ou tout autre générateur supporté par CMake)
- *(optionnel)* `clang-format-20` / `clang-tidy-20` pour les targets
  `format` et `tidy`

`libconfig` et `googletest` sont récupérés automatiquement par
`FetchContent` au premier `cmake configure`. Le build bonus tire en plus
`nlohmann/json` (voir [Bonus](#bonus)).

## Build

Le projet utilise [CMake Presets](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html)
(`CMakePresets.json` à la racine).

### Build rapide

```bash
cmake --workflow build       # configure + compile
./raytracer scenes/example.cfg --no-viewport   # produit out.ppm
```

Le binaire est compilé dans `build/src/raytracer` puis **copié à la
racine du dépôt** (`./raytracer`) par une commande post-build, comme
l'exige le sujet.

### Presets disponibles

| Famille       | Preset    | Effet                                                |
| ------------- | --------- | ---------------------------------------------------- |
| **configure** | `default` | Génère `build/` (Unix Makefiles), build de prod      |
|               | `test`    | Hérite de `default`, active `BUILD_TESTS=ON`         |
|               | `bonus`   | Hérite de `default`, active `BUILD_BONUS=ON`         |
| **build**     | `default` / `test` / `bonus` | Compile le preset configure homonyme  |
| **test**      | `test`    | Lance `ctest`, *output-on-failure*                   |
| **workflow**  | `build`   | configure(`default`) → build(`default`)              |
|               | `bonus`   | configure(`bonus`) → build(`bonus`)                  |
|               | `test`    | configure(`test`) → build(`test`) → ctest(`test`)    |

```bash
cmake --workflow build       # build de prod sans tests ni bonus
cmake --workflow bonus       # build avec les fonctionnalités bonus
cmake --workflow test        # build + exécution de la suite de tests
```

### Build manuel (sans preset)

```bash
cmake -B build -DBUILD_TESTS=ON -DBUILD_BONUS=ON
cmake --build build
```

### Options CMake

| Option        | Défaut  | Description                                            |
| ------------- | ------- | ------------------------------------------------------ |
| `BUILD_TESTS` | `OFF`   | Compile la suite gtest (`tests/`)                      |
| `BUILD_BONUS` | `OFF`   | Compile les fonctionnalités bonus (`bonus/`)           |
| `LOG_LEVEL`   | `info`* | Niveau de log minimum **compilé** dans le binaire      |

`LOG_LEVEL` accepte `trace`, `debug`, `info`, `warn`, `error`, `silent`.
Les appels de log en dessous du niveau choisi sont supprimés à la
compilation (empreinte binaire nulle).
*Défaut : `info` en `Release`, `trace` sinon.

```bash
cmake -B build -DLOG_LEVEL=warn
```

## Usage

```
USAGE: ./raytracer <SCENE_FILE> [--no-bvh] [--no-viewport] [--config <FILE>]
```

| Argument        | Rôle                                                            |
| --------------- | --------------------------------------------------------------- |
| `<SCENE_FILE>`  | Fichier de scène (`.cfg`, ou `.gltf`/`.obj`/… avec le bonus)    |
| `--no-bvh`      | Désactive la structure d'accélération BVH                       |
| `--no-viewport` | Rendu *headless* : pas de fenêtre, écrit directement le fichier |
| `--config <F>`  | Fichier de configuration de rendu JSON (**bonus uniquement**)   |
| `-h`, `--help`  | Affiche l'aide (doit être le seul argument)                     |

Comportement :

- **Par défaut**, le raytracer ouvre une **fenêtre SFML** qui affiche
  l'image au fur et à mesure du rendu (accumulation progressive). La
  touche `Tab` cycle entre les modes d'affichage (*wireframe* →
  *material preview* → *rendered*).
- Avec `--no-viewport`, le rendu est *headless* et l'image est écrite au
  format **PPM** dans `out.ppm` (répertoire courant). Avec le bonus
  `--config`, le nom du fichier de sortie peut être surchargé.

```bash
./raytracer scenes/materials.cfg                  # fenêtre interactive
./raytracer scenes/materials.cfg --no-viewport    # rendu → out.ppm
./raytracer scenes/materials.cfg --no-bvh         # sans accélération BVH
```

**Codes de sortie :** `0` en cas de succès, `84` en cas d'erreur (les
messages d'erreur sont écrits sur `stderr`).

**Logs runtime :** la variable d'environnement `RT_LOG_LEVEL`
(`trace`|`debug`|`info`|`warn`|`error`|`silent`, défaut `info`) règle le
niveau de log à l'exécution, dans la limite du niveau compilé
(`LOG_LEVEL`).

```bash
RT_LOG_LEVEL=debug ./raytracer scenes/example.cfg --no-viewport
```

## Format de scène

Une scène `.cfg` est un fichier [libconfig](https://hyperrealm.github.io/libconfig/).
Toutes les sections sont **optionnelles** sauf `camera` (et au moins une
lumière en mode `rendered`). Les couleurs sont des entiers `0`–`255`.

| Section      | Contenu                                                            |
| ------------ | ------------------------------------------------------------------ |
| `settings`   | Résolution, échantillonnage, tuiles, threads, profondeur de rebond |
| `camera`     | Position, orientation, champ de vision                             |
| `primitives` | Listes `spheres`, `planes`, `cylinders`, `cones`                   |
| `lights`     | Listes `ambient`, `directional`, `point`                           |
| `background` | Fond `solid` ou `gradient`                                         |
| `world`      | Mode d'affichage et occlusion ambiante                             |

### `settings`

| Clé               | Défaut | Description                                  |
| ----------------- | ------ | -------------------------------------------- |
| `imageWidth`      | `800`  | Largeur de l'image en pixels                 |
| `imageHeight`     | `600`  | Hauteur de l'image en pixels                 |
| `samplesPerPixel` | `1`    | Rayons par pixel (anti-aliasing)             |
| `maxDepth`        | `5`    | Profondeur maximale des rebonds              |
| `tileWidth`       | `32`   | Largeur d'une tuile de rendu                 |
| `tileHeight`      | `32`   | Hauteur d'une tuile de rendu                 |
| `numThreads`      | `0`    | Threads de rendu (`0` = autodétection)       |

### `camera`

`type` vaut `perspective` (seul type disponible). Position et orientation :

| Clé          | Description                                                   |
| ------------ | ------------------------------------------------------------- |
| `position`   | Position de la caméra (`{ x; y; z }`)                         |
| `direction`  | Direction du regard ; ou bien `lookAt` (point visé)           |
| `up`         | Vecteur « haut » (défaut `{0,1,0}`)                           |
| `fov`        | Champ de vision vertical en degrés (alias `vfov`)             |
| `aperture`   | Ouverture du diaphragme (profondeur de champ, défaut `0`)     |
| `focusDist`  | Distance de mise au point (défaut `1`)                        |

### `primitives`

Chaque clé est une **liste** d'objets. Tout objet accepte un bloc
`material` optionnel et une liste `transforms` optionnelle.

| Liste       | Type      | Clés                                              |
| ----------- | --------- | ------------------------------------------------- |
| `spheres`   | sphère    | `center`, `radius`                                |
| `planes`    | plan      | `axis` (`"X"`/`"Y"`/`"Z"`) + `position`, ou `point` + `normal` |
| `cylinders` | cylindre  | `center`, `axis`, `radius`, `height`              |
| `cones`     | cône      | `apex`, `axis`, `angle` (radians), `height`       |

### `material`

`type` sélectionne le modèle de surface :

| `type`       | Clés principales                                                  |
| ------------ | ----------------------------------------------------------------- |
| `diffuse`    | `albedo`/`color`, `specular`, `shininess` (Phong optionnel)       |
| `glossy`     | `fuzz`, `albedo`/`color`, `specular`, `shininess` (réflexion)     |
| `glass`      | `refractionIndex`, `tint`, `specular`, `shininess` (réfraction)   |
| `principled` | `baseColor`, `metallic`, `roughness`, `ior`, `alpha` (BSDF)       |
| `textured`   | `albedo`/`color`, `specular`, `shininess`, `texture`              |

Le bloc `texture` d'un matériau `textured` accepte `type` =
`solid` (`color`), `checker` (`odd`, `even`, `scale`), `noise` (Perlin)
ou `bitmap` (`path` vers un `.ppm`).

### `transforms`

Liste appliquée par objet, dans l'ordre d'insertion :

| `type`        | Clés                              |
| ------------- | --------------------------------- |
| `translation` | `offset` (`{ x; y; z }`)          |
| `rotation`    | `axis`, `angle` (radians)         |
| `scale`       | `factor` (`{ x; y; z }`)          |

### `lights`

| Liste         | Type        | Clés                                  |
| ------------- | ----------- | ------------------------------------- |
| `ambient`     | ambiante    | `color`, `intensity`                  |
| `directional` | directionne | `direction`, `color`, `intensity`     |
| `point`       | ponctuelle  | `position`, `color`, `intensity`      |

### `background`

`type` = `solid` (clé `color`) ou `gradient` (clés `topColor`,
`bottomColor`). Absent ⇒ fond noir.

### `world`

| Clé                | Description                                                     |
| ------------------ | --------------------------------------------------------------- |
| `mode`             | `rendered` (défaut), `materialPreview` ou `wireframe`           |
| `ambientOcclusion` | Bloc `{ enabled; samples; radius; intensity; }`                 |

## Exemple de scène commenté

Voici une scène `.cfg` minimale et complète, expliquée bloc par bloc.

```cfg
settings:                          # ── Paramètres de rendu globaux ──────────
{
  imageWidth      = 1920;          # largeur de l'image produite, en pixels
  imageHeight     = 1080;          # hauteur de l'image produite, en pixels
  samplesPerPixel = 4;             # 4 rayons/pixel → anti-aliasing léger
  maxDepth        = 10;            # rebonds max d'un rayon (reflets, réfraction)
};

camera:                            # ── Caméra (obligatoire) ─────────────────
{
  position  = { x = 0.0; y = 0.0; z = 5.0; };   # caméra à 5 unités sur +Z
  direction = { x = 0.0; y = 0.0; z = -1.0; };  # regarde vers -Z
  fov       = 90.0;                              # champ de vision vertical (°)
};

primitives:                        # ── Géométrie de la scène ────────────────
{
  spheres = (                      # liste de sphères
    {                              # sphère rouge à l'origine
      center   = { x = 0.0; y = 0.0; z = 0.0; };
      radius   = 1.0;
      material = { type = "diffuse";            # surface lambertienne
                   albedo = { r = 255; g = 0; b = 0; }; };
    },
    {                              # petite sphère verte décalée sur +X
      center   = { x = 2.0; y = 0.0; z = 0.0; };
      radius   = 0.5;
      material = { type = "diffuse";
                   albedo = { r = 0; g = 255; b = 0; }; };
    }
  );

  planes = (                       # liste de plans
    {                              # sol gris à y = -1
      axis     = "Y";              # plan perpendiculaire à l'axe Y…
      position = -1.0;             # …situé à l'ordonnée -1
      material = { type = "diffuse";
                   albedo = { r = 128; g = 128; b = 128; }; };
    }
  );
};

lights:                            # ── Éclairage (≥ 1 en mode rendered) ─────
{
  ambient = (                      # lumière ambiante : remplit les ombres
    { color = { r = 255; g = 255; b = 255; }; intensity = 0.3; }
  );

  directional = (                  # lumière directionnelle : « soleil »
    { direction = { x = 1.0; y = -1.0; z = -1.0; };  # vient du haut-gauche
      color     = { r = 255; g = 255; b = 255; };
      intensity = 0.8; }
  );
};
```

Rendu de cette scène :

```bash
./raytracer scenes/example.cfg --no-viewport && open out.ppm
```

## Scènes d'exemple

Le dossier [`scenes/`](scenes/) contient des scènes prêtes à rendre,
chacune mettant en avant une fonctionnalité :

| Scène                                                   | Met en avant                                                |
| ------------------------------------------------------- | ----------------------------------------------------------- |
| [`example.cfg`](scenes/example.cfg)                     | Scène minimale (sphères + plan + lumières)                  |
| [`all_components.cfg`](scenes/all_components.cfg)       | Démo complète : toutes les primitives et tous les matériaux |
| [`sphere.cfg`](scenes/sphere.cfg)                       | Primitive sphère                                            |
| [`plane.cfg`](scenes/plane.cfg)                         | Primitive plan                                              |
| [`cylinder.cfg`](scenes/cylinder.cfg)                   | Primitive cylindre                                          |
| [`cone.cfg`](scenes/cone.cfg)                           | Primitive cône                                              |
| [`materials.cfg`](scenes/materials.cfg)                 | Matériaux `diffuse` / `glossy` / `glass` côte à côte        |
| [`lights.cfg`](scenes/lights.cfg)                       | Lumières ambiante + directionnelles colorées                |
| [`translation.cfg`](scenes/translation.cfg)             | Transformation `translation` par objet                     |
| [`rotation.cfg`](scenes/rotation.cfg)                   | Transformation `rotation` par objet                        |
| [`textured_materials.cfg`](scenes/textured_materials.cfg) | Textures procédurales (`checker`, `noise`)                |
| [`bitmap_texture.cfg`](scenes/bitmap_texture.cfg)       | Texture bitmap chargée depuis un `.ppm`                     |
| [`phong_ao.cfg`](scenes/phong_ao.cfg)                   | Reflets Phong + occlusion ambiante                          |

## Fonctionnalités

Référence : les niveaux **must / should / could** du sujet
([`doc/subject.md`](doc/subject.md)).

### Must — fonctionnalités obligatoires

- **Primitives** : sphère, plan
- **Transformation** : translation
- **Lumières** : directionnelle, ambiante
- **Matériau** : couleur unie (matériau `diffuse`)
- **Configuration de scène** : ajout de primitives, lumières et caméra
- **Sortie** : fichier image PPM, sans interface obligatoire

### Should

- **Primitives** : cylindre, cône
- **Transformation** : rotation
- **Lumières** : ombres portées (rayons d'ombre)

### Could — bonus implémentés

- **Transformation** : scale
- **Lumières** : directionnelles multiples, ponctuelles multiples,
  lumières colorées, modèle de réflexion de Phong, occlusion ambiante
- **Matériaux** : transparence / réfraction (`glass`), réflexion
  (`glossy`), BSDF *principled*, texture depuis fichier (`bitmap`),
  damier procédural (`checker`), bruit de Perlin procédural (`noise`)
- **Optimisations** : partitionnement de l'espace (BVH), multithreading
  (pool de threads, rendu par tuiles)
- **Interface** : affichage de l'image pendant et après le rendu
  (fenêtre SFML), aperçu rapide via les modes *wireframe* et
  *material preview*
- **Primitives supplémentaires** : triangle et mesh (chargés par le
  loader Assimp, voir [Bonus](#bonus))

## Patterns utilisés

Le sujet impose au moins deux design patterns. Le moteur en utilise
plusieurs :

| Pattern       | Où                                                                    |
| ------------- | --------------------------------------------------------------------- |
| **Factory**   | `ComponentFactory` et ses sous-factories (`PrimitiveFactory`, `LightFactory`, `MaterialFactory`, `CameraFactory`, `TransformationFactory`) : chacune construit une instance fraîche à partir d'un nom de type et d'un bloc libconfig. `SceneLoaderFactory` choisit un loader selon l'extension du fichier. |
| **Builder**   | `SceneBuilder` assemble une `Scene` étape par étape (`addObject`, `addLight`, `addCamera`, `setBackground`) puis valide le tout dans `build()`. |
| **Composite** | `Collection` regroupe des `IObject` et **est elle-même un** `IObject` ; une intersection sur la scène délègue récursivement à la collection racine. Le `BVHNode` suit le même contrat. |
| **Decorator** | `TransformedObject` enveloppe un `IObject` et lui ajoute des transformations tout en exposant exactement la même interface `IObject`. |
| **Strategy**  | `IShadingMode` (`Wireframe`, `MaterialPreview`, `Rendered`) et `IIntegrator` (`WhittedIntegrator`) : l'algorithme de shading est interchangeable à l'exécution via `ShadingContext`. |
| **Observer**  | Le renderer expose un *callback* de progression (`setProgressCallback`) qui notifie la barre de progression et la fenêtre temps réel. |

## Architecture du dépôt

```
raytracer/
├── CMakeLists.txt          Build racine : options, FetchContent, targets
├── CMakePresets.json       Presets default / test / bonus
├── raytracer               Binaire copié à la racine après le build
├── src/
│   ├── main.cpp            Point d'entrée : parsing CLI, codes de sortie
│   ├── application/        Application (orchestration) + ArgsParser
│   ├── exceptions/         RaytracerException & dérivées
│   ├── config/             Exceptions de configuration
│   ├── constants/          Messages d'erreur
│   ├── common/helper/      Logger structuré, Queue
│   ├── os/threads/         ThreadPool (rendu multithread)
│   ├── utils/math/         Vector3D, Color, Ray, Matrix, AABB, Optics…
│   ├── components/
│   │   ├── Primitives/     Sphere, Plane, Cylinder, Cone, Triangle, Mesh,
│   │   │                   Collection (composite), BVH, TransformedObject
│   │   ├── Transformations/ Translation, Rotation, Scale
│   │   ├── camera/         PerspectiveCamera
│   │   ├── light/          Ambient, Directional, Point
│   │   └── material/       Diffuse, Glossy, Glass, Principled, Textured
│   ├── factory/            ComponentFactory + sous-factories par interface
│   ├── scene/              Scene, SceneBuilder, loaders .cfg, World, fonds
│   ├── rendering/          renderer, integrator, shading, helpers
│   ├── output/             Writers d'image (PPM)
│   └── interface/          Fenêtre SFML : Viewport, ViewportRunner…
├── scenes/                 Scènes .cfg de démonstration
├── tests/                  Suite de tests googletest
├── bonus/                  Loader Assimp, denoiser OID, config JSON
└── doc/                    raytracer.1 (manpage), logger.md, subject.md
```

## Étendre le moteur

L'extensibilité du moteur passe par les **interfaces** et les
**factories**. Pour chaque famille de composants, il existe une
interface à implémenter et une factory où enregistrer le nouveau type.

| Famille        | Interface       | Factory                 | Dispatch CFG             |
| -------------- | --------------- | ----------------------- | ------------------------ |
| Primitive      | `IObject`       | `PrimitiveFactory`      | `CFGSceneLoader::parsePrimitives` |
| Lumière        | `ILight`        | `LightFactory`          | `CFGSceneLoader::parseLights`     |
| Matériau       | `IMaterial`     | `MaterialFactory`       | bloc `material` d'un objet        |
| Caméra         | `ICamera`       | `CameraFactory`         | `CFGSceneLoader::parseCamera`     |
| Transformation | `ITransformation` | `TransformationFactory` | bloc `transforms` d'un objet    |
| Format de scène | `ISceneLoader` | `SceneLoaderFactory`    | `Application` (`registerLoader`)  |

**Exemple — ajouter une primitive :**

1. Créer `src/components/Primitives/<Nom>/<Nom>.{hpp,cpp}` avec une
   classe qui implémente `IObject` (`hits`, `getBoundingBox`,
   `applyTransformation`).
2. Ajouter les sources à `src/components/Primitives/CMakeLists.txt`.
3. Dans `PrimitiveFactory::create`, ajouter une branche
   `if (type == "<nom>")` qui parse le `libconfig::Setting` et construit
   l'instance.
4. Dans `CFGSceneLoader::parsePrimitives`, enregistrer la liste CFG
   correspondante via `addList(p, "<noms>", "<nom>", builder, &SceneBuilder::addObject)`.

La nouvelle primitive est alors instanciable depuis n'importe quelle
scène `.cfg`, sans toucher au renderer ni à l'intégrateur. Le même
schéma s'applique aux autres familles.

## Bonus

Les fonctionnalités bonus sont compilées lorsque `BUILD_BONUS=ON`
(`cmake --workflow bonus`). Elles vivent dans le dossier
[`bonus/`](bonus/) et le binaire n'y est lié qu'avec ce flag.

| Bonus                  | Description                                                             |
| ---------------------- | ----------------------------------------------------------------------- |
| **Loader Assimp**      | Charge des scènes `.gltf`, `.glb`, `.obj`, `.fbx`… (triangles & meshes) |
| **OpenImageDenoise**   | Débruiteur appliqué à l'image après le rendu                            |
| **Config de rendu JSON** | `--config <fichier.json>` surcharge les paramètres de rendu           |

Sans `BUILD_BONUS`, l'option `--config` est rejetée avec un message
explicite. Un exemple de fichier de configuration est fourni :
[`config.example.json`](config.example.json) (résolution, échantillons,
threads, mode d'affichage, nom du fichier de sortie).

```bash
cmake --workflow bonus
./raytracer scene.gltf --no-viewport --config config.example.json
```

## Tests

```bash
cmake --workflow test        # configure + build + ctest
```

ou, en deux temps :

```bash
cmake --build --preset test
ctest --preset test
```

## Format & lint

Targets auto-détectées si les outils sont dans le `PATH` (le projet vise
`clang-format-20` / `clang-tidy-20`, avec repli sur les binaires sans
suffixe) :

```bash
cmake --build build --target format        # applique clang-format -i
cmake --build build --target format-check  # CI : échoue s'il y a un diff
cmake --build build --target tidy          # clang-tidy
```

## Documentation

- [`doc/raytracer.1`](doc/raytracer.1) — page de manuel (section 1).
  Aperçu local : `cmake --build build --target man`.
- [`doc/logger.md`](doc/logger.md) — guide du logger structuré.
- [`doc/subject.md`](doc/subject.md) — énoncé du projet.
