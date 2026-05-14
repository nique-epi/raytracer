/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Logger
*/

#include "Logger.hpp"

#include <unistd.h>

#include <algorithm>
#include <atomic>
#include <cctype>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>

namespace {

std::mutex& outputMutex() {
  static std::mutex mutex;
  return mutex;
}

std::string toLower(std::string_view input) {
  std::string out(input);
  std::ranges::transform(out, out.begin(),
                         [](unsigned char chr) { return std::tolower(chr); });
  return out;
}

raytracer::common::LogLevel parseLevel(std::string_view text,
                                       raytracer::common::LogLevel fallback) {
  using raytracer::common::LogLevel;
  const std::string lower = toLower(text);
  if (lower == "trace") {
    return LogLevel::Trace;
  }
  if (lower == "debug") {
    return LogLevel::Debug;
  }
  if (lower == "info") {
    return LogLevel::Info;
  }
  if (lower == "warn" || lower == "warning") {
    return LogLevel::Warn;
  }
  if (lower == "error") {
    return LogLevel::Error;
  }
  if (lower == "silent" || lower == "off" || lower == "none") {
    return LogLevel::Silent;
  }
  return fallback;
}

raytracer::common::LogLevel initialLevel() {
  // NOLINTNEXTLINE(concurrency-mt-unsafe) — read once at first init.
  const char* envValue = std::getenv("RT_LOG_LEVEL");
  if (envValue == nullptr) {
    return raytracer::common::LogLevel::Info;
  }
  return parseLevel(envValue, raytracer::common::LogLevel::Info);
}

std::atomic<raytracer::common::LogLevel>& currentLevel() {
  static std::atomic<raytracer::common::LogLevel> level{initialLevel()};
  return level;
}

bool computeColorEnabled() {
  // NOLINTNEXTLINE(concurrency-mt-unsafe) — read once at first init.
  if (std::getenv("NO_COLOR") != nullptr) {
    return false;
  }
  // NOLINTNEXTLINE(concurrency-mt-unsafe) — read once at first init.
  if (std::getenv("RT_LOG_NO_COLOR") != nullptr) {
    return false;
  }
  return ::isatty(::fileno(stderr)) != 0;
}

bool colorEnabled() {
  static const bool enabled = computeColorEnabled();
  return enabled;
}

constexpr std::string_view ansiReset = "\x1b[0m";

std::string_view ansiForLevel(raytracer::common::LogLevel level) {
  using raytracer::common::LogLevel;
  switch (level) {
    case LogLevel::Trace:
      return "\x1b[90m";  // bright black / gray
    case LogLevel::Debug:
      return "\x1b[36m";  // cyan
    case LogLevel::Info:
      return "\x1b[32m";  // green
    case LogLevel::Warn:
      return "\x1b[33m";  // yellow
    case LogLevel::Error:
      return "\x1b[31m";  // red
    case LogLevel::Silent:
      return {};
  }
  return {};
}

std::string_view labelForLevel(raytracer::common::LogLevel level) {
  using raytracer::common::LogLevel;
  switch (level) {
    case LogLevel::Trace:
      return "TRACE";
    case LogLevel::Debug:
      return "DEBUG";
    case LogLevel::Info:
      return "INFO ";
    case LogLevel::Warn:
      return "WARN ";
    case LogLevel::Error:
      return "ERROR";
    case LogLevel::Silent:
      return "SILENT";
  }
  return "?????";
}

std::string formatTimestamp() {
  using std::chrono::duration_cast;
  using std::chrono::milliseconds;
  using std::chrono::system_clock;
  using std::chrono::time_point_cast;
  const auto now = system_clock::now();
  const auto seconds = time_point_cast<std::chrono::seconds>(now);
  const auto millis = duration_cast<milliseconds>(now - seconds).count();
  const std::time_t timeValue = system_clock::to_time_t(now);
  std::tm broken{};
#if defined(_WIN32)
  localtime_s(&broken, &timeValue);
#else
  (void)localtime_r(&timeValue, &broken);
#endif
  std::ostringstream out;
  out << std::put_time(&broken, "%H:%M:%S") << '.' << std::setw(3)
      << std::setfill('0') << millis;
  return out.str();
}

}  // namespace

namespace raytracer::common {

Logger::Logger(std::string module) : module_(std::move(module)) {}

LogLevel Logger::level() {
  return currentLevel().load(std::memory_order_acquire);
}

void Logger::setLevel(LogLevel newLevel) {
  currentLevel().store(newLevel, std::memory_order_release);
}

bool Logger::shouldLog(LogLevel candidate) {
  return static_cast<std::uint8_t>(candidate) >=
         static_cast<std::uint8_t>(level());
}

void Logger::writeLine(LogLevel candidate, std::string_view body) const {
  const std::string_view color =
      colorEnabled() ? ansiForLevel(candidate) : std::string_view{};
  const std::string_view reset =
      colorEnabled() ? ansiReset : std::string_view{};
  const std::string timestamp = formatTimestamp();

  std::lock_guard<std::mutex> lock(outputMutex());
  std::cerr << color << '[' << timestamp << "] [" << labelForLevel(candidate)
            << "] [" << module_ << "] - " << body << reset << '\n';
}

Logger::ScopedTimer Logger::scope(std::string label, LogLevel level) const {
  return {module_, std::move(label), level};
}

Logger::ScopedTimer::ScopedTimer(std::string module, std::string label,
                                 LogLevel level)
    : module_(std::move(module)),
      label_(std::move(label)),
      level_(level),
      begin_(std::chrono::steady_clock::now()),
      active_(true) {}

Logger::ScopedTimer::ScopedTimer(ScopedTimer&& other) noexcept
    : module_(std::move(other.module_)),
      label_(std::move(other.label_)),
      level_(other.level_),
      begin_(other.begin_),
      active_(other.active_) {
  other.active_ = false;
}

Logger::ScopedTimer::~ScopedTimer() {
  if (!active_) {
    return;
  }
  if (!Logger::shouldLog(level_)) {
    return;
  }
  const auto elapsed = std::chrono::steady_clock::now() - begin_;
  const auto microseconds =
      std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
  const double milliseconds = static_cast<double>(microseconds) / 1000.0;
  std::ostringstream out;
  out << label_ << " took " << std::fixed << std::setprecision(3)
      << milliseconds << " ms";

  const std::string_view color =
      colorEnabled() ? ansiForLevel(level_) : std::string_view{};
  const std::string_view reset =
      colorEnabled() ? ansiReset : std::string_view{};
  const std::string timestamp = formatTimestamp();

  std::lock_guard<std::mutex> lock(outputMutex());
  std::cerr << color << '[' << timestamp << "] [" << labelForLevel(level_)
            << "] [" << module_ << "] - " << out.str() << reset << '\n';
}

}  // namespace raytracer::common
