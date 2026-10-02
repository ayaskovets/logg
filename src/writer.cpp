#include <logg/writer.hpp>

#include <cstdio>
#include <format>
#include <fstream>
#include <system_error>

namespace logg {

auto writer::kNull() noexcept -> writer {
  write_type write = [](std::string_view) -> void {};

  return writer(std::move(write));
}

auto writer::kStdout() noexcept -> writer {
  write_type write = [](std::string_view log) -> void {
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
  write_type write = [](std::string_view log) -> void {
    // NOLINTNEXTLINE(bugprone-suspicious-stringview-data-usage)
    if (std::fwrite(log.data(), log.size(), 1, stderr) != 1) [[unlikely]] {
      throw std::runtime_error(
          std::format("writer::operator()(): fwrite(stderr) error {}",
                      std::generic_category().message(std::ferror(stderr))));
    }
  };

  return writer(std::move(write));
}

auto writer::kStream(std::ostream & ostream) noexcept -> writer {
  write_type write = [&ostream](std::string_view log) -> void {
    ostream << log;
  };

  return writer(std::move(write));
}

auto writer::kPath(std::filesystem::path const & path) -> writer {
  auto ostream = std::make_shared<std::ofstream>(path.c_str(),
                                                 std::ios::out | std::ios::app);
  if (!ostream || ostream->is_open()) {
    throw std::runtime_error("writer::writer(): could not open file");
  }

  write_type write =
      [ostream = std::move(ostream)](std::string_view log) mutable -> void {
    *ostream << log;
  };

  return writer(std::move(write));
}

}  // namespace logg
