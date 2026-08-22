#include <logg/logger.hpp>

#include <benchmark/benchmark.h>

namespace benchmarks {
// NOLINTBEGIN(misc-use-internal-linkage,cppcoreguidelines-avoid-magic-numbers)

namespace {

constexpr auto devnull() noexcept -> decltype(auto) {
  return std::unique_ptr<FILE, decltype([](FILE * devnull) -> void {
                           // NOLINTNEXTLINE(cert-err33-c,cppcoreguidelines-owning-memory)
                           std::fclose(devnull);
                         })>(std::fopen("/dev/null", "we"));
}

}  // namespace

constexpr void BM_logger_transparent(benchmark::State & state) noexcept {
  logg::logger const logger(logg::level::kDebug, logg::renderer::kNoop(),
                            logg::writer::kPath("/dev/null"));

  // NOLINTNEXTLINE(readability-identifier-length)
  for (auto const _ : state) {
    benchmark::DoNotOptimize((logger(logg::level::kDebug, "{}\n", 3), nullptr));
  }
}
BENCHMARK(BM_logger_transparent);

constexpr void BM_logger_format(benchmark::State & state) noexcept {
  auto const file = devnull();

  // NOLINTNEXTLINE(readability-identifier-length)
  for (auto const _ : state) {
    benchmark::DoNotOptimize(
        std::fputs(std::format("{}\n", 3).c_str(), file.get()));
  }
}
BENCHMARK(BM_logger_format);

constexpr void BM_logger_fprintf(benchmark::State & state) noexcept {
  auto const file = devnull();

  // NOLINTNEXTLINE(readability-identifier-length)
  for (auto const _ : state) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    benchmark::DoNotOptimize(std::fprintf(file.get(), "%d\n", 3));
  }
}
BENCHMARK(BM_logger_fprintf);

constexpr void BM_logger_fputs(benchmark::State & state) noexcept {
  auto const file = devnull();

  // NOLINTNEXTLINE(readability-identifier-length)
  for (auto const _ : state) {
    benchmark::DoNotOptimize(fputs("3\n", file.get()));
  }
}
BENCHMARK(BM_logger_fputs);

// NOLINTEND(misc-use-internal-linkage,cppcoreguidelines-avoid-magic-numbers)
}  // namespace benchmarks
