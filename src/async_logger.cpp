#include <logg/async_logger.hpp>

#include <stop_token>

#include "logg/overflow_policy.hpp"

namespace logg {

template <overflow_policy Policy>
void async_logger<Policy>::async_logging_task(
    std::stop_token const & stop_token) noexcept {
  // TODO: impl logging task
}

template void async_logger<overflow_policy::kDrop>::async_logging_task(
    std::stop_token const & stop_token);
template void async_logger<overflow_policy::kBlock>::async_logging_task(
    std::stop_token const & stop_token);

}  // namespace logg
