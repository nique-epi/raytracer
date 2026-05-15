/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ThreadPool
*/

/**
 * @file ThreadPool.hpp
 * @brief Fixed-size worker pool dispatching tasks through a Queue.
 *
 * Design goals:
 * - Boost.thread-like API: submit any invocable, get a std::future back.
 * - Composition over inheritance: tasks live in a dedicated Queue<Task>.
 * - Graceful shutdown by default: destructor drains queued tasks then joins.
 * - waitIdle() acts as a fence — useful to synchronize on a render tile batch
 *   without tearing down the pool.
 *
 * Template definitions for @c submit live in ThreadPool.tpp, which is
 * included at the bottom of this header (R1 template exception).
 */

#pragma once

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <future>
#include <mutex>
#include <thread>
#include <type_traits>
#include <vector>
#include "common/helper/Queue.hpp"

namespace raytracer::os::threads {

class ThreadPool {
 public:
  using Task = std::function<void()>;
  using TaskQueue = ::raytracer::common::Queue<Task>;

  /**
   * @brief Build a pool sized to std::thread::hardware_concurrency().
   *
   * Falls back to a single worker if hardware concurrency reports zero.
   */
  ThreadPool();

  /**
   * @brief Build a pool with @p workerCount workers (minimum 1).
   */
  explicit ThreadPool(std::size_t workerCount);

  /**
   * @brief Drain pending tasks, signal shutdown, join all workers.
   */
  ~ThreadPool();

  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;
  ThreadPool(ThreadPool&&) = delete;
  ThreadPool& operator=(ThreadPool&&) = delete;

  /**
   * @brief Submit a callable for asynchronous execution.
   *
   * Returns a std::future holding the result (or any exception thrown by the
   * callable). Submitting after @ref shutdown throws std::runtime_error.
   *
   * Definition lives in ThreadPool.tpp.
   */
  template <typename Func, typename... Args>
  auto submit(Func&& func, Args&&... args)
      -> std::future<std::invoke_result_t<Func, Args...>>;

  /**
   * @brief Block until all submitted tasks have completed.
   *
   * Workers stay alive — submit() can still be called afterwards. Cheap to
   * call on an already idle pool.
   */
  void waitIdle();

  /**
   * @brief Idempotent shutdown: close the queue, drain it, join all workers.
   *
   * After this call submit() throws and the pool is no longer running. The
   * destructor calls shutdown() automatically.
   */
  void shutdown();

  std::size_t workerCount() const noexcept;
  std::size_t pendingTaskCount() const;
  std::size_t inFlightTaskCount() const noexcept;
  bool isRunning() const noexcept;

 private:
  void createWorkers(std::size_t workerCount);
  void workerLoop();

  TaskQueue taskQueue_;
  std::vector<std::thread> workers_;

  std::atomic<bool> running_{true};
  std::atomic<std::size_t> inFlightTasks_{0};

  mutable std::mutex idleMutex_;
  std::condition_variable idleCondition_;
};

}  // namespace raytracer::os::threads

// Template definitions — kept out-of-class for readability.
// Must stay at the very end of this header.
#include "ThreadPool.tpp"
