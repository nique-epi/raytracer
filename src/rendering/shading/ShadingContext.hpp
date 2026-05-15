/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ShadingContext
*/

/**
 * @file ShadingContext.hpp
 * @brief Strategy-pattern context that owns the active `IShadingMode`.
 *
 * `ShadingContext` is the "Context" role in the textbook Strategy
 * pattern: it holds a swappable reference to one concrete strategy
 * (`IShadingMode`) and exposes a setter so a client (typically the
 * UI / `Application`) can change the active strategy at runtime.
 *
 * Thread-safety: the active strategy is guarded by a `std::mutex`.
 * `setStrategy` and `currentStrategy` are safe to call concurrently
 * from any thread.
 *
 * Frame-consistency contract: the renderer snapshots the strategy
 * **once** per `render()` call via `currentStrategy()` and reuses it
 * across the whole frame. A `setStrategy()` call that races with an
 * in-flight render takes effect at the **next** `render()` — never
 * mid-frame — so the user never sees a half-wireframe / half-rendered
 * image.
 *
 * Both `setStrategy` and the constructor throw `std::invalid_argument`
 * on a null strategy: a context without a strategy has no meaning.
 */

#pragma once

#include <memory>
#include <mutex>

namespace raytracer::shading {

class IShadingMode;

class ShadingContext {
 public:
  explicit ShadingContext(std::shared_ptr<IShadingMode> initial);
  ~ShadingContext() = default;

  ShadingContext(const ShadingContext&) = delete;
  ShadingContext& operator=(const ShadingContext&) = delete;
  ShadingContext(ShadingContext&&) = delete;
  ShadingContext& operator=(ShadingContext&&) = delete;

  void setStrategy(std::shared_ptr<IShadingMode> strategy);

  [[nodiscard]] std::shared_ptr<IShadingMode> currentStrategy() const;

 private:
  mutable std::mutex mutex_;
  std::shared_ptr<IShadingMode> strategy_;
};

}  // namespace raytracer::shading
