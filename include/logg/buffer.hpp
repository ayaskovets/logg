#pragma once

#include <string>
#include <variant>

#include <swiss/frozen_flat_set.hpp>

namespace logg {

/**
 * @brief Intermediate logging string buffer that output data is rendered to
 * @note Buffer getters are lazy to allow for customization of storage duration
 * of the underlying memory
 */
class buffer final {
 private:
  using bounds_type = swiss::frozen_flat_set<std::size_t, 2>;

 public:
  /**
   * @brief Make a buffer that shares single memory chunk across all its
   * instances
   */
  static auto kStatic() noexcept -> buffer;

  /**
   * @brief Make a buffer that holds a dedicated memory chunk for each thread
   */
  static auto kThreadLocal() noexcept -> buffer;

  /**
   * @brief Make a buffer that holds a dedicated memory chunk for the created
   * object
   */
  static auto kLocal(bounds_type bounds) noexcept -> buffer;

 public:
  /**
   * @brief Reset buffer capacity and set size to zero
   * @note Can reallocate memory
   */
  auto reset() -> std::string &;

  /**
   * @brief Set buffer [min, max] bounds
   * @note Can reallocate memory
   */
  void set_bounds(bounds_type bounds);

 private:
  using impl = std::pair<std::string, bounds_type>;

 private:
  explicit buffer(impl && local);
  explicit buffer(impl * shared) noexcept;

 private:
  auto visit() noexcept -> impl &;

 private:
  std::variant<impl, impl *> local_or_shared_data_;
};

}  // namespace logg
