#include <logg/writer.hpp>

#include <cstdio>
#include <format>
#include <system_error>

namespace logg {

auto writer::kNoop() noexcept -> writer {
  write_t const write = [](std::string_view) {};

  return writer(write);
}

auto writer::kStdout() noexcept -> writer {
  write_t const write = [](std::string_view log) {
    // NOLINTNEXTLINE(bugprone-suspicious-stringview-data-usage)
    if (std::fputs(log.data(), stdout) == EOF) [[unlikely]] {
      throw std::runtime_error(
          std::format("fputs(stdout) failed: {}",
                      std::generic_category().message(std::ferror(stdout))));
    }
  };

  return writer(write);
}

auto writer::kStderr() noexcept -> writer {
  write_t const write = [](std::string_view log) {
    // NOLINTNEXTLINE(bugprone-suspicious-stringview-data-usage)
    if (std::fputs(log.data(), stderr) == EOF) [[unlikely]] {
      throw std::runtime_error(
          std::format("fputs(stderr) failed: {}",
                      std::generic_category().message(std::ferror(stderr))));
    }
  };

  return writer(write);
}

auto writer::kStream(std::ostream & ostream) noexcept -> writer {
  write_t const write = [&ostream](std::string_view log) { ostream << log; };

  return writer(write);
}

}  // namespace logg
