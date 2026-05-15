/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Queue
*/

/**
 * @file Queue.hpp
 * @brief Generic thread-safe blocking queue inspired by boost::concurrent_queue.
 *
 * Producer/consumer queue with a closeable lifecycle: once @ref close is
 * called, no new items may be pushed and waiting consumers are unblocked.
 *
 * Generic by design: the `os::threads::ThreadPool` instantiates it with
 * `std::function<void()>`, but it is also intended for any future
 * observer-style pattern (event bus, notifier, etc.) that needs a
 * fan-out FIFO with backpressure-free push semantics.
 *
 * The queue is non-copyable and non-movable — it owns a mutex and a
 * condition variable, so it must be aggregated by composition.
 */

#pragma once

#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <queue>
#include <utility>

namespace raytracer::common {

/**
 * @brief Outcome of a pop operation.
 *
 * @c Success — an item was successfully extracted.
 * @c Closed  — the queue was closed and contained no item to drain.
 * @c Empty   — non-blocking pop on an empty (but still open) queue.
 */
enum class QueueStatus : std::uint8_t { Success, Closed, Empty };

template <typename T>
class Queue {
 public:
  Queue() = default;
  ~Queue() = default;

  Queue(const Queue&) = delete;
  Queue& operator=(const Queue&) = delete;
  Queue(Queue&&) = delete;
  Queue& operator=(Queue&&) = delete;

  /**
   * @brief Push an item at the back of the queue.
   * @return false if the queue is closed (item is dropped), true otherwise.
   */
  bool push(T value) {
    {
      std::lock_guard<std::mutex> lock(mutex_);
      if (closed_) {
        return false;
      }
      items_.push(std::move(value));
    }
    notEmpty_.notify_one();
    return true;
  }

  /**
   * @brief Block until an item is available, the queue is closed, or both.
   *
   * On @ref QueueStatus::Closed the @p out reference is left untouched.
   */
  QueueStatus waitPop(T& out) {
    std::unique_lock<std::mutex> lock(mutex_);
    notEmpty_.wait(lock, [this] { return closed_ || !items_.empty(); });
    if (items_.empty()) {
      return QueueStatus::Closed;
    }
    out = std::move(items_.front());
    items_.pop();
    return QueueStatus::Success;
  }

  /**
   * @brief Non-blocking pop.
   *
   * Returns @ref QueueStatus::Empty when the queue is open but empty, and
   * @ref QueueStatus::Closed when it has been closed and is empty.
   */
  QueueStatus tryPop(T& out) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (items_.empty()) {
      return closed_ ? QueueStatus::Closed : QueueStatus::Empty;
    }
    out = std::move(items_.front());
    items_.pop();
    return QueueStatus::Success;
  }

  /**
   * @brief Mark the queue as closed and wake up all waiters.
   *
   * Items already in the queue can still be drained via @ref waitPop /
   * @ref tryPop until empty; subsequent @ref push calls return false.
   */
  void close() {
    {
      std::lock_guard<std::mutex> lock(mutex_);
      closed_ = true;
    }
    notEmpty_.notify_all();
  }

  bool isClosed() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return closed_;
  }

  bool isEmpty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return items_.empty();
  }

  std::size_t size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return items_.size();
  }

 private:
  mutable std::mutex mutex_;
  std::condition_variable notEmpty_;
  std::queue<T> items_;
  bool closed_{false};
};

}  // namespace raytracer::common
