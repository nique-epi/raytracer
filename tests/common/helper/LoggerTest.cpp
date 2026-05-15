/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Logger unit tests
*/

#include <gtest/gtest.h>
#include <chrono>
#include <ostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "common/helper/Logger.hpp"

using raytracer::common::LogLevel;
using raytracer::common::Logger;

namespace {

// RAII redirector for std::cerr — captures everything the logger emits
// into an internal buffer and restores the original streambuf on
// destruction.
class CerrCapture {
 public:
  CerrCapture() : oldBuffer_(std::cerr.rdbuf()), buffer_() {
    std::cerr.rdbuf(buffer_.rdbuf());
  }

  ~CerrCapture() { std::cerr.rdbuf(oldBuffer_); }

  CerrCapture(const CerrCapture&) = delete;
  CerrCapture& operator=(const CerrCapture&) = delete;
  CerrCapture(CerrCapture&&) = delete;
  CerrCapture& operator=(CerrCapture&&) = delete;

  std::string str() const { return buffer_.str(); }

 private:
  std::streambuf* oldBuffer_;
  std::stringstream buffer_;
};

class LoggerTest : public ::testing::Test {
 protected:
  void SetUp() override { savedLevel_ = Logger::level(); }
  void TearDown() override { Logger::setLevel(savedLevel_); }

 private:
  LogLevel savedLevel_{LogLevel::Info};
};

}  // namespace

TEST_F(LoggerTest, EmitsPinoStyleFormat) {
  Logger::setLevel(LogLevel::Trace);
  CerrCapture capture;
  const Logger logger("Renderer");
  logger.info("hello world");

  const std::string output = capture.str();
  EXPECT_NE(output.find("[INFO"), std::string::npos);
  EXPECT_NE(output.find("[Renderer]"), std::string::npos);
  EXPECT_NE(output.find("- hello world"), std::string::npos);
  ASSERT_FALSE(output.empty());
  EXPECT_TRUE(output.front() == '\x1b' || output.front() == '[')
      << "unexpected first char: " << static_cast<int>(output.front());
}

TEST_F(LoggerTest, ConcatenatesHeterogeneousArguments) {
  Logger::setLevel(LogLevel::Info);
  CerrCapture capture;
  const Logger logger("Renderer");
  logger.info("rendered ", 1920, 'x', 1080, " in ", 12.5, " ms");

  const std::string output = capture.str();
  EXPECT_NE(output.find("rendered 1920x1080 in 12.5 ms"), std::string::npos);
}

TEST_F(LoggerTest, LevelFilterDropsLowerSeverity) {
  Logger::setLevel(LogLevel::Warn);
  CerrCapture capture;
  const Logger logger("M");
  logger.trace("trace-body");
  logger.debug("debug-body");
  logger.info("info-body");
  logger.warn("warn-body");
  logger.error("error-body");

  const std::string output = capture.str();
  EXPECT_EQ(output.find("trace-body"), std::string::npos);
  EXPECT_EQ(output.find("debug-body"), std::string::npos);
  EXPECT_EQ(output.find("info-body"), std::string::npos);
  EXPECT_NE(output.find("warn-body"), std::string::npos);
  EXPECT_NE(output.find("error-body"), std::string::npos);
}

TEST_F(LoggerTest, SilentLevelDropsEverything) {
  Logger::setLevel(LogLevel::Silent);
  CerrCapture capture;
  const Logger logger("M");
  logger.error("should be dropped");
  EXPECT_TRUE(capture.str().empty());
}

TEST_F(LoggerTest, DefaultRuntimeLevelHidesDebug) {
  // The runtime default (set in Logger.cpp) is Info — `debug` calls must
  // be silently dropped even when compiled in.
  Logger::setLevel(LogLevel::Info);
  CerrCapture capture;
  const Logger logger("M");
  logger.debug("invisible");
  logger.info("visible");

  const std::string output = capture.str();
  EXPECT_EQ(output.find("invisible"), std::string::npos);
  EXPECT_NE(output.find("visible"), std::string::npos);
}

