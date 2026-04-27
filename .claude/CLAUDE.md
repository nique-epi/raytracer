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
