#pragma once

#include <cstdint>
#include <format>

namespace logg {

/**
 * @brief Log level. All logs with levels less than the current one are
 * discarded
 */
enum class level : std::uint8_t {
  kDebug = 0U,
  kInfo  = 1U,
  kWarn  = 2U,
  kError = 3U,
};

}  // namespace logg

template <>
class std::formatter<logg::level> {
 public:
  template <typename FormatContext>
  constexpr auto parse(FormatContext & ctx) const noexcept -> decltype(auto) {
    return ctx.begin();
  }

  template <typename FormatContext>
  constexpr auto format(logg::level const & level, FormatContext & ctx) const
      -> decltype(auto) {
    switch (level) {
      case logg::level::kDebug:
        return std::format_to(ctx.out(), "Debug");
      case logg::level::kInfo:
        return std::format_to(ctx.out(), "Info");
      case logg::level::kWarn:
        return std::format_to(ctx.out(), "Warn");
      case logg::level::kError:
        return std::format_to(ctx.out(), "Error");
    }
  }
};
