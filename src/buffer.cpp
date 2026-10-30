#include <logg/buffer.hpp>

#include <variant>

#include <swiss/overloaded.hpp>

namespace logg {

auto buffer::kStatic() noexcept -> buffer {
  return buffer(shared_storage::kStatic);
}

auto buffer::kThreadLocal() noexcept -> buffer {
  return buffer(shared_storage::kThreadLocal);
}

auto buffer::kLocal(bounds_type bounds) noexcept -> buffer {
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

void buffer::set_bounds(buffer::bounds_type bounds) {
  auto & [storage, current_bounds] = visit();

  current_bounds = bounds;
}

buffer::buffer(impl && local) : local_or_shared_data_(std::move(local)) {}

buffer::buffer(shared_storage shared) noexcept
    : local_or_shared_data_(shared) {}

auto buffer::visit() noexcept -> impl & {
  return std::visit(
      swiss::overloaded(
          [](buffer::impl & local_data) noexcept -> buffer::impl & {
            return local_data;
          },
          [](shared_storage shared) noexcept -> buffer::impl & {
            if (shared == shared_storage::kStatic) {
              static impl data({}, std::array<std::size_t, 2>{});
              return data;
            }

            thread_local impl data({}, std::array<std::size_t, 2>{});
            return data;
          }),
      local_or_shared_data_);
}

}  // namespace logg
