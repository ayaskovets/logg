#include <logg/renderer.hpp>

#include <string_view>

#include <gtest/gtest.h>

namespace tests {

TEST(renderer, size) {
  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
  // NOLINTBEGIN(readability-magic-numbers)
  static_assert(sizeof(logg::renderer) == 64);
  static_assert(alignof(logg::renderer) == 8);
  // NOLINTEND(readability-magic-numbers)
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
}

TEST(renderer, smoke) {
  logg::renderer const renderer(
      [](std::string_view fmt, std::format_args args, std::size_t, logg::level,
         std::string & buffer) -> std::string_view {
        std::size_t const size = buffer.size();
        std::vformat_to(std::back_inserter(buffer), fmt, args);
        return std::string_view(buffer).substr(size);
      },
      logg::buffer::kLocal(std::array<std::size_t, 2>{24, 24}));

  EXPECT_EQ(renderer("{}", std::make_format_args("22"), 1, logg::level::kDebug),
            std::string_view("22"));

  EXPECT_EQ(
      renderer("{}", std::make_format_args("123"), 1, logg::level::kDebug),
      std::string_view("123"));
}

TEST(renderer, noop) {
  logg::renderer const renderer = logg::renderer::kNoop(
      logg::buffer::kLocal(std::array<std::size_t, 2>{24, 24}));

  EXPECT_EQ(
      renderer("some log", std::make_format_args(), 0, logg::level::kDebug),
      std::string_view("some log"));
}

TEST(renderer, leveled) {
  logg::renderer const renderer = logg::renderer::kLeveled(
      logg::buffer::kLocal(std::array<std::size_t, 2>{24, 24}));

  EXPECT_EQ(
      renderer("log #{}", std::make_format_args("1"), 1, logg::level::kDebug),
      std::string_view("Debug\tlog #1\n"));

  EXPECT_EQ(
      renderer("some log", std::make_format_args(), 0, logg::level::kDebug),
      std::string_view("Debug\tsome log\n"));
}

TEST(renderer, colored_leveled) {
  logg::renderer const renderer = logg::renderer::kColoredLeveled(
      logg::buffer::kLocal(std::array<std::size_t, 2>{24, 24}));

  EXPECT_EQ(
      renderer("log #{}", std::make_format_args("1"), 1, logg::level::kDebug),
      std::string_view("Debug\tlog #1\n"));

  EXPECT_EQ(
      renderer("some log", std::make_format_args(), 0, logg::level::kDebug),
      std::string_view("Debug\tsome log\n"));
}

}  // namespace tests
