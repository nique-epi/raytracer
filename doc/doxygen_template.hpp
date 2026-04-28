/**
 * @file doxygen_template.hpp
 * @brief Reference template for Doxygen-compliant C++ documentation.
 *
 * This file is a copy-paste reference. It covers every entity type used in
 * this project: files, namespaces, classes, interfaces, structs, enums,
 * free functions, templates, and operators.
 *
 * Rules enforced across this project:
 *  - Block style  : /** ... *\/ (Javadoc), NOT ///
 *  - Command style: @tag, NOT \tag
 *  - Brief style  : imperative mood, one line ("Compute X", not "Computes X")
 *  - @returns     : NOT @return
 *  - @throws      : NOT @throw / @exception
 *  - @param       : always with direction [in], [out], or [in,out]
 *  - @tparam      : name must match the template parameter name exactly
 *  - No duplicate docs in overrides — use @inheritDoc or add context only
 *
 * @date    2026-04-28
 * @version 1.0
 * @copyright (C) 2026 Nique-Epi ;)
 */

#ifndef DOC_DOXYGEN_TEMPLATE_HPP_
#define DOC_DOXYGEN_TEMPLATE_HPP_

#include <cstddef>
#include <stdexcept>
#include <string>


// ─────────────────────────────────────────────────────────────────────────────
// NAMESPACE
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Example namespace grouping related utilities.
 *
 * Detailed description goes here when the one-liner is not enough.
 * Leave this section out when the brief is self-explanatory.
 */
namespace Example {


// ─────────────────────────────────────────────────────────────────────────────
// ENUM
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Axis identifiers for 3-D space.
 */
enum class Axis {
  X = 0, /**< Horizontal axis. */
  Y = 1, /**< Vertical axis.   */
  Z = 2, /**< Depth axis.      */
};


// ─────────────────────────────────────────────────────────────────────────────
// STRUCT (plain data)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Immutable pair of min/max bounds along one axis.
 */
struct Interval {
  double min; /**< Lower bound (inclusive). */
  double max; /**< Upper bound (inclusive). */
};


// ─────────────────────────────────────────────────────────────────────────────
// PURE INTERFACE
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Contract for objects that can be serialised to a string.
 *
 * Any class implementing this interface must produce a human-readable
 * representation via toString().
 *
 * @note Implement @ref toString() in a way that is safe to call from
 *       multiple threads simultaneously.
 */
class IStringifiable {
 public:
  virtual ~IStringifiable() = default;

  /**
   * @brief Serialise this object to a human-readable string.
   *
   * @returns A non-empty string representation of the object.
   */
  virtual std::string toString() const = 0;
};


// ─────────────────────────────────────────────────────────────────────────────
// CONCRETE CLASS
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Fixed-size ring buffer with a configurable element type.
 *
 * Elements are stored in a contiguous array; push() overwrites the oldest
 * element when the buffer is full.
 *
 * @tparam T    Element type; must be CopyAssignable and DefaultConstructible.
 * @tparam N    Maximum number of elements; must be greater than zero.
 *
 * @see IStringifiable for the toString() contract.
 *
 * @warning Not thread-safe. Protect concurrent accesses with an external lock.
 */
template <typename T, std::size_t N>
class RingBuffer : public IStringifiable {
 public:
  // ── Construction ───────────────────────────────────────────────────────────

  /**
   * @brief Construct an empty ring buffer.
   */
  RingBuffer();

  /**
   * @brief Construct a ring buffer pre-filled with @p value.
   *
   * @param [in] value Initial value copied into every slot.
   */
  explicit RingBuffer(const T& value);

  // ── Capacity ───────────────────────────────────────────────────────────────

  /**
   * @brief Return the maximum number of elements this buffer can hold.
   *
   * @returns Capacity N passed as template argument.
   */
  std::size_t capacity() const;

  /**
   * @brief Return the number of elements currently stored.
   *
   * @returns A value in [0, N].
   */
  std::size_t size() const;

  /**
   * @brief Check whether the buffer contains no elements.
   *
   * @returns true if size() == 0.
   */
  bool empty() const;

  // ── Modifiers ──────────────────────────────────────────────────────────────