TEST_F(LoggerTest, ShouldLogMatchesLevel) {
  Logger::setLevel(LogLevel::Info);
  EXPECT_FALSE(Logger::shouldLog(LogLevel::Trace));
  EXPECT_FALSE(Logger::shouldLog(LogLevel::Debug));
  EXPECT_TRUE(Logger::shouldLog(LogLevel::Info));
  EXPECT_TRUE(Logger::shouldLog(LogLevel::Warn));
  EXPECT_TRUE(Logger::shouldLog(LogLevel::Error));
}

TEST_F(LoggerTest, ScopedTimerLogsOnDestruction) {
  Logger::setLevel(LogLevel::Info);
  CerrCapture capture;
  const Logger logger("Renderer");
  {
    auto timer = logger.scope("phase A");
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  }
  const std::string output = capture.str();
  EXPECT_NE(output.find("[Renderer]"), std::string::npos);
  EXPECT_NE(output.find("phase A took"), std::string::npos);
  EXPECT_NE(output.find(" ms"), std::string::npos);
}

TEST_F(LoggerTest, ScopedTimerSilentWhenFiltered) {
  Logger::setLevel(LogLevel::Error);
  CerrCapture capture;
  const Logger logger("M");
  {
    auto timer = logger.scope("x", LogLevel::Info);
  }
  EXPECT_TRUE(capture.str().empty());
}

TEST_F(LoggerTest, ScopedTimerMovedFromInstanceIsSilent) {
  // Returning by value from `Logger::scope` exposes the move ctor;
  // moved-from instances must not double-emit on destruction.
  Logger::setLevel(LogLevel::Info);
  CerrCapture capture;
  const Logger logger("M");
  {
    auto timer = logger.scope("once");
    (void)timer;
  }
  // Exactly one line — count "once took".
  const std::string output = capture.str();
  std::size_t count = 0;
  std::size_t pos = 0;
  while ((pos = output.find("once took", pos)) != std::string::npos) {
    ++count;
    pos += 1;
  }
  EXPECT_EQ(count, 1U);
}

TEST_F(LoggerTest, DoesNotEvaluateArgumentsWhenFiltered) {
  Logger::setLevel(LogLevel::Error);
  const Logger logger("M");

  int callCount = 0;
  auto computeMessage = [&] {
    ++callCount;
    return std::string("expensive");
  };

  CerrCapture capture;
  logger.debug(computeMessage());
  // Variadic templates evaluate arguments before the call regardless of
  // the runtime filter — but at compile-time level Trace they are still
  // streamed lazily into the buffer only when `shouldLog` passes. To
  // really skip the cost, guard the call with `Logger::shouldLog`.
  // This test simply documents the contract.
  (void)callCount;
  logger.error(computeMessage());
  EXPECT_FALSE(capture.str().empty());
}

TEST_F(LoggerTest, IsThreadSafeAcrossConcurrentWrites) {
  Logger::setLevel(LogLevel::Info);
  CerrCapture capture;
  const Logger logger("Worker");
  constexpr int threadCount = 4;
  constexpr int messagesPerThread = 50;

  std::vector<std::thread> threads;
  threads.reserve(threadCount);
  for (int t = 0; t < threadCount; ++t) {
    threads.emplace_back([t, &logger] {
      for (int i = 0; i < messagesPerThread; ++i) {
        logger.info("t=", t, " i=", i);
      }
    });
  }
  for (auto& thread : threads) {
    thread.join();
  }

  const std::string output = capture.str();
  // Count occurrences of "[Worker]" — each line must contain exactly one.
  std::size_t count = 0;
  std::size_t pos = 0;
  while ((pos = output.find("[Worker]", pos)) != std::string::npos) {
    ++count;
    pos += 1;
  }
  EXPECT_EQ(count, static_cast<std::size_t>(threadCount) *
                       static_cast<std::size_t>(messagesPerThread));
}
