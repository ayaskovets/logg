#pragma once

#include <functional>
#include <ostream>
#include <string_view>

namespace logg {

/**
 * @brief Log message destination. Responsible for writing formatted log strings
 * into some destination (stdout, file, network socket, etc.)
 */
class writer final {
 public:
  /**
   * @brief Make writer that does nothing with the string to be printed
   */
  static auto kNoop() noexcept -> writer;

  /**
   * @brief Make writer that prints to stdout
   */
  static auto kStdout() noexcept -> writer;

  /**
   * @brief Make writer that prints to stderr
   */
  static auto kStderr() noexcept -> writer;

  /**
   * @brief Make writer that writes to a std::ostream instance
   */
  static auto kStream(std::ostream & ostream) noexcept -> writer;

 private:
  using write_t = std::function<void(std::string_view log)>;

 public:
  constexpr explicit writer(write_t write) noexcept
      : write_(std::move(write)) {}

 public:
  constexpr void operator()(std::string_view log) const { write_(log); }

 private:
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-const-or-ref-data-members)
  write_t const write_;
};

}  // namespace logg