  /**
   * @brief Insert an element at the back of the buffer.
   *
   * When the buffer is full the oldest element is silently overwritten.
   *
   * @param [in] value Element to insert (copied).
   */
  void push(const T& value);

  /**
   * @brief Remove and return the oldest element.
   *
   * @returns The element that was at the front of the buffer.
   *
   * @throws std::underflow_error if the buffer is empty.
   *
   * @pre size() > 0
   */
  T pop();

  // ── Element access ─────────────────────────────────────────────────────────

  /**
   * @brief Access the element at logical index @p i (read-only).
   *
   * @param [in] i Zero-based index, where 0 is the oldest element.
   * @returns Const reference to the element.
   *
   * @throws std::out_of_range if i >= size().
   */
  const T& at(std::size_t i) const;

  // ── IStringifiable ─────────────────────────────────────────────────────────

  /**
   * @brief Serialise the buffer contents to a bracketed comma-separated list.
   *
   * Example output: `[1, 2, 3]` (oldest first).
   *
   * @returns A non-empty string representation.
   *
   * @code
   *   RingBuffer<int, 4> buf;
   *   buf.push(1);
   *   buf.push(2);
   *   std::cout << buf.toString(); // "[1, 2]"
   * @endcode
   */
  std::string toString() const override;

 private:
  T           data_[N];
  std::size_t head_{0};
  std::size_t tail_{0};
  std::size_t size_{0};
};


// ─────────────────────────────────────────────────────────────────────────────
// FREE FUNCTION
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Clamp @p value to the range [@p lo, @p hi].
 *
 * @tparam T Numeric type; must support operator< and be CopyConstructible.
 *
 * @param [in] value Value to clamp.
 * @param [in] lo    Lower bound (inclusive).
 * @param [in] hi    Upper bound (inclusive).
 * @returns @p lo if value < lo, @p hi if value > hi, otherwise @p value.
 *
 * @pre lo <= hi
 *
 * @note Equivalent to std::clamp (C++17) but available in C++14 contexts.
 */
template <typename T>
T clamp(T value, T lo, T hi);

/**
 * @brief Linearly interpolate between @p a and @p b by factor @p t.
 *
 * @param [in] a Start value (t = 0).
 * @param [in] b End   value (t = 1).
 * @param [in] t Interpolation factor; not clamped to [0, 1].
 * @returns a + t * (b - a)
 */
double lerp(double a, double b, double t);


// ─────────────────────────────────────────────────────────────────────────────
// OPERATOR (associated with a class via @relates)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Write a string-serialisable object to an output stream.
 *
 * @param [in,out] os  Output stream.
 * @param [in]     obj Object to serialise.
 * @returns Reference to @p os to enable chaining.
 *
 * @relates IStringifiable
 */
std::ostream& operator<<(std::ostream& os, const IStringifiable& obj);


// ─────────────────────────────────────────────────────────────────────────────
// GROUPING EXAMPLE
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @defgroup math_utils Math utilities
 * @brief Standalone numeric helpers used throughout the renderer.
 * @{
 */

/**
 * @brief Convert degrees to radians.
 *
 * @param [in] degrees Angle in degrees.
 * @returns Equivalent angle in radians.
 *
 * @ingroup math_utils
 */
double toRadians(double degrees);

/**
 * @brief Compute the square of @p x.
 *
 * @tparam T Numeric type supporting operator*.
 * @param [in] x Input value.
 * @returns x * x
 *
 * @ingroup math_utils
 */
template <typename T>
T square(T x);

/** @} */  // end of math_utils


// ─────────────────────────────────────────────────────────────────────────────
// DEPRECATION / TODO / WARNING / NOTE EXAMPLES
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Compute the dot product of two 3-D arrays.
 *
 * @param [in] a First  array (must have at least 3 elements).
 * @param [in] b Second array (must have at least 3 elements).
 * @returns Scalar dot product.
 *
 * @deprecated Use Math::Vector3D::dot() instead.
 *
 * @warning @p a and @p b must point to at least 3 elements; no bounds check
 *          is performed.
 *
 * @note Retained for backward compatibility with the legacy C API.
 *
 * @todo Remove once all callers have migrated to Vector3D.
 */
double legacyDot(const double* a, const double* b);


}  // namespace Example

#endif  // DOC_DOXYGEN_TEMPLATE_HPP_
