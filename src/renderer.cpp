#include <logg/renderer.hpp>

#include <chrono>
#include <iterator>
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

auto renderer::kNoop(buffer buffer) noexcept -> renderer {
  render_type constexpr render = [](std::string_view fmt, std::format_args args,
                                    level,
                                    std::string & buffer) -> std::string_view {
    std::size_t const prefix_size = buffer.size();
    std::vformat_to(std::inserter(buffer, buffer.end()), fmt, args);

    return std::string_view(buffer).substr(prefix_size);
  };

  return renderer(render, std::move(buffer));
}

auto renderer::kLeveled(buffer buffer) noexcept -> renderer {
  render_type constexpr render = [](std::string_view fmt, std::format_args args,
                                    level level,
                                    std::string & buffer) -> std::string_view {
    std::size_t const prefix_size = buffer.size();
    std::format_to(std::back_inserter(buffer), "{}\t", level);
    std::vformat_to(std::inserter(buffer, buffer.end()), fmt, args);
    buffer.push_back('\n');

    return std::string_view(buffer).substr(prefix_size);
  };

  return renderer(render, std::move(buffer));
}

auto renderer::kColoredLeveled(buffer buffer) noexcept -> renderer {
  render_type constexpr render = [](std::string_view fmt, std::format_args args,
                                    level level,
                                    std::string & buffer) -> std::string_view {
    std::size_t const prefix_size = buffer.size();
    std::format_to(std::back_inserter(buffer), "{}{}{}\t", kSetColor(level),
                   level, kResetColor(level));
    std::vformat_to(std::inserter(buffer, buffer.end()), fmt, args);
    buffer.push_back('\n');

    return std::string_view(buffer).substr(prefix_size);
  };

  return renderer(render, std::move(buffer));
}

auto renderer::kColoredLeveledTimestamped(buffer buffer) noexcept -> renderer {
  render_type constexpr render = [](std::string_view fmt, std::format_args args,
                                    level level,
                                    std::string & buffer) -> std::string_view {
    std::size_t const prefix_size = buffer.size();
    std::format_to(std::back_inserter(buffer), "{:%FT%T%z}\t{}{}{}\t",
                   std::chrono::system_clock::now(), kSetColor(level), level,
                   kResetColor(level));
    std::vformat_to(std::inserter(buffer, buffer.end()), fmt, args);
    buffer.push_back('\n');

    return std::string_view(buffer).substr(prefix_size);
  };

  return renderer(render, std::move(buffer));
}

}  // namespace logg
