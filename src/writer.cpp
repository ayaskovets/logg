#include <logg/writer.hpp>

#include <cstdio>
#include <format>
#include <system_error>

namespace logg {

auto writer::kNull() noexcept -> writer {
  write_t write = [](std::string_view) -> void {};

  return writer(std::move(write));
}

auto writer::kStdout() noexcept -> writer {
  write_t write = [](std::string_view log) -> void {
    // NOLINTNEXTLINE(bugprone-suspicious-stringview-data-usage)
    if (std::fputs(log.data(), stdout) == EOF) [[unlikely]] {
      throw std::runtime_error(
          std::format("fputs(stdout) failed: {}",
                      std::generic_category().message(std::ferror(stdout))));
    }
  };

  return writer(std::move(write));
}

auto writer::kStderr() noexcept -> writer {
  write_t write = [](std::string_view log) -> void {
    // NOLINTNEXTLINE(bugprone-suspicious-stringview-data-usage)
    if (std::fputs(log.data(), stderr) == EOF) [[unlikely]] {
      throw std::runtime_error(
          std::format("fputs(stderr) failed: {}",
                      std::generic_category().message(std::ferror(stderr))));
    }
  };

  return writer(std::move(write));
}

auto writer::kFile(FILE * file) noexcept -> writer {
  write_t write = [file](std::string_view log) -> void {
    // NOLINTNEXTLINE(bugprone-suspicious-stringview-data-usage)
    if (std::fputs(log.data(), file) == EOF) [[unlikely]] {
      throw std::runtime_error(
          std::format("fputs({}) failed: {}", static_cast<void *>(file),
                      std::generic_category().message(std::ferror(stderr))));
    }
  };

  return writer(std::move(write));
}

auto writer::kStream(std::ostream & ostream) noexcept -> writer {
  write_t write = [&ostream](std::string_view log) -> void { ostream << log; };

  return writer(std::move(write));
}

}  // namespace logg
