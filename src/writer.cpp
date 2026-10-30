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
    if (std::fwrite(log.data(), 1, log.size(), stdout) != log.size())
        [[unlikely]] {
      throw std::runtime_error(
          std::format("writer::operator()(): fwrite(stdout) error {}",
                      std::generic_category().message(errno)));
    }
  };

  return writer(std::move(write));
}

auto writer::kStderr() noexcept -> writer {
  write_type write = [](std::string_view log) -> void {
    if (std::fwrite(log.data(), 1, log.size(), stderr) != log.size())
        [[unlikely]] {
      throw std::runtime_error(
          std::format("writer::operator()(): fwrite(stderr) error {}",
                      std::generic_category().message(errno)));
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
  if (!ostream->is_open()) {
    throw std::runtime_error("writer::writer(): could not open file");
  }

  write_type write =
      [ostream = std::move(ostream)](std::string_view log) mutable -> void {
    *ostream << log;
  };

  return writer(std::move(write));
}

}  // namespace logg
