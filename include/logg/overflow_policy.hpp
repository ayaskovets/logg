#pragma once

#include <cstdint>
#include <format>

namespace logg {

/**
 * @brief Overflow policy for logging methods for specifying fallback action in
 * case logging sink is unavailable or full
 */
enum class overflow_policy : std::uint8_t {
  kDrop  = 0U,
  kBlock = 1U,
};

}  // namespace logg

template <>
class std::formatter<logg::overflow_policy> {
 public:
  template <typename FormatContext>
  constexpr auto parse(FormatContext & ctx) const noexcept -> decltype(auto) {
    return ctx.begin();
  }

  template <typename FormatContext>
  constexpr auto format(logg::overflow_policy const & overflow_policy,
                        FormatContext & ctx) const -> decltype(auto) {
    switch (overflow_policy) {
      case logg::overflow_policy::kDrop:
        return std::format_to(ctx.out(), "Drop");
      case logg::overflow_policy::kBlock:
        return std::format_to(ctx.out(), "Block");
    }
  }
};
