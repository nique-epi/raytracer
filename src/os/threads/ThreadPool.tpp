/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ThreadPool template definitions
*/

/**
 * @file ThreadPool.tpp
 * @brief Out-of-class template definitions for ThreadPool.
 *
 * Included at the very end of ThreadPool.hpp. Consumers should always
 * include "os/threads/ThreadPool.hpp" — this file is just a readability
 * split for the @c submit() template body (R1 template exception,
 * option B: definitions in a .tpp).
 *
 * The first directive of this file pulls in ThreadPool.hpp. In the
 * normal include flow (`#include "ThreadPool.hpp"` → which itself
 * `#include "ThreadPool.tpp"` at the end), `#pragma once` short-circuits
 * the back-reference, so the cost is zero. When clangd / an IDE opens
 * the .tpp standalone, this defensive include is what makes the class
 * declarations visible and keeps `ThreadPool::submit` resolvable.
 */

#pragma once

#include "ThreadPool.hpp"

#include <future>
#include <memory>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>

namespace raytracer::os::threads {

template <typename Func, typename... Args>
auto ThreadPool::submit(Func&& func, Args&&... args)
    -> std::future<std::invoke_result_t<Func, Args...>> {
  using ReturnType = std::invoke_result_t<Func, Args...>;

  if (!running_.load(std::memory_order_acquire)) {
    throw std::runtime_error("ThreadPool::submit on a stopped pool");
  }

  auto bound = [callable = std::forward<Func>(func),
                argsTuple = std::make_tuple(
                    std::forward<Args>(args)...)]() mutable -> ReturnType {
    return std::apply(std::move(callable), std::move(argsTuple));
  };

  auto packagedTask =
      std::make_shared<std::packaged_task<ReturnType()>>(std::move(bound));
  std::future<ReturnType> future = packagedTask->get_future();

  inFlightTasks_.fetch_add(1, std::memory_order_relaxed);
  if (!taskQueue_.push([packagedTask]() { (*packagedTask)(); })) {
    inFlightTasks_.fetch_sub(1, std::memory_order_relaxed);
    throw std::runtime_error("ThreadPool::submit on a stopped pool");
  }
  return future;
}

}  // namespace raytracer::os::threads
