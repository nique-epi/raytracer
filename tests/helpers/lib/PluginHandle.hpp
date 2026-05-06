/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PluginHandle
*/

#pragma once

#include <dlfcn.h>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <utility>

namespace raytracer::tests::helpers {

/**
 * @brief RAII wrapper around a `dlopen` handle.
 *
 * Loads a shared library on construction with `RTLD_NOW | RTLD_LOCAL`
 * and releases it on destruction. Move-only because the underlying
 * handle cannot be duplicated.
 *
 * Usage:
 * @code
 *   auto path = findPlugin("ambient.so");
 *   PluginHandle plugin(path);
 *   auto createFn = plugin.resolve<ILight* (*)()>("createLight");
 *   ILight* light = createFn();
 * @endcode
 */
class PluginHandle {
 public:
  /**
   * @brief Open the shared library located at @p path.
   *
   * @param [in] path Absolute or relative path to the .so file.
   * @throws std::runtime_error If the file does not exist or `dlopen`
   *         fails (the dlerror() message is appended).
   */
  explicit PluginHandle(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
      throw std::runtime_error("Plugin not found: " + path.string());
    }
    (void)dlerror();
    handle_ = dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
    if (handle_ == nullptr) {
      throw std::runtime_error(
          formatError("Failed to open " + path.string(), dlerror()));
    }
  }

  ~PluginHandle() {
    if (handle_ != nullptr) {
      dlclose(handle_);
    }
  }

  PluginHandle(const PluginHandle&) = delete;
  PluginHandle& operator=(const PluginHandle&) = delete;

  PluginHandle(PluginHandle&& other) noexcept
      : handle_(std::exchange(other.handle_, nullptr)) {}

  PluginHandle& operator=(PluginHandle&& other) noexcept {
    if (this != &other) {
      if (handle_ != nullptr) {
        dlclose(handle_);
      }
      handle_ = std::exchange(other.handle_, nullptr);
    }
    return *this;
  }

  /**
   * @brief Resolve @p symbolName from the loaded library and cast it
   *        to the function pointer type @p Fn.
   *
   * @tparam Fn Function pointer type expected for the symbol.
   * @param [in] symbolName Name of the symbol to look up.
   * @returns The resolved function pointer.
   * @throws std::runtime_error If the symbol cannot be resolved.
   */
  template <typename Fn>
  Fn resolve(const char* symbolName) const {
    (void)dlerror();
    void* symbol = dlsym(handle_, symbolName);
    const char* err = dlerror();
    if (err != nullptr || symbol == nullptr) {
      throw std::runtime_error(
          formatError(std::string("Failed to resolve ") + symbolName, err));
    }
    return reinterpret_cast<Fn>(symbol);
  }

  /**
   * @brief Raw `dlopen` handle, for callers that need to interact with
   *        the dl* API directly.
   */
  [[nodiscard]] void* native() const noexcept { return handle_; }

 private:
  static std::string formatError(const std::string& prefix, const char* err) {
    if (err == nullptr) {
      return prefix;
    }
    return prefix + ": " + err;
  }

  void* handle_ = nullptr;
};

/**
 * @brief Resolve a plugin file located in the test's current working
 *        directory.
 *
 * Plugin tests run from a build directory into which the .so is copied
 * via a `POST_BUILD` custom command. This helper just verifies that the
 * file is present and returns its absolute path.
 *
 * @param [in] fileName Plugin file name (e.g. "ambient.so").
 * @returns Absolute path to the plugin file.
 * @throws std::runtime_error If the file is not present.
 */
inline std::filesystem::path findPlugin(const std::string& fileName) {
  const auto candidate = std::filesystem::current_path() / fileName;
  if (!std::filesystem::exists(candidate)) {
    throw std::runtime_error("Could not find " + fileName);
  }
  return candidate;
}

}  // namespace raytracer::tests::helpers
