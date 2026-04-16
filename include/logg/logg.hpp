#pragma once

#include <logg/level.hpp>
#include <logg/logger.hpp>

namespace logg {

template <typename... Args>
constexpr void debug(std::format_string<Args...> fmt, Args &&... args) {
  logg::logger::kDefault()(logg::level::kDebug, fmt,
                           std::forward<Args>(args)...);
}

template <typename... Args>
constexpr void info(std::format_string<Args...> fmt, Args &&... args) {
  logg::logger::kDefault()(logg::level::kInfo, fmt,
                           std::forward<Args>(args)...);
}

template <typename... Args>
constexpr void warning(std::format_string<Args...> fmt, Args &&... args) {
  logg::logger::kDefault()(logg::level::kWarn, fmt,
                           std::forward<Args>(args)...);
}

template <typename... Args>
constexpr void error(std::format_string<Args...> fmt, Args &&... args) {
  logg::logger::kDefault()(logg::level::kError, fmt,
                           std::forward<Args>(args)...);
}

}  // namespace logg
