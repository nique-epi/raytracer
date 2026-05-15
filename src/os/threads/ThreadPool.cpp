/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ThreadPool
*/

#include "ThreadPool.hpp"
#include <algorithm>

namespace raytracer::os::threads {

ThreadPool::ThreadPool() {
  const auto detected = std::thread::hardware_concurrency();
  createWorkers(detected == 0 ? 1 : static_cast<std::size_t>(detected));
}

ThreadPool::ThreadPool(std::size_t workerCount) {
  createWorkers(std::max<std::size_t>(workerCount, 1));
}

ThreadPool::~ThreadPool() { shutdown(); }

void ThreadPool::createWorkers(std::size_t workerCount) {
  workers_.reserve(workerCount);
  for (std::size_t index = 0; index < workerCount; ++index) {
    workers_.emplace_back([this] { workerLoop(); });
  }
}

void ThreadPool::workerLoop() {
  while (true) {
    Task task;
    const auto status = taskQueue_.waitPop(task);
    if (status != ::raytracer::common::QueueStatus::Success) {
      return;
    }

    task();

    if (inFlightTasks_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
      std::lock_guard<std::mutex> lock(idleMutex_);
      idleCondition_.notify_all();
    }
  }
}

void ThreadPool::waitIdle() {
  std::unique_lock<std::mutex> lock(idleMutex_);
  idleCondition_.wait(lock, [this] {
    return inFlightTasks_.load(std::memory_order_acquire) == 0;
  });
}

void ThreadPool::shutdown() {
  if (!running_.exchange(false, std::memory_order_acq_rel)) {
    return;
  }
  taskQueue_.close();
  for (auto& worker : workers_) {
    if (worker.joinable()) {
      worker.join();
    }
  }
  workers_.clear();
}

std::size_t ThreadPool::workerCount() const noexcept { return workers_.size(); }

std::size_t ThreadPool::pendingTaskCount() const { return taskQueue_.size(); }

std::size_t ThreadPool::inFlightTaskCount() const noexcept {
  return inFlightTasks_.load(std::memory_order_acquire);
}

bool ThreadPool::isRunning() const noexcept {
  return running_.load(std::memory_order_acquire);
}

}  // namespace raytracer::os::threads
