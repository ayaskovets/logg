#include <logg/renderer.hpp>

#include <chrono>
#include <string_view>

namespace logg {

namespace {

constexpr auto kRedColor    = "\x1B[31m";
constexpr auto kGreenColor  = "\x1B[32m";
constexpr auto kYellowColor = "\x1B[33m";
constexpr auto kNoColor     = "\x1B[0m";

constexpr auto kSetColor(level level) noexcept -> char const * {
  switch (level) {
    case level::kDebug:
      return "";
    case level::kInfo:
      return kGreenColor;
    case level::kWarn:
      return kYellowColor;
    case level::kError:
      return kRedColor;
  }
}

constexpr auto kResetColor(level level) noexcept -> char const * {
  switch (level) {
    case level::kDebug:
      return "";
    case level::kInfo:
    case level::kWarn:
    case level::kError:
      return kNoColor;
  }
}

}  // namespace

auto renderer::kNoop() noexcept -> renderer {
  render_t constexpr render = [](std::string & buffer, level,
                                 std::string_view fmt, std::format_args args,
                                 std::size_t) {
    std::size_t const prefix_size = buffer.size();
    std::vformat_to(std::back_inserter(buffer), fmt, args);

    return std::string_view(buffer).substr(prefix_size);
  };

  return renderer(render);
}

auto renderer::kColoredLeveled() noexcept -> renderer {
  render_t constexpr render = [](std::string & buffer, level level,
                                 std::string_view fmt, std::format_args args,
                                 std::size_t args_size) {
    std::size_t const prefix_size = buffer.size();
    std::format_to(std::back_inserter(buffer), "{}{}{}\t{}\n", kSetColor(level),
                   level, kResetColor(level), fmt);

    std::size_t const fmt_size = buffer.size() - prefix_size;
    if (args_size > 0) [[likely]] {
      std::vformat_to(std::back_inserter(buffer),
                      std::string_view(buffer).substr(prefix_size, fmt_size),
                      args);
      return std::string_view(buffer).substr(prefix_size + fmt_size);
    }

    return std::string_view(buffer).substr(prefix_size);
  };

  return renderer(render);
}

auto renderer::kColoredLeveledTimestamped() noexcept -> renderer {
  render_t constexpr render = [](std::string & buffer, level level,
                                 std::string_view fmt, std::format_args args,
                                 std::size_t args_size) {
    std::size_t const prefix_size = buffer.size();
    std::format_to(std::back_inserter(buffer), "{:%FT%T%z}\t{}{}{}\t{}\n",
                   std::chrono::system_clock::now(), kSetColor(level), level,
                   kResetColor(level), fmt);

    std::size_t const fmt_size = buffer.size() - prefix_size;
    if (args_size > 0) [[likely]] {
      std::vformat_to(std::back_inserter(buffer),
                      std::string_view(buffer).substr(prefix_size, fmt_size),
                      args);
      return std::string_view(buffer).substr(prefix_size + fmt_size);
    }

    return std::string_view(buffer).substr(prefix_size);
  };

  return renderer(render);
}

}  // namespace logg
