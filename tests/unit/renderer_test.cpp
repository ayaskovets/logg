#include <logg/renderer.hpp>

#include <string_view>

#include <gtest/gtest.h>

namespace tests {

TEST(renderer, size) {
  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
  // NOLINTBEGIN(readability-magic-numbers)
  static_assert(sizeof(logg::renderer) == 8);
  static_assert(alignof(logg::renderer) == 8);
  // NOLINTEND(readability-magic-numbers)
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
}

TEST(renderer, smoke) {
  std::string buffer = "prefix";

  logg::renderer const renderer([](std::string & buffer, logg::level,
                                   std::string_view fmt, std::format_args args,
                                   std::size_t) -> std::string_view {
    std::size_t const size = buffer.size();
    std::vformat_to(std::back_inserter(buffer), fmt, args);
    return std::string_view(buffer).substr(size);
  });

  EXPECT_EQ(renderer(buffer, logg::level::kDebug, "{}",
                     std::make_format_args("22"), 1),
            std::string_view("22"));

  EXPECT_EQ(renderer(buffer, logg::level::kDebug, "{}",
                     std::make_format_args("123"), 1),
            std::string_view("123"));
}

TEST(renderer, noop) {
  std::string buffer = "prefix";

  logg::renderer const renderer = logg::renderer::kNoop();

  EXPECT_EQ(renderer(buffer, logg::level::kDebug, "some log",
                     std::make_format_args(), 0),
            std::string_view("some log"));
}

TEST(renderer, leveled) {
  std::string buffer = "prefix";

  logg::renderer const renderer = logg::renderer::kLeveled();

  EXPECT_EQ(renderer(buffer, logg::level::kDebug, "log #{}",
                     std::make_format_args("1"), 1),
            std::string_view("Debug\tlog #1\n"));

  buffer.clear();
  EXPECT_EQ(renderer(buffer, logg::level::kDebug, "some log",
                     std::make_format_args(), 0),
            std::string_view("Debug\tsome log\n"));
}

TEST(renderer, colored_leveled) {
  std::string buffer = "prefix";

  logg::renderer const renderer = logg::renderer::kColoredLeveled();

  EXPECT_EQ(renderer(buffer, logg::level::kDebug, "log #{}",
                     std::make_format_args("1"), 1),
            std::string_view("Debug\tlog #1\n"));

  buffer.clear();
  EXPECT_EQ(renderer(buffer, logg::level::kDebug, "some log",
                     std::make_format_args(), 0),
            std::string_view("Debug\tsome log\n"));
}

}  // namespace tests
