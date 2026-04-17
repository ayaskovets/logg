#include <logg/logger.hpp>

#include <sstream>
#include <string>
#include <string_view>

#include <gtest/gtest.h>

#include "logg/renderer.hpp"
#include "logg/writer.hpp"

namespace tests::unit {

TEST(logger, size) {
  // NOLINTBEGIN(readability-magic-numbers)
  static_assert(sizeof(logg::logger) == 48);
  static_assert(alignof(logg::logger) == 8);
  // NOLINTEND(readability-magic-numbers)
}

TEST(logger, level_check) {
  std::size_t logs_count = 0;

  auto const level = logg::level::kWarn;
  logg::renderer const renderer(logg::renderer::kNoop());
  logg::writer const writer(
      [&logs_count](std::string_view) { logs_count += 1; });
  logg::logger const logger(level, renderer, writer);

  logger(logg::level::kDebug, "");
  EXPECT_EQ(logs_count, 0);

  logger(logg::level::kInfo, "");
  EXPECT_EQ(logs_count, 0);

  logger(logg::level::kWarn, "");
  EXPECT_EQ(logs_count, 1);

  logger(logg::level::kError, "");
  EXPECT_EQ(logs_count, 2);
}

TEST(logger, buffer_resizing) {
  std::ostringstream oss;

  auto const level = logg::level::kDebug;
  logg::renderer const renderer(logg::renderer::kNoop());
  logg::writer const writer(logg::writer::kStream(oss));
  logg::logger const logger(level, renderer, writer);

  std::string const kLargeString(2048, 'l');
  std::string const kSmallString(32, 's');

  logger(logg::level::kDebug, "{}", kLargeString.c_str());
  EXPECT_EQ(oss.str(), kLargeString);
  logger(logg::level::kDebug, "{}", kSmallString.c_str());
  EXPECT_EQ(oss.str(), kLargeString + kSmallString);
  logger(logg::level::kDebug, "{}", kLargeString.c_str());
  EXPECT_EQ(oss.str(), kLargeString + kSmallString + kLargeString);
  logger(logg::level::kDebug, "{}", kSmallString.c_str());
  EXPECT_EQ(oss.str(),
            kLargeString + kSmallString + kLargeString + kSmallString);
}

}  // namespace tests::unit
