#include <logg/writer.hpp>

#include <sstream>

#include <gtest/gtest.h>

namespace tests {

TEST(writer, size) {
  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
  // NOLINTBEGIN(readability-magic-numbers)
  static_assert(sizeof(logg::writer) == 32);
  static_assert(alignof(logg::writer) == 8);
  // NOLINTEND(readability-magic-numbers)
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
}

TEST(writer, smoke) {
  std::string buffer;

  logg::writer const writer(
      [&buffer](std::string_view log) -> void { buffer = log; });

  writer("a log");
  EXPECT_EQ(buffer, "a log");

  writer("another log");
  EXPECT_EQ(buffer, "another log");
}

TEST(writer, null) {
  logg::writer const writer = logg::writer::kNull();

  testing::internal::CaptureStdout();
  testing::internal::CaptureStderr();

  writer("whatever");

  EXPECT_TRUE(testing::internal::GetCapturedStdout().empty());
  EXPECT_TRUE(testing::internal::GetCapturedStderr().empty());
}

TEST(writer, _stdout) {
  logg::writer const writer = logg::writer::kStdout();

  testing::internal::CaptureStdout();
  testing::internal::CaptureStderr();

  writer("whatever");

  EXPECT_EQ(testing::internal::GetCapturedStdout(), "whatever");
  EXPECT_TRUE(testing::internal::GetCapturedStderr().empty());
}

TEST(writer, _stderr) {
  logg::writer const writer = logg::writer::kStderr();

  testing::internal::CaptureStdout();
  testing::internal::CaptureStderr();

  writer("whatever");

  EXPECT_TRUE(testing::internal::GetCapturedStdout().empty());
  EXPECT_EQ(testing::internal::GetCapturedStderr(), "whatever");
}

TEST(writer, ostream) {
  std::ostringstream oss;

  logg::writer const writer = logg::writer::kStream(oss);

  writer("some log");

  EXPECT_EQ(oss.str(), "some log");
}

}  // namespace tests
