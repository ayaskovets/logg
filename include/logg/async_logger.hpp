#pragma once

#include <logg/level.hpp>
#include <logg/overflow_policy.hpp>
#include <logg/renderer.hpp>
#include <logg/writer.hpp>

#include <thread>
#include <type_traits>

#include <flow/spsc_msgbuf.hpp>

namespace logg {

/**
 * @brief Logger with asynchronous fmt parsing and logging done in a dedicated
 * thread
 * @note Log level is immutable. The logger is thread-safe for 1 producer and 1
 * consumer. The data logged
 * @warning
 */
template <overflow_policy Policy>
class async_logger final {
 private:
  static constexpr std::size_t kBufferCapacity = 4096;

 public:
  constexpr async_logger(level level, renderer renderer, writer writer)
      : level_(level),
        renderer_(renderer),
        writer_(std::move(writer)),
        thread_([this](std::stop_token const & stop_token) noexcept {
          async_logging_task(stop_token);
        }) {}

 public:
  template <typename... Args>
    requires(std::is_trivially_copyable_v<Args> && ...)
  constexpr void operator()(level level, std::format_string<Args...> fmt,
                            Args const &... args) const {
    if (level_ > level) [[unlikely]] {
      return;
    }

    std::size_t msg_size     = 0;  // TODO: calculate size for serialized args
    auto const push_callback = [&](std::span<std::byte> span) {
      // TODO: serialize args & format string
    };

    if constexpr (Policy == overflow_policy::kDrop) {
      msgbuf_.try_push(msg_size, push_callback);
    } else if constexpr (Policy == overflow_policy::kBlock) {
      msgbuf_.push(thread_.get_stop_token(), msg_size, push_callback);
    } else {
      static_assert(false, "Unsupported overflow policy");
    }
  }

 private:
  void async_logging_task(std::stop_token const & stop_token) noexcept;

 private:
  // NOLINTBEGIN(cppcoreguidelines-avoid-const-or-ref-data-members)
  level const level_;
  renderer const renderer_;
  writer const writer_;
  // NOLINTEND(cppcoreguidelines-avoid-const-or-ref-data-members)

  std::jthread thread_;
  flow::spsc_msgbuf<std::uint16_t, kBufferCapacity> msgbuf_;
};

}  // namespace logg
