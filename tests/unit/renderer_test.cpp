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
  std::string buffer;

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
  EXPECT_EQ(buffer, std::string_view("22"));

  EXPECT_EQ(renderer(buffer, logg::level::kDebug, "{}",
                     std::make_format_args("123"), 1),
            std::string_view("123"));
  EXPECT_EQ(buffer, std::string_view("22123"));
}

TEST(renderer, noop) {
  std::string buffer;

  logg::renderer const renderer = logg::renderer::kNoop();

  EXPECT_EQ(renderer(buffer, logg::level::kDebug, "some log",
                     std::make_format_args(), 0),
            std::string_view("some log"));
  EXPECT_EQ(buffer, std::string_view("some log"));
}

TEST(renderer, colored_leveled) {
  std::string buffer = "some prefix";

  logg::renderer const renderer = logg::renderer::kColoredLeveled();

  EXPECT_EQ(renderer(buffer, logg::level::kDebug, "log #{}",
                     std::make_format_args("1"), 1),
            std::string_view(("D\tlog #1\n")));
  EXPECT_EQ(buffer, std::string_view("some prefixD\tlog #{}\nD\tlog #1\n"));

  EXPECT_EQ(renderer(buffer, logg::level::kInfo, "log #{}",
                     std::make_format_args("2"), 1),
            std::string_view(("\x1B[32mI\x1B[0m\tlog #2\n")));
  EXPECT_EQ(buffer,
            std::string_view(
                "some prefixD\tlog #{}\nD\tlog #1\n\x1B[32mI\x1B[0m\tlog "
                "#{}\n\x1B[32mI\x1B[0m\tlog #2\n"));

  EXPECT_EQ(renderer(buffer, logg::level::kDebug, "log #3",
                     std::make_format_args(), 0),
            std::string_view(("D\tlog #3\n")));
  EXPECT_EQ(buffer,
            std::string_view(
                "some prefixD\tlog #{}\nD\tlog #1\n\x1B[32mI\x1B[0m\tlog "
                "#{}\n\x1B[32mI\x1B[0m\tlog #2\nD\tlog #3\n"));
}

}  // namespace tests
