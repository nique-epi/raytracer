/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ProgressBar
*/

/**
 * @file ProgressBar.hpp
 * @brief Optional CLI progress reporter for the headless render path.
 *
 * Walls off the unicode progress-bar widget — its mutex-guarded throttle
 * state, the ETA estimator and the terminal escape codes — from
 * `Application`, which only orchestrates the render. Callers wire the
 * widget by handing it the renderer whose `setProgressCallback` will
 * be installed; nothing more.
 */

#pragma once

namespace raytracer::core {

class RaytracerRenderer;

/**
 * @brief Install a CLI progress callback on @p renderer when stderr is
 *        a TTY.
 *
 * When stderr is attached to a terminal, replaces @p renderer's
 * progress callback with one that paints a 30-cell unicode progress
 * bar plus an ETA estimate to stderr. When stderr is redirected (pipe,
 * CI, file), the call is a no-op so callers can invoke it
 * unconditionally on the headless code-path.
 *
 * @param[in,out] renderer Renderer whose progress callback is set.
 */
void attachConsoleProgressBar(RaytracerRenderer& renderer);

}  // namespace raytracer::core
