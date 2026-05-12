# Project Rules — Raytracer

Ce fichier définit les règles de codage et d'architecture du projet. Il est
chargé automatiquement par Claude Code à chaque session sur ce repo et doit
être respecté à la lettre.

## Règle méta — comment ce fichier évolue

À chaque fois que l'utilisateur remonte un problème sur la manière de coder,
sur l'architecture, sur le style, ou sur un anti-pattern repéré dans le code,
**ce retour doit être transformé en règle numérotée et ajoutée à ce fichier**.

Principes :

- Les règles sont **permanentes** et **numérotées de façon stable**
  (`R1`, `R2`, …). On ne renumérote jamais une règle existante : les
  références (`cf. R3`) doivent rester valides dans le temps.
- On ne supprime une règle qu'avec une décision explicite de l'utilisateur.
- Si une règle est révisée, on garde le même numéro et on note la
  modification.
- Avant d'ajouter une nouvelle règle, vérifier qu'elle ne fait pas doublon
  avec une règle existante — dans ce cas, **étendre** la règle existante
  plutôt qu'en créer une nouvelle.

### Format d'une règle

```
### R<n> — <titre court>

**Règle :** <énoncé impératif en une phrase>.

**Pourquoi :** <raison / contexte>.

**À appliquer :** <où et quand la règle s'active : fichiers, situations>.

**Exemple interdit :** <snippet>
**Exemple correct :** <snippet>
```

Les sections `Exemple interdit` / `Exemple correct` sont optionnelles mais
recommandées dès qu'un cas concret aide à lever l'ambiguïté.

### Comment ajouter une nouvelle règle

1. Prendre le prochain numéro libre : si la dernière règle est `R7`, la
   nouvelle est `R8`.
2. Suivre le gabarit ci-dessus.
3. L'insérer à la fin de la section **Règles** ci-dessous.
4. Ne **pas** renuméroter les règles existantes.

---

## Règles

### R1 — Séparation stricte `.hpp` / `.cpp`

**Règle :** un fichier `.hpp` ne contient que des **déclarations** (classes,
signatures de méthodes, `enum`, `struct`, `using`, déclarations de templates,
forward declarations). **Aucune logique métier, aucun corps de méthode** ne
doit apparaître dans un `.hpp`. Toute définition va dans le `.cpp`
correspondant.

**Pourquoi :** garantir une séparation claire interface / implémentation,
réduire les temps de compilation, éviter les dépendances transitives via les
headers, et faciliter la lecture (un `.hpp` doit pouvoir être survolé en
quelques secondes pour comprendre l'API d'une classe).

**À appliquer :** tous les fichiers `.hpp` du projet, sans exception
fonctionnelle.

**Exceptions techniques limitées et justifiées :**

- Templates qui *doivent* techniquement être définis dans le header (ou dans
  un `.tpp` inclus en fin de header).
- `constexpr` lorsque la définition est requise à la compilation.
- Méthodes triviales `= default` / `= delete`.

Tout le reste — y compris les getters/setters d'une ligne — va dans le
`.cpp`.

**Exemple interdit (`Foo.hpp`) :**

```cpp
class Foo {
 public:
    int bar() { return 42; }                  // INTERDIT : corps dans le .hpp
    void setName(const std::string& n) {      // INTERDIT
        name_ = n;
    }
 private:
    std::string name_;
};
```

**Exemple correct :**

```cpp
// Foo.hpp
class Foo {
 public:
    int bar();
    void setName(const std::string& n);
 private:
    std::string name_;
};
```

```cpp
// Foo.cpp
#include "Foo.hpp"

int Foo::bar() { return 42; }
void Foo::setName(const std::string& n) { name_ = n; }
```

### R2 — Interfaces purement virtuelles

**Règle :** une **interface** (classe destinée à être uniquement implémentée
par d'autres, jamais instanciée) doit n'exposer que des **méthodes purement
virtuelles** (`= 0`) et un destructeur virtuel `= default`. Aucune méthode
d'interface ne doit avoir de corps `{}`, même vide.

**Pourquoi :** une interface décrit un contrat, pas un comportement. Fournir
une implémentation par défaut (`virtual void render() {}`) brouille la
distinction interface / classe abstraite et masque les oublis
d'implémentation côté classes dérivées.

**À appliquer :** toute classe dont le rôle est d'être une interface
(typiquement préfixée `I`, par exemple `IRenderer`, `IPrimitive`,
`ILightSource`). Les classes abstraites *partiellement* implémentées ne sont
pas des interfaces et ne sont pas concernées par cette règle.

**Exemple interdit :**

```cpp
class IRenderer {
 public:
    virtual ~IRenderer() = default;
    virtual void render() {}                  // INTERDIT : corps {}
    virtual void setQuality(int q) {}         // INTERDIT
};
```

**Exemple correct :**

```cpp
// IRenderer.hpp
class IRenderer {
 public:
    virtual ~IRenderer() = default;
    virtual void render() = 0;
    virtual void setQuality(int q) = 0;
};
```

Note : le destructeur virtuel par défaut (`= default`) est autorisé et
recommandé — c'est la seule "implémentation" tolérée dans une interface.

### R3 — Aucun co-auteur ni mention d'assistant dans les commits

**Règle :** les messages de commit ne doivent contenir **aucun trailer
`Co-Authored-By:`**, **aucune mention d'un assistant IA** (Claude, Copilot,
ChatGPT, Cursor, etc.), **aucun lien vers un outil d'IA**, et **aucun
emoji robot / "Generated with"**. Le sujet, le corps et les trailers du
commit doivent décrire uniquement *ce que* le commit change et *pourquoi*.

