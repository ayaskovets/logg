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
      [](std::string_view fmt, std::format_args args, logg::level,
         std::string & buffer) -> std::string_view {
        std::size_t const size = buffer.size();
        std::vformat_to(std::back_inserter(buffer), fmt, args);
        return std::string_view(buffer).substr(size);
      },
      logg::buffer::kLocal(std::array<std::size_t, 2>{24, 24}));

  EXPECT_EQ(renderer("{}", std::make_format_args("22"), logg::level::kDebug),
            std::string_view("22"));

  EXPECT_EQ(renderer("{}", std::make_format_args("123"), logg::level::kDebug),
            std::string_view("123"));
}

TEST(renderer, noop) {
  logg::renderer const renderer = logg::renderer::kNoop(
      logg::buffer::kLocal(std::array<std::size_t, 2>{24, 24}));

  EXPECT_EQ(renderer("some log", std::make_format_args(), logg::level::kDebug),
            std::string_view("some log"));
}

TEST(renderer, leveled) {
  logg::renderer const renderer = logg::renderer::kLeveled(
      logg::buffer::kLocal(std::array<std::size_t, 2>{24, 24}));

  EXPECT_EQ(
      renderer("log #{}", std::make_format_args("1"), logg::level::kDebug),
      std::string_view("Debug\tlog #1\n"));

  EXPECT_EQ(renderer("some log", std::make_format_args(), logg::level::kDebug),
            std::string_view("Debug\tsome log\n"));
}

TEST(renderer, colored_leveled) {
  logg::renderer const renderer = logg::renderer::kColoredLeveled(
      logg::buffer::kLocal(std::array<std::size_t, 2>{24, 24}));

  EXPECT_EQ(
      renderer("log #{}", std::make_format_args("1"), logg::level::kDebug),
      std::string_view("Debug\tlog #1\n"));

  EXPECT_EQ(renderer("some log", std::make_format_args(), logg::level::kDebug),
            std::string_view("Debug\tsome log\n"));
}

TEST(renderer, buffer_resize) {
  std::string const kLargeString(8192, 'x');
  std::string const kLiteral(8192, 'y');
  std::string const kFormat   = "{}" + kLiteral + "{}";
  std::string const kExpected = kLargeString + kLiteral + kLargeString;

  {
    logg::renderer const renderer = logg::renderer::kNoop(
        logg::buffer::kLocal(std::array<std::size_t, 2>{24, 24}));

    EXPECT_EQ(
        renderer(kFormat, std::make_format_args(kLargeString, kLargeString),
                 logg::level::kInfo),
        kExpected);
  }

  {
    logg::renderer const renderer = logg::renderer::kLeveled(
        logg::buffer::kLocal(std::array<std::size_t, 2>{24, 24}));

    EXPECT_EQ(
        renderer(kFormat, std::make_format_args(kLargeString, kLargeString),
                 logg::level::kInfo),
        "Info\t" + kExpected + '\n');
  }

  {
    logg::renderer const renderer = logg::renderer::kColoredLeveled(
        logg::buffer::kLocal(std::array<std::size_t, 2>{24, 24}));

    EXPECT_EQ(
        renderer(kFormat, std::make_format_args(kLargeString, kLargeString),
                 logg::level::kInfo),
        "\x1B[32mInfo\x1B[0m\t" + kExpected + '\n');
  }

  {
    logg::renderer const renderer = logg::renderer::kColoredLeveledTimestamped(
        logg::buffer::kLocal(std::array<std::size_t, 2>{24, 24}));

    auto const result =
        renderer(kFormat, std::make_format_args(kLargeString, kLargeString),
                 logg::level::kInfo);
    EXPECT_EQ(result.substr(result.find('\t') + 1),
              "\x1B[32mInfo\x1B[0m\t" + kExpected + '\n');
  }
}

}  // namespace tests
