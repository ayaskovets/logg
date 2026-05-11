#pragma once

#include <format>
#include <utility>

#include "level.hpp"
#include "renderer.hpp"
#include "writer.hpp"

namespace logg {

/**
 * @brief Logger with customizable string rendering and destination
 * @note Log level is immutable. The logger is generally thread-safe, but this
 * is dependent on the implementation of the writer
 * @note Underlying TLS buffer size always stays in range [MaxBufferCapacity,
 * MinBufferCapacity] after any single log but can be temporarily resized by the
 * renderer
 */
// NOLINTNEXTBEGIN(cppcoreguidelines-avoid-magic-numbers)
template <std::size_t MaxBufferCapacity = 4096,
          std::size_t MinBufferCapacity = 1024>
// NOLINTNEXTEND(cppcoreguidelines-avoid-magic-numbers)
class logger {
 private:
  /**
   * @note Underlying TLS buffer size always stays in range
   * [kMinimumBufferCapacity, kMaximumBufferCapacity] after any single log but
   * can be temporarily resized by the renderer
   */

 public:
  constexpr logger(level level, renderer renderer, writer writer) noexcept
      : level_(level), renderer_(renderer), writer_(std::move(writer)) {}

 public:
  template <typename... Args>
  constexpr void operator()(level level, std::format_string<Args...> fmt,
                            Args const &... args) const {
    if (level_ > level) {
      return;
    }

    thread_local std::string buffer(MinBufferCapacity, ' ');
    buffer.clear();

    writer_(renderer_(buffer, level, fmt.get(), std::make_format_args(args...),
                      sizeof...(Args)));

    if (buffer.size() > MaxBufferCapacity) [[unlikely]] {
      buffer = std::string();
      buffer.reserve(MinBufferCapacity);
    }
  }

 private:
  // NOLINTBEGIN(cppcoreguidelines-avoid-const-or-ref-data-members)
  level const level_;
  renderer const renderer_;
  writer const writer_;
  // NOLINTEND(cppcoreguidelines-avoid-const-or-ref-data-members)
};

}  // namespace logg
