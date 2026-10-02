#pragma once

#include <format>
#include <string_view>

#include "buffer.hpp"
#include "level.hpp"

namespace logg {

/**
 * @brief Log message decorator and formatter. Responsible for rendering format
 * string using the provided arguments
 */
class renderer final {
 public:
  /**
   * @brief Make a no-op renderer which works transparently and does not affect
   * the format string in any way
   */
  static auto kNoop(buffer buffer) noexcept -> renderer;

  /**
   * @brief Make a renderer that adds the level of a printed log at the
   * beginning of the format string
   * @note Newline is automatically appended at the end
   */
  static auto kLeveled(buffer buffer) noexcept -> renderer;

  /**
   * @brief Make a renderer that adds the colored level of a printed log at the
   * beginning of the format string
   * @note Newline is automatically appended at the end
   */
  static auto kColoredLeveled(buffer buffer) noexcept -> renderer;

  /**
   * @brief Make a renderer that adds the colored level of a printed log and
   * current system_clock timestamp at the beginning of the format string
   * @note Newline is automatically appended at the end
   */
  static auto kColoredLeveledTimestamped(buffer buffer) noexcept -> renderer;

 private:
  using render_type = std::string_view (*)(std::string_view fmt,
                                           std::format_args args,
                                           std::size_t args_size, level level,
                                           std::string & buffer);

 public:
  explicit constexpr renderer(render_type render, buffer buffer) noexcept
      : render_(render), buffer_(std::move(buffer)) {}

 public:
  constexpr auto operator()(std::string_view fmt, std::format_args args,
                            std::size_t args_size, level level) const
      -> std::string_view {
    return render_(fmt, args, args_size, level, buffer_.reset());
  }

 private:
  render_type render_;
  mutable buffer buffer_;
};

}  // namespace logg
