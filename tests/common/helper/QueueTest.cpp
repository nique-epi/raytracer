/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Queue unit tests
*/

#include <gtest/gtest.h>
#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

#include "common/helper/Queue.hpp"

using raytracer::common::Queue;
using raytracer::common::QueueStatus;

TEST(QueueTest, NewQueueIsEmptyAndOpen) {
  Queue<int> queue;
  EXPECT_TRUE(queue.isEmpty());
  EXPECT_FALSE(queue.isClosed());
  EXPECT_EQ(queue.size(), 0u);
}

TEST(QueueTest, PushIncreasesSize) {
  Queue<int> queue;
  EXPECT_TRUE(queue.push(1));
  EXPECT_TRUE(queue.push(2));
  EXPECT_EQ(queue.size(), 2u);
  EXPECT_FALSE(queue.isEmpty());
}

TEST(QueueTest, TryPopReturnsEmptyOnOpenEmptyQueue) {
  Queue<int> queue;
  int out = 0;
  EXPECT_EQ(queue.tryPop(out), QueueStatus::Empty);
}

TEST(QueueTest, TryPopExtractsInFifoOrder) {
  Queue<int> queue;
  queue.push(10);
  queue.push(20);
  queue.push(30);

  int out = 0;
  EXPECT_EQ(queue.tryPop(out), QueueStatus::Success);
  EXPECT_EQ(out, 10);
  EXPECT_EQ(queue.tryPop(out), QueueStatus::Success);
  EXPECT_EQ(out, 20);
  EXPECT_EQ(queue.tryPop(out), QueueStatus::Success);
  EXPECT_EQ(out, 30);
  EXPECT_EQ(queue.tryPop(out), QueueStatus::Empty);
}

TEST(QueueTest, PushOnClosedQueueIsRejected) {
  Queue<int> queue;
  queue.close();
  EXPECT_TRUE(queue.isClosed());
  EXPECT_FALSE(queue.push(42));
  EXPECT_TRUE(queue.isEmpty());
}

TEST(QueueTest, WaitPopReturnsClosedWhenQueueIsClosedAndEmpty) {
  Queue<int> queue;
  queue.close();
  int out = -1;
  EXPECT_EQ(queue.waitPop(out), QueueStatus::Closed);
  EXPECT_EQ(out, -1);
}

TEST(QueueTest, WaitPopDrainsBeforeReportingClosed) {
  Queue<int> queue;
  queue.push(7);
  queue.close();

  int out = 0;
  EXPECT_EQ(queue.waitPop(out), QueueStatus::Success);
  EXPECT_EQ(out, 7);
  EXPECT_EQ(queue.waitPop(out), QueueStatus::Closed);
}

TEST(QueueTest, WaitPopUnblocksOnClose) {
  Queue<int> queue;
  std::atomic<bool> consumerStarted{false};
  std::atomic<bool> consumerDone{false};

  std::thread consumer([&] {
    consumerStarted.store(true);
    int out = 0;
    EXPECT_EQ(queue.waitPop(out), QueueStatus::Closed);
    consumerDone.store(true);
  });

  while (!consumerStarted.load()) {
    std::this_thread::yield();
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(20));
  queue.close();
  consumer.join();
  EXPECT_TRUE(consumerDone.load());
}

TEST(QueueTest, MultiProducerMultiConsumerPreservesAllItems) {
  constexpr int producerCount = 4;
  constexpr int consumerCount = 4;
  constexpr int itemsPerProducer = 250;

  Queue<int> queue;
  std::atomic<int> consumedSum{0};
  std::atomic<int> consumedCount{0};

  std::vector<std::thread> consumers;
  consumers.reserve(consumerCount);
  for (int i = 0; i < consumerCount; ++i) {
    consumers.emplace_back([&] {
      int value = 0;
      while (queue.waitPop(value) == QueueStatus::Success) {
        consumedSum.fetch_add(value, std::memory_order_relaxed);
        consumedCount.fetch_add(1, std::memory_order_relaxed);
      }
    });
  }

  std::vector<std::thread> producers;
  producers.reserve(producerCount);
  for (int producer = 0; producer < producerCount; ++producer) {
    producers.emplace_back([&queue, producer] {
      for (int i = 0; i < itemsPerProducer; ++i) {
        queue.push(producer * itemsPerProducer + i);
      }
    });
  }

  for (auto& thread : producers) thread.join();
  queue.close();
  for (auto& thread : consumers) thread.join();

  const int expectedCount = producerCount * itemsPerProducer;
  int expectedSum = 0;
  for (int i = 0; i < expectedCount; ++i) expectedSum += i;

  EXPECT_EQ(consumedCount.load(), expectedCount);
  EXPECT_EQ(consumedSum.load(), expectedSum);
}
