# Agent guidance for `raytracer`

## What this project is

- C++20 raytracer built with CMake.
- The executable is named `raytracer` and is emitted in the repository root by default.
- The assignment requirements live in [`subject.md`](doc/subject.md); follow that file for feature scope and grading constraints.

## Build and layout

- Use the root `CMakeLists.txt` as the source of truth for build settings.
- `src/` contains the application code and its own `CMakeLists.txt`.
- `WITH_TESTS` is an optional CMake flag; only add test sources if a `tests/` directory is introduced.
- Keep `src.list` in sync if the project tooling expects it for source inventory.

## Style and conventions

- Follow [`.clang-format`](.clang-format) and [`.editorconfig`](.editorconfig).
- Use 2-space indentation, no tabs, and keep lines within the project’s 80-column style.
- Preserve the existing minimal CMake structure unless there is a clear project need to expand it.

## Implementation notes

- Write diagnostics to stderr and return exit code `84` for runtime errors, per the assignment brief.
- Avoid committing build outputs, temporary files, or generated binaries; `.gitignore` already covers the usual artifacts.
- Prefer small, focused changes that keep the project easy to extend with raytracing primitives, lights, and scene parsing.

## Good places to check before changing code

- [`subject.md`](subject.md) for feature requirements and constraints.
- [`src/main.cpp`](src/main.cpp) for the current entry point.
- [`src/CMakeLists.txt`](src/CMakeLists.txt) for per-target wiring.
