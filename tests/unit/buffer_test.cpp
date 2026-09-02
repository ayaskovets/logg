#include <logg/buffer.hpp>

#include <thread>

#include <gtest/gtest.h>

namespace tests {

TEST(buffer, size) {
  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
  // NOLINTBEGIN(readability-magic-numbers)
  static_assert(sizeof(logg::buffer) == 56);
  static_assert(alignof(logg::buffer) == 8);
  // NOLINTEND(readability-magic-numbers)
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
}

TEST(buffer, reset) {
  auto const bounds = std::array<std::size_t, 2>{32, 128};
  auto buffer       = logg::buffer::kLocal(bounds);

  auto & data = buffer.reset();
  EXPECT_TRUE(data.empty());

  data.resize(16);
  auto const initial_capacity = data.capacity();
  buffer.reset();
  EXPECT_TRUE(data.empty());
  EXPECT_EQ(data.capacity(), initial_capacity);

  data.resize(64);
  auto const intermediate_capacity = data.capacity();
  buffer.reset();
  EXPECT_TRUE(data.empty());
  EXPECT_EQ(data.capacity(), intermediate_capacity);

  data.resize(256);
  buffer.reset();
  EXPECT_TRUE(data.empty());
  EXPECT_EQ(data.capacity(), initial_capacity);
}

TEST(buffer, static_instance) {
  auto & data                 = logg::buffer::kStatic().reset();
  auto const initial_capacity = data.capacity();

  EXPECT_TRUE(data.empty());
  EXPECT_EQ(data.capacity(), initial_capacity);

  logg::buffer::kStatic().set_bounds(std::array<std::size_t, 2>{1024, 1024});
  logg::buffer::kStatic().reset();
  EXPECT_TRUE(data.empty());
  EXPECT_NE(data.capacity(), initial_capacity);
}

TEST(buffer, tls_instance) {
  auto & data                 = logg::buffer::kThreadLocal().reset();
  auto const initial_capacity = data.capacity();

  EXPECT_TRUE(data.empty());
  EXPECT_EQ(data.capacity(), initial_capacity);

  logg::buffer::kThreadLocal().set_bounds(
      std::array<std::size_t, 2>{1024, 1024});
  logg::buffer::kThreadLocal().reset();
  EXPECT_TRUE(data.empty());
  EXPECT_NE(data.capacity(), initial_capacity);

  std::jthread const thread([initial_capacity]() noexcept -> void {
    auto & data = logg::buffer::kThreadLocal().reset();
    EXPECT_EQ(data.capacity(), initial_capacity);
  });
}

}  // namespace tests
