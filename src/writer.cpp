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
    if (std::fwrite(log.data(), log.size(), 1, stdout) != 1) [[unlikely]] {
      throw std::runtime_error(
          std::format("writer::operator()(): fwrite(stdout) error {}",
                      std::generic_category().message(std::ferror(stdout))));
    }
  };

  return writer(std::move(write));
}

auto writer::kStderr() noexcept -> writer {
  write_t write = [](std::string_view log) -> void {
    // NOLINTNEXTLINE(bugprone-suspicious-stringview-data-usage)
    if (std::fwrite(log.data(), log.size(), 1, stderr) != 1) [[unlikely]] {
      throw std::runtime_error(
          std::format("writer::operator()(): fwrite(stderr) error {}",
                      std::generic_category().message(std::ferror(stderr))));
    }
  };

  return writer(std::move(write));
}

auto writer::kFile(FILE * file) noexcept -> writer {
  write_t write = [file](std::string_view log) -> void {
    // NOLINTNEXTLINE(bugprone-suspicious-stringview-data-usage)
    if (std::fwrite(log.data(), log.size(), 1, file) != 1) [[unlikely]] {
      throw std::runtime_error(
          std::format("writer::operator()(): fwrite({}) error {}",
                      static_cast<void *>(file),
                      std::generic_category().message(std::ferror(file))));
    }
  };

  return writer(std::move(write));
}

auto writer::kStream(std::ostream & ostream) noexcept -> writer {
  write_t write = [&ostream](std::string_view log) -> void { ostream << log; };

  return writer(std::move(write));
}

}  // namespace logg
