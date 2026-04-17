#pragma once

#include <logg/level.hpp>
#include <logg/renderer.hpp>
#include <logg/writer.hpp>

#include <format>
#include <utility>

namespace logg {

/**
 * @brief Logger with customizable string rendering and destination
 * @note Log level is immutable. The logger is generally thread-safe, but this
 * is dependent on the implementation of the writer
 */
class logger {
 private:
  /**
   * @note Underlying TLS buffer size always stays in range
   * [kMinimumBufferCapacity, kMaximumBufferCapacity] after any single log but
   * can be temporarily resized by the renderer
   */
  static constexpr std::size_t kMaximumBufferCapacity = 4096;
  static constexpr std::size_t kMinimumBufferCapacity = 1024;

 public:
  constexpr logger(level level, renderer renderer, writer writer) noexcept
      : level_(level), renderer_(renderer), writer_(std::move(writer)) {}

 public:
  template <typename... Args>
  constexpr void operator()(level level, std::format_string<Args...> fmt,
                            Args const &... args) const {
    if (level_ > level) [[unlikely]] {
      return;
    }

    thread_local std::string buffer;
    buffer.clear();

    writer_(renderer_(buffer, level, fmt.get(), std::make_format_args(args...),
                      sizeof...(Args)));

    if (buffer.size() > kMaximumBufferCapacity) [[unlikely]] {
      buffer = std::string();
      buffer.reserve(kMinimumBufferCapacity);
    }
  }

 protected:
  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  // NOLINTBEGIN(cppcoreguidelines-avoid-const-or-ref-data-members)
  level const level_;
  renderer const renderer_;
  writer const writer_;
  // NOLINTEND(cppcoreguidelines-avoid-const-or-ref-data-members)
  // NOLINTEND(misc-non-private-member-variables-in-classes)
};

}  // namespace logg
