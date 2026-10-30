#pragma once

#include <format>
#include <utility>

#include "level.hpp"
#include "renderer.hpp"
#include "writer.hpp"

namespace logg {

/**
 * @brief Logger with customizable string rendering and destination
 * @note Log level is immutable. Thread safety is decided by the renderer and
 * writer implementations
 */
class logger {
 public:
  constexpr logger(level level, renderer renderer, writer writer) noexcept
      : level_(level),
        renderer_(std::move(renderer)),
        writer_(std::move(writer)) {}

 public:
  template <typename... Args>
  constexpr void operator()(level level, std::format_string<Args...> fmt,
                            Args const &... args) const {
    if (level_ <= level) {
      writer_(renderer_(fmt.get(), std::make_format_args(args...), level));
    }
  }

 private:
  level level_;
  renderer renderer_;
  writer writer_;
};

}  // namespace logg