**Pourquoi :** la paternité d'un commit appartient à l'auteur humain qui
prend la responsabilité du code. L'outillage utilisé pour produire le diff
(IA, IDE, snippets) est un détail privé qui ne doit pas polluer
l'historique git ni le repo mirror Epitech. Les Co-Authored-By légitimes
(pair-programming humain) ne sont pas l'usage qu'on a ici, donc on
l'interdit pour éviter toute ambiguïté.

**À appliquer :** absolument tous les commits du projet, y compris les
commits intermédiaires d'une PR (squash ou non). S'applique aussi à toute
réécriture d'historique : si un commit existant contient une de ces
mentions, il doit être réécrit avant push.

**Exemple interdit :**

```
chore: gitignore the subject PDF

Co-Authored-By: Claude Opus 4.7 (1M context) <noreply@anthropic.com>
```

```
feat: add Vector3 utilities

🤖 Generated with [Claude Code](https://claude.com/claude-code)
```

**Exemple correct :**

```
chore: gitignore the subject PDF
```

```
feat: add Vector3 utilities

Implements addition, scaling and dot product. Used by the camera
ray-generation step.
```

### R4 — Indexation mémoire à chaque découverte de fichier ou de système

**Règle :** dès qu'un fichier, une interface, un pattern CMake ou un système est **lu ou découvert** au cours d'une session, son contenu essentiel (API, signatures, conventions, emplacement) doit être **indexé ou mis à jour dans les fichiers mémoire** avant la fin de la session.

**Pourquoi :** éviter de re-lire et re-explorer les mêmes fichiers à chaque nouvelle conversation, ce qui consomme des crédits inutilement. Chaque re-découverte est un coût payé deux fois.

**À appliquer :** systématiquement après avoir lu un `.hpp`, `.cpp`, `CMakeLists.txt`, fixture de test, interface ou après avoir compris un pattern non encore documenté en mémoire.

**Exemple interdit :** lire `ITransformation.hpp` pour utiliser `apply()` / `applyToNormal()` sans en laisser de trace en mémoire.

**Exemple correct :** après avoir lu `ITransformation.hpp`, mettre à jour `project_component_structure.md` avec la signature de ses méthodes, puis mettre à jour `MEMORY.md`.

### R5 — Pas de préfixe `k` pour les constantes

**Règle :** les constantes (`constexpr`, `const`, valeurs d'`enum`) ne doivent **pas** être préfixées par `k` (style Google). Utiliser un nom direct en `camelCase` ou en `UPPER_SNAKE_CASE` selon le contexte.

**Pourquoi :** le préfixe `k` est une convention Google qui n'apporte rien au-delà de ce que `constexpr` / `const` / `enum` expriment déjà au type system. Elle pollue la lecture sans ajouter d'information. Le projet utilise déjà des constantes sans préfixe (`epsilon` dans `Constants.hpp`, `DEFAULT_FOV` dans `PerspectiveCamera.hpp`).

**À appliquer :** toute nouvelle constante. Les constantes existantes en `kFoo` doivent être renommées dès qu'on touche au fichier.

**Exemple interdit :**

```cpp
constexpr double kShadowRayTMin = 0.001;
constexpr int kMaxDepth = 10;
constexpr double kEpsilon = 1e-9;
```

**Exemple correct :**

```cpp
constexpr double shadowRayTMin = 0.001;
constexpr int maxDepth = 10;
constexpr double epsilon = 1e-9;

// UPPER_SNAKE_CASE accepté pour les constantes "publiques" exposées
// dans un header :
constexpr double DEFAULT_FOV = 90.0;
```

### R6 — Pas d'abréviation dans les identifiants

**Règle :** les identifiants publics (méthodes, fonctions, classes, variables
membres exposées) doivent utiliser des **noms complets et explicites**, jamais
des abréviations — même quand l'abréviation est une convention répandue dans
le domaine (ex. PBRT, mitsuba, etc.).

**Pourquoi :** une abréviation idiomatique pour un expert du domaine est
opaque pour tout autre lecteur. Le code est lu plus souvent qu'écrit, et le
public du projet (jury, futurs collaborateurs Epitech) ne partage pas
forcément le bagage théorique sous-jacent. Le coût supplémentaire à
l'écriture est négligeable, le gain en lisibilité est durable.

**À appliquer :** toute nouvelle déclaration et toute déclaration touchée
lors d'un refactoring. Les abréviations établies pour les types fondamentaux
de la STL (`std::ptr`, `std::ref`) ne sont pas concernées — la règle vise les
identifiants **du projet**.

**Exemple interdit :**

```cpp
// Référence PBRT : Li = "incoming radiance"
virtual math::Color Li(const math::Ray& ray,
                       const scene::Scene& scene, int depth) = 0;

// Idem pour des abréviations "métier"
double calcBRDF(const Vector3D& wi, const Vector3D& wo);  // wi, wo, BRDF
int computeNbHits();                                       // Nb
void initCfg();                                            // Cfg
```

**Exemple correct :**

```cpp
virtual math::Color computeRadiance(const math::Ray& ray,
                                    const scene::Scene& scene,
                                    int depth) = 0;

double computeReflectance(const Vector3D& incomingDirection,
                          const Vector3D& outgoingDirection);
int computeHitCount();
void initConfiguration();
```
