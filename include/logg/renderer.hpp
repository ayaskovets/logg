#pragma once

#include <format>
#include <string_view>

#include "level.hpp"

namespace logg {

/**
 * @brief Log message decorator and formatter. Responsible for rendering format
 * string and arguments into a string buffer
 * @note The rendering function is free to use the buffer anyhow but is required
 * to return a view containing the formatted message
 */
class renderer final {
 public:
  /**
   * @brief Make a no-op renderer which works transparently and does not affect
   * the format string in any way
   */
  static auto kNoop() noexcept -> renderer;

  /**
   * @brief Make a renderer that adds the colored level of a printed log at the
   * beginning of the format string
   * @note Newline is automatically appended at the end
   */
  static auto kColoredLeveled() noexcept -> renderer;

  /**
   * @brief Make a renderer that adds the colored level of a printed log and
   * current system_clock timestamp at the beginning of the format string
   * @note Newline is automatically appended at the end
   */
  static auto kColoredLeveledTimestamped() noexcept -> renderer;

 private:
  using render_t = std::string_view (*)(std::string & buffer, level level,
                                        std::string_view fmt,
                                        std::format_args args,
                                        std::size_t args_size);

 public:
  explicit constexpr renderer(render_t render) noexcept : render_(render) {}

 public:
  constexpr auto operator()(std::string & buffer, level level,
                            std::string_view fmt, std::format_args args,
                            std::size_t args_size) const -> std::string_view {
    return render_(buffer, level, fmt, args, args_size);
  }

 private:
  render_t render_;
};

}  // namespace logg
