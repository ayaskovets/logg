#include <logg/buffer.hpp>

#include <variant>

#include <swiss/overloaded.hpp>

namespace logg {

auto buffer::kStatic() noexcept -> buffer {
  static impl impl({}, std::array<std::size_t, 2>{});
  return buffer(&impl);
}

auto buffer::kThreadLocal() noexcept -> buffer {
  thread_local impl impl({}, std::array<std::size_t, 2>{});
  return buffer(&impl);
}

auto buffer::kLocal(bounds_t bounds) noexcept -> buffer {
  impl impl({}, bounds);
  return buffer(std::move(impl));
}

auto buffer::reset() -> std::string & {
  auto & [storage, bounds] = visit();

  if (storage.capacity() > bounds[1]) [[unlikely]] {
    /**
     * @note Guaranteed way to free the underlying memory
     */
    storage = std::string();
  }

  if (storage.capacity() < bounds[0]) [[unlikely]] {
    storage.reserve(bounds[0]);
  }

  storage.clear();

  return storage;
}

void buffer::set_bounds(buffer::bounds_t bounds) {
  auto & [storage, current_bounds] = visit();

  current_bounds = bounds;
}

buffer::buffer(impl && local) : local_or_shared_data_(std::move(local)) {
  // NOLINTNEXTLINE(readability-identifier-length)
  auto & [_, bounds] = visit();
  set_bounds(bounds);
}

buffer::buffer(impl * shared) noexcept : local_or_shared_data_(shared) {
  // NOLINTNEXTLINE(readability-identifier-length)
  auto & [_, bounds] = visit();
  set_bounds(bounds);
}

auto buffer::visit() noexcept -> impl & {
  return std::visit(
      swiss::overloaded(
          [](buffer::impl & local_data) noexcept -> buffer::impl & {
            return local_data;
          },
          [](buffer::impl * shared_data) noexcept -> buffer::impl & {
            return *shared_data;
          }),
      local_or_shared_data_);
}

}  // namespace logg
