/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ThreadPool unit tests
*/

#include <gtest/gtest.h>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <future>
#include <stdexcept>
#include <thread>
#include <vector>

#include "os/threads/ThreadPool.hpp"

using raytracer::os::threads::ThreadPool;

TEST(ThreadPoolTest, DefaultConstructorUsesHardwareConcurrency) {
  ThreadPool pool;
  const auto expected = std::thread::hardware_concurrency();
  const std::size_t expectedCount = expected == 0 ? 1u : expected;
  EXPECT_EQ(pool.workerCount(), expectedCount);
  EXPECT_TRUE(pool.isRunning());
}

TEST(ThreadPoolTest, WorkerCountIsAtLeastOne) {
  ThreadPool pool(0);
  EXPECT_EQ(pool.workerCount(), 1u);
}

TEST(ThreadPoolTest, SubmitReturnsFutureWithResult) {
  ThreadPool pool(2);
  auto future = pool.submit([] { return 42; });
  EXPECT_EQ(future.get(), 42);
}

TEST(ThreadPoolTest, SubmitForwardsArguments) {
  ThreadPool pool(2);
  auto future = pool.submit([](int a, int b) { return a + b; }, 7, 35);
  EXPECT_EQ(future.get(), 42);
}

TEST(ThreadPoolTest, SubmitPropagatesExceptions) {
  ThreadPool pool(1);
  auto future = pool.submit([] {
    throw std::runtime_error("boom");
    return 0;
  });
  EXPECT_THROW(future.get(), std::runtime_error);
}

TEST(ThreadPoolTest, RunsTasksInParallel) {
  constexpr int taskCount = 16;
  ThreadPool pool(4);

  std::atomic<int> counter{0};
  std::vector<std::future<void>> futures;
  futures.reserve(taskCount);

  for (int i = 0; i < taskCount; ++i) {
    futures.emplace_back(pool.submit([&counter] {
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
      counter.fetch_add(1, std::memory_order_relaxed);
    }));
  }
  for (auto& future : futures) future.get();
  EXPECT_EQ(counter.load(), taskCount);
}

TEST(ThreadPoolTest, WaitIdleBlocksUntilAllTasksComplete) {
  ThreadPool pool(3);

  std::atomic<int> running{0};
  std::atomic<int> peak{0};
  std::atomic<int> finished{0};

  constexpr int taskCount = 20;
  for (int i = 0; i < taskCount; ++i) {
    pool.submit([&running, &peak, &finished] {
      const int now = running.fetch_add(1) + 1;
      int previous = peak.load();
      while (now > previous &&
             !peak.compare_exchange_weak(previous, now)) {}
      std::this_thread::sleep_for(std::chrono::milliseconds(3));
      running.fetch_sub(1);
      finished.fetch_add(1);
    });
  }

  pool.waitIdle();
  EXPECT_EQ(finished.load(), taskCount);
  EXPECT_EQ(pool.inFlightTaskCount(), 0u);
  EXPECT_EQ(pool.pendingTaskCount(), 0u);
  EXPECT_LE(peak.load(), 3);
  EXPECT_GE(peak.load(), 1);
}

TEST(ThreadPoolTest, WaitIdleOnEmptyPoolReturnsImmediately) {
  ThreadPool pool(2);
  const auto begin = std::chrono::steady_clock::now();
  pool.waitIdle();
  const auto elapsed = std::chrono::steady_clock::now() - begin;
  EXPECT_LT(elapsed, std::chrono::milliseconds(50));
}

TEST(ThreadPoolTest, SubmitAfterShutdownThrows) {
  ThreadPool pool(2);
  pool.shutdown();
  EXPECT_FALSE(pool.isRunning());
  EXPECT_THROW((void)pool.submit([] { return 1; }), std::runtime_error);
}

TEST(ThreadPoolTest, ShutdownIsIdempotent) {
  ThreadPool pool(2);
  pool.shutdown();
  EXPECT_NO_THROW(pool.shutdown());
  EXPECT_FALSE(pool.isRunning());
}

TEST(ThreadPoolTest, DestructorDrainsRemainingTasks) {
  std::atomic<int> finished{0};
  {
    ThreadPool pool(2);
    for (int i = 0; i < 32; ++i) {
      pool.submit([&finished] {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        finished.fetch_add(1);
      });
    }
  }
  EXPECT_EQ(finished.load(), 32);
}

TEST(ThreadPoolTest, SumPartitioning) {
  ThreadPool pool(4);

  constexpr int total = 10000;
  constexpr int chunkCount = 8;
  constexpr int chunkSize = total / chunkCount;

  std::vector<std::future<long long>> futures;
  futures.reserve(chunkCount);

  for (int chunk = 0; chunk < chunkCount; ++chunk) {
    futures.emplace_back(pool.submit(
        [chunk] {
          long long sum = 0;
          const int begin = chunk * chunkSize;
          const int end = begin + chunkSize;
          for (int i = begin; i < end; ++i) sum += i;
          return sum;
        }));
  }

  long long aggregated = 0;
  for (auto& future : futures) aggregated += future.get();

  long long expected = 0;
  for (int i = 0; i < total; ++i) expected += i;
  EXPECT_EQ(aggregated, expected);
}
