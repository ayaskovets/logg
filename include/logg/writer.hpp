#pragma once

#include <filesystem>
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
   * @brief Make a writer that does nothing with the string to be printed
   */
  static auto kNull() noexcept -> writer;

  /**
   * @brief Make a writer that prints to stdout
   */
  static auto kStdout() noexcept -> writer;

  /**
   * @brief Make writer that prints to stderr
   */
  static auto kStderr() noexcept -> writer;

  /**
   * @brief Make a writer that writes to a std::ostream instance
   */
  static auto kStream(std::ostream & ostream) noexcept -> writer;

  /**
   * @brief Make a writer that opens a file and writes to it
   * @note Throws an exception in case file can not be opened
   */
  static auto kPath(std::filesystem::path const & path) -> writer;

 private:
  using write_t = std::function<void(std::string_view log)>;

 public:
  explicit constexpr writer(write_t write) noexcept
      : write_(std::move(write)) {}

 public:
  constexpr void operator()(std::string_view log) const { write_(log); }

 private:
  write_t write_;
};

}  // namespace logg
