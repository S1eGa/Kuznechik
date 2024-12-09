#include "Types.h"

#include <chrono>
#include <climits>
#include <functional>

inline Block blockFrom(uint64_t upper, uint64_t lower) {
  Block result = upper;
  result <<= sizeof(lower) * CHAR_BIT;
  result += lower;
  return result;
}

template <class TimeT = std::chrono::seconds,
          class ClockT = std::chrono::steady_clock>
struct measure {
  template <class F, class... Args>
  static auto duration(F &&func, Args &&...args) {
    auto start = ClockT::now();
    std::invoke(std::forward<F>(func), std::forward<Args>(args)...);
    return std::chrono::duration_cast<TimeT>(ClockT::now() - start);
  }
};
